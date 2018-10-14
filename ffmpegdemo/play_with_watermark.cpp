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
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/avutil.h"
#include "SDL.h"
#include "SDL_main.h"
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
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <SDL2/SDL.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#endif

#define ENABLE_SDL 1


//drawtext = fontfile = arial.ttf:fontcolor = red : fontsize = 30 : text = 'Lei Xiaohua' : x = 50 : y = 50
const char * filer_descr = "drawbox=10:20:200:60:red@0.5";
static AVFormatContext *pFormatCtx;
static AVCodecContext *pCodecCtx;
AVFilterContext *buffersink_ctx;
AVFilterContext *buffersrc_ctx;
AVFilterGraph *filter_graph;
static int video_stream_index = -1;



#define SFM_REFRESH_EVENT (SDL_USEREVENT+1)
#define SFM_BREAK_EVENT (SDL_USEREVENT+2)
int thread_exit_wm = 0;

int thread_pause_wm = 0;

int sfp_refresh_thread_wm(void * opaque) {
	thread_exit_wm = 0;
	thread_pause_wm = 0;
	while (!thread_exit_wm)
	{
		if (!thread_pause_wm) {
			SDL_Event event;
			event.type = SFM_REFRESH_EVENT;
			SDL_PushEvent(&event);
		}
		SDL_Delay(40);
	}
	thread_exit_wm = 0;
	thread_pause_wm = 0;
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);
	return 0;
}

static int open_input_file(const char *filename) {
	int ret = 0;
	AVCodec *dec;
	if ((ret == avformat_open_input(&pFormatCtx, filename, NULL, NULL)) < 0) {
		printf("Cannot open input file\n");
		return ret;
	}
	if ((ret = avformat_find_stream_info(pFormatCtx, NULL)) < 0) {
		printf("Cannot find stream information\n");
		return ret;
	}
	ret = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
	if (ret < 0) {
		printf("Cannot find a video stream in the input file\n");
		return ret;
	}
	video_stream_index = ret;
	pCodecCtx = pFormatCtx->streams[video_stream_index]->codec;
	if ((ret = avcodec_open2(pCodecCtx, dec, NULL)) < 0) {
		printf("Cannot open video decoder\n");
		return ret;
	}
	return 0;
}

static int init_filters(const char *filters_descr) {
	char args[512];
	int ret = 0;
	const AVFilter *buffersrc = avfilter_get_by_name("buffer");
	const AVFilter *buffersink = avfilter_get_by_name("buffersink");
	AVFilterInOut *outputs = avfilter_inout_alloc();
	AVFilterInOut *inputs = avfilter_inout_alloc();
	enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P,AV_PIX_FMT_NONE };
	AVBufferSinkParams *buffersink_params;
	filter_graph = avfilter_graph_alloc();
	snprintf(args, sizeof(args), "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d", pCodecCtx->width,
		pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->time_base.num, pCodecCtx->time_base.den,
		pCodecCtx->sample_aspect_ratio.num, pCodecCtx->sample_aspect_ratio.den);
	ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",args, NULL, filter_graph);
	if (ret < 0) {
		printf("Cannot create buffer source\n");
		return ret;
	}
	buffersink_params = av_buffersink_params_alloc();
	buffersink_params->pixel_fmts = pix_fmts;
	ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out", NULL, buffersink_params, filter_graph);
	av_free(buffersink_params);
	if (ret < 0) {
		printf("Cannot create buffer sink\n");
		return ret;
	}
	outputs->name = av_strdup("in");
	outputs->filter_ctx = buffersrc_ctx;
	outputs->pad_idx = 0;
	outputs->next = NULL;

	inputs->name = av_strdup("out");
	inputs->filter_ctx = buffersink_ctx;
	inputs->pad_idx = 0;
	inputs->next = NULL;

	if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr, &inputs, &outputs, NULL)) < 0) {
		return ret;
	}
	if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0) {
		return ret;
	}
	return 0;
}

int watermark_play(const char* file) {

	AVFrame	*pFrame,*pFrame_out,*pFrameYUV;
	AVPacket *packet;
	int ret,got_frame;
	unsigned char *out_buffer;
	av_register_all();
	avformat_network_init();
	avfilter_register_all();
	if ((ret = open_input_file(file)) < 0) {
		goto end;
	}
	if ((ret = init_filters(filer_descr)) < 0) {
		goto end;
	}

#if ENABLE_SDL

	//------------SDL----------------
	int screen_w, screen_h;
	SDL_Window *screen;
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Rect sdlRect;
	SDL_Thread *video_tid;
	SDL_Event event;

	struct  SwsContext *img_convert_ctx;
		
	
#endif
	
	pFrame = av_frame_alloc();
	pFrame_out = av_frame_alloc();
	pFrameYUV = av_frame_alloc();


	out_buffer = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);



	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		printf("Could not initialize SDL -%s \n", SDL_GetError());
	}
	else {
		printf("Success init SDL");
	}
	screen_w = pCodecCtx->width;
	screen_h = pCodecCtx->height;
	screen = SDL_CreateWindow("ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_OPENGL);
	if (!screen) {
		printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
		return -1;
	}

	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
	sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, screen_w, screen_h);
	sdlRect.x = 0;
	sdlRect.y = 0;
	sdlRect.w = screen_w;
	sdlRect.h = screen_h;

	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	video_tid = SDL_CreateThread(sfp_refresh_thread_wm, NULL, NULL);


	while (1)
	{
		SDL_WaitEvent(&event);
		if (event.type == SFM_REFRESH_EVENT) {
			while (1)
			{
				if (av_read_frame(pFormatCtx, packet) < 0)
					thread_exit_wm = 1;
				if (packet->stream_index == video_stream_index)
					break;
			}
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_frame, packet);
			if (ret < 0) {
				printf("Decode Error.\n");
				return -1;
			}
			if (got_frame) {

				pFrame->pts = av_frame_get_best_effort_timestamp(pFrame);
				if (av_buffersrc_add_frame(buffersrc_ctx, pFrame) < 0) {
					printf("Error while feeding the filtergraph\n");
					break;
				}
				while (av_buffersink_get_frame(buffersink_ctx, pFrame_out) >= 0)
				{
					printf("Process 1 frame!\n");
					if (pFrame_out->format == AV_PIX_FMT_YUV420P) {
						sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame_out->data, pFrame_out->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
						//SDL---------------------------
						SDL_UpdateTexture(sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0]);
						SDL_RenderClear(sdlRenderer);
						//SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );  
						SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
						SDL_RenderPresent(sdlRenderer);
					}
					av_frame_unref(pFrame_out);
				}

			
			}
			av_free_packet(packet);
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_SPACE)
				thread_pause_wm = !thread_pause_wm;
		}
		else if (event.type == SDL_QUIT) {
			thread_exit_wm = 1;
		}
		else if (event.type == SFM_BREAK_EVENT) {
			break;
		}
		av_frame_unref(pFrame);
	}
	sws_freeContext(img_convert_ctx);
	SDL_Quit();
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
end:
	avfilter_graph_free(&filter_graph);
	if (pCodecCtx)
		avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);


	if (ret < 0 && ret != AVERROR_EOF) {
		char buf[1024];
		av_strerror(ret, buf, sizeof(buf));
		printf("Error occurred: %s\n", buf);
		return -1;
	}

	
	return 0;
}