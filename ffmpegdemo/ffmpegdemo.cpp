// ffmpegdemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int main()
{
	//standard_play();
	//standard_play_su();
	//audio_play();
	//yuv_to_jpg();
	//yuv_to_h264();
	

	yuv420_split("asset/lena_256x256_yuv420p.yuv", 256, 256, 1);

	return 0;
}

