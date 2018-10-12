#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int opencvtest()
{
	Mat originalImage = imread("asset/test.jpg");
	if (originalImage.empty())
	{
		cout << "fail to load image !" << endl;
		return -1;
	}
	namedWindow("opencv test", CV_WINDOW_AUTOSIZE);
	imshow("opencv test", originalImage);
	waitKey(0);
	return 0;
}
