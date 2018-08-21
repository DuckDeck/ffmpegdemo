#include "stdafx.h"

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#ifdef __cplusplus
};
#endif
#endif


//test different codec
#define TEST_H264  1
#define TEST_HEVC  0

int pure_play() {
	AVCodec *pCodec;
	AVCodecContext *pCodeCtx = NULL;
	AVCodecParserContext *pCodeParserCtx = NULL;

	return 0;
}
