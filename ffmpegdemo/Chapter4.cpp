#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void practise1() {
	namedWindow("no1", WINDOW_AUTOSIZE);
	Mat m = Mat(500, 500, CV_8UC3, Scalar(0, 0, 0));
	imshow("no1", m);
}


int learnOpenCV4()
{
	practise1();
	waitKey(0);
	return 0;
}

