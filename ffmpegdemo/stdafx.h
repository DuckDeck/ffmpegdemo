// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

int standard_play();
int standard_play_su();
int audio_play();
int yuv_to_jpg();
int yuv_to_h264();


int yuv420_split(const char* url, int w, int h, int num);
// TODO: 在此处引用程序需要的其他头文件
