#include "stdafx.h"

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
#include "sdl/SDL.h"
#include "sdl/SDL_main.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <SDL2/SDL.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#endif


#define MAX_AUDIO_FRAME_SIZE 192000

#define OUTPUT_PCM 1  //输出的PCM文件还是有问题

#define USE_SDL 1

static Uint8 *audio_chunk;
static  Uint32  audio_len;
static  Uint8  *audio_pos;

void fill_audio(void *udata, Uint8 *stream, int len) {
	SDL_memset(stream, 0, len);
	if (audio_len == 0)
		return;
	len = (len > audio_len ? audio_len : len);
	SDL_MixAudio(stream, audio_pos,len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

int audio_play() {
	AVFormatContext *pFormatCtx;
	int i, audioStream;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	AVPacket *packet;
	uint8_t *out_buffer;
	AVFrame *pFrame;
	SDL_AudioSpec wanted_spec;
	int ret;

	uint32_t len = 0;
	int got_picture;
	int index = 0;
	int64_t in_channel_layout;
	struct SwrContext *au_convert_ctx;
	FILE *pFile = NULL;
	char url[] = "test.mp3";
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) {
		printf("Couldn't open input stream.\n");
		return -1;
	}
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		printf("Couldn't find stream information.\n");
		return	-1;
	}
	av_dump_format(pFormatCtx, 0, url, false);
	audioStream = -1;
	for(i=0;i<pFormatCtx->nb_streams;i++)
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
			audioStream = i;
			break;
		}

	if (audioStream == -1) {
		printf("Didn't find a audio stream.\n");
		return -1;
	}

	pCodecCtx = pFormatCtx->streams[audioStream]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		printf("Codec not found.\n");
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("Could not open codec.\n");
		return -1;
	}

#if OUTPUT_PCM
	pFile = fopen("output.pcm", "wb");
#endif

	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(packet);

	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
	int out_nb_samples = pCodecCtx->frame_size;
	AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = 44100;
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
	int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
	out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
	pFrame = av_frame_alloc();

#if USE_SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}
	wanted_spec.freq = out_sample_rate;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = out_channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = out_nb_samples;
	wanted_spec.callback = fill_audio;
	wanted_spec.userdata = pCodecCtx;
	if (SDL_OpenAudio(&wanted_spec, NULL) < 0) {
		printf("can't open audio.\n");
		return -1;
	}
#endif

	in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);
	au_convert_ctx = swr_alloc();
	au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,
		in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
	swr_init(au_convert_ctx);
	SDL_PauseAudio(0);

	while (av_read_frame(pFormatCtx, packet) >= 0) {
		if (packet->stream_index == audioStream) {
			ret = avcodec_decode_audio4(pCodecCtx, pFrame, &got_picture, packet);
			if (ret < 0) {
				printf("Error in decoding audio frame.\n");
				return -1;
			}
			if (got_picture > 0) {
				swr_convert(au_convert_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)pFrame->data, pFrame->nb_samples);
#if 1
				printf("index:%5d\t pts:%lld\t packet size:%d\n", index, packet->pts, packet->size);
#endif
#if OUTPUT_PCM
				//Write PCM
				fwrite(out_buffer, 1, out_buffer_size, pFile);
#endif
				index++;
			}
#if USE_SDL
			while (audio_len > 0) {
				SDL_Delay(1);
			}
			audio_chunk = (Uint8 *)out_buffer;
			audio_len = out_buffer_size;
			audio_pos = audio_chunk;
#endif
		}
		av_free_packet(packet);
	}
	swr_free(&au_convert_ctx);
#if  USE_SDL
	SDL_CloseAudio();
	SDL_Quit();
#endif //  USE_SDL
#if OUTPUT_PCM
	fclose(pFile);
#endif // OUTPUT_PCM
	av_free(out_buffer);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	return 0;
}