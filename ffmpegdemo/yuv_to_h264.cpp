#include "stdafx.h"

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#ifdef __cplusplus
};
#endif
#endif

int flush_video_encoder(AVFormatContext * fmt_ctx, unsigned int stream_index) {
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities & AV_CODEC_CAP_DELAY)) {
		return 0;
	}
	while (1)
	{
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_video2(fmt_ctx->streams[stream_index]->codec, &enc_pkt, NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0) {
			break;
		}
		if (!got_frame) {
			ret = 0;
			break;
		}
		printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", enc_pkt.size);
		ret = av_write_frame(fmt_ctx, &enc_pkt);
		if (ret < 0) {
			break;
		}
	}
	return ret;
}
//目前这个方法失败了，还不知道为什么，基础还不够
int yuv_to_h264() {
	AVFormatContext* pFormatCtx;
	AVOutputFormat* fmt;
	AVStream* video_st;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;
	AVPacket pkt;
	uint8_t* picture_buf;
	AVFrame* pFrame;
	int picture_size;
	int y_size;
	int framecnt = 0;
	
	//input_file
	FILE *in_file = fopen("asset/test.yuv", "rb");
	int in_w = 400, in_h = 226;
	int framenum = 500;
	//output_file
	const char* out_file = "asset/test.h264";

	av_register_all();
	pFormatCtx = avformat_alloc_context();
	fmt = av_guess_format(NULL, out_file, NULL);
	pFormatCtx->oformat = fmt;

	if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0) {
		printf("Failed to open output file! \n");
		return -1;
	}

	video_st = avformat_new_stream(pFormatCtx, 0);

	if (video_st == NULL) {
		return -1;
	}

	pCodecCtx = video_st->codec;
	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
	pCodecCtx->width = in_w;
	pCodecCtx->height = in_h;
	pCodecCtx->bit_rate = 400000;
	pCodecCtx->gop_size = 250;

	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;

	pCodecCtx->qmin = 10;
	pCodecCtx->qmax = 51;
	pCodecCtx->max_b_frames = 3;
	AVDictionary *para = 0;

	if (pCodecCtx->codec_id == AV_CODEC_ID_H264) {
		av_dict_set(&para, "preset", "slow", 0);
		av_dict_set(&para,"tune", "zerolatency", 0);
	}
	if (pCodecCtx->codec_id == AV_CODEC_ID_H265) {
		av_dict_set(&para, "preset", "ultrafast", 0);
		av_dict_set(&para, "tune", "zero-latency", 0);
	}
	
	

	av_dump_format(pFormatCtx, 0, out_file, 1);

	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec) {
		printf("Can not find encoder! \n");
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec, &para) < 0) {
		printf("Failed to open encoder! \n");
		return -1;
	}


	pFrame = av_frame_alloc();
	picture_size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
	picture_buf = (uint8_t *)av_malloc(picture_size);
	avpicture_fill((AVPicture *)pFrame, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
	//Write File Header
	avformat_write_header(pFormatCtx, NULL);
	av_new_packet(&pkt, picture_size);
	y_size = pCodecCtx->width * pCodecCtx->height;

	for (int i = 0; i < framenum; i++) {
		if (fread(picture_buf, 1, y_size * 3 / 2, in_file) <= 0) {
			printf("Failed to read raw data! \n");
			return -1;
		}
		else if(feof(in_file)) {
			break;
		}
		pFrame->data[0] = picture_buf;				     //Y
		pFrame->data[1] = picture_buf + y_size;          // U 
		pFrame->data[2] = picture_buf + y_size * 5 / 4;  // V
		pFrame->pts = i * (video_st->time_base.den) / (video_st->time_base.num * 25);
		int got_picture = 0;
		int ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_picture);
		if (ret < 0) {
			printf("Failed to encode! \n");
			return -1;
		}
		if (got_picture == 1){
			printf("Succeed to encode frame: %5d\tsize:%5d\n", framecnt, pkt.size);
			framecnt++;
			pkt.stream_index = video_st->index;
			ret = av_write_frame(pFormatCtx, &pkt);
			av_free_packet(&pkt);
		}

	}

	int ret = flush_video_encoder(pFormatCtx, 0);

	if (ret < 0) {
		printf("Flushing encoder failed\n");
		return -1;
	}

	av_write_trailer(pFormatCtx);
	if (video_st) {
		avcodec_close(video_st->codec);
		av_free(pFrame);
		av_free(picture_buf);
	}

	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);
	fclose(in_file);
	return 0;
	
}

