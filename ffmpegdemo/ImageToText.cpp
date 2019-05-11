#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;


//视频


//int g_slider_position = 0;
//int g_run = 1, g_dontset = 0;
//VideoCapture g_cap;

//void onTrackbarSilde(int pos, void *) {
//	g_cap.set(CAP_PROP_POS_FRAMES, pos);
//	if (!g_dontset) {
//		g_run = 1;
//	}
//	g_dontset = 0;
//}

//void playVideoWithDrag() {
//	namedWindow("Video", WINDOW_AUTOSIZE);
//
//	g_cap.open("asset/face.mp4");
//	int frames = g_cap.get(CAP_PROP_FRAME_COUNT);
//	int tmpw = g_cap.get(CAP_PROP_FRAME_WIDTH);
//	int tmph = g_cap.get(CAP_PROP_FRAME_HEIGHT);
//	cout << "Video has " << frames << "frames of dimensions (" << tmpw << "," << tmph << ")." << endl;
//
//	createTrackbar("Position", "Video", &g_slider_position, frames, onTrackbarSilde);
//
//
//	Mat frame;
//	for (;;) {
//		if (g_run != 0) {
//			g_cap >> frame;
//			if (frame.empty()) {
//				break;
//			}
//			int current_pos = g_cap.get(CAP_PROP_POS_FRAMES);
//			g_dontset = 1;
//			setTrackbarPos("Position", "Video", current_pos);
//
//
//			imshow("Video", frame);
//			g_run -= 1;
//		}
//		char c = waitKey(10);
//		if (c == 's') {
//			g_run = 1;
//			cout << "Single step ,run = " << g_run << endl;
//		}
//		if (c == 'r') {
//			g_run = -1;
//			cout << "Run mode ,run = " << g_run << endl;
//		}
//		if (c == 27) {
//			break;
//		}
//
//	}
//
//}

void ImageToText(const char* path) {
	Mat m = imread(path);

}

int toText(const char* path)
{


	
	// 目前还没有找到GUI显示文字的办法
	//playVideoWithDrag();
	waitKey(0);
	return 0;
}

