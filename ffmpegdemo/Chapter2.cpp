#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;


//视频
void playVideo() {
	namedWindow("Video", WINDOW_AUTOSIZE);
	VideoCapture cap;
	cap.open("asset/test.mp4");
	Mat frame;
	for (;;) {
		cap >> frame;
		if (frame.empty()) break;
		imshow("Video", frame);
		if (waitKey(33) >= 0)break;
	}
}



int g_slider_position = 0;
int g_run = 1, g_dontset = 0;
VideoCapture g_cap;

void onTrackbarSilde(int pos, void *) {
	g_cap.set(CAP_PROP_POS_FRAMES, pos);
	if (!g_dontset) {
		g_run = 1;
	}
	g_dontset = 0;
}

void playVideoWithDrag() {
	namedWindow("Video", WINDOW_AUTOSIZE);
	
	g_cap.open("asset/face.mp4");
	int frames = g_cap.get(CAP_PROP_FRAME_COUNT);
	int tmpw = g_cap.get(CAP_PROP_FRAME_WIDTH);
	int tmph = g_cap.get(CAP_PROP_FRAME_HEIGHT);
	cout << "Video has " << frames << "frames of dimensions (" << tmpw << "," << tmph << ")." << endl;

	createTrackbar("Position", "Video", &g_slider_position, frames, onTrackbarSilde);


	Mat frame;
	for (;;) {
		if (g_run != 0) {
			g_cap >> frame;
			if (frame.empty()) {
				break;
			}
			int current_pos = g_cap.get(CAP_PROP_POS_FRAMES);  //获取到当前frame
			g_dontset = 1;
			setTrackbarPos("Position", "Video", current_pos);
			imshow("Video", frame);
			g_run -= 1;
		}
		char c = waitKey(10);
		if (c == 's') {
			g_run = 1;
			cout << "Single step ,run = " << g_run << endl;
		}
		if (c == 'r') {
			g_run = -1;
			cout << "Run mode ,run = " << g_run << endl;
		}
		if (c == 27) {
			break;
		}
		
	}

}

void openCamera() {
	namedWindow("Video", WINDOW_AUTOSIZE);
	VideoCapture cap;
	
	cap.open(-1);
	//死活打不开摄像头
	if (!cap.isOpened()) {
		std::cerr << "Could not open capture" << endl;
	}
}

int learnOpenCV1()
{
	

	//playVideo();
	//playVideoWithDrag();
	openCamera();
	waitKey(0);
	return 0;
}

