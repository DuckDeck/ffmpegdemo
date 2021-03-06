// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <tchar.h>

int standard_play();
int standard_play_su();
int audio_play();
int yuv_to_jpg();
int yuv_to_h264();
int pcm_to_aac();
int capture_jpg(const char* videoPath);
int watermark_play(const char* file);

int yuv420_split(const char* url, int w, int h, int num);
int yuv420_add_Border(const char *url, int w, int h, int border, int num);
int yuv420_graybar(int width, int height, int ymin, int ymax, int barnum, const char *url_output);
int rgb24_to_yuv420p(const char *url_in, int w, int h, int num, const char* url_out);
int rgb24_colorbar(int width, int height, const char *url_out);
int pcm16le_split_left_right(const char *url);
int pcm16le_halfvolume_left_channel(const char *url);
int pcm16le_speedX2(const char *url);
int pcm16le_to_pcm8(const char *url);
int pcm16le_cut_singlechanel(const char* url, int start_num, int dur_num);
int pcm16le_to_wav(const char* pcm_url, int channels, int sample_rate, const char *wav_url);
// TODO: 在此处引用程序需要的其他头文件

int opencvtest();
int testface(const char* imgpath);

int testface(cv::Mat mat);


int learnOpenCV1();
int learnOpenCV4();

int cube();