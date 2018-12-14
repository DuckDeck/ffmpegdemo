#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int opencvtest()
{
	Mat originalImage = imread("asset/001.jpg");
	if (originalImage.empty())
	{
		cout << "fail to load image !" << endl;
		return -1;
	}
	
	namedWindow("origin", CV_WINDOW_AUTOSIZE);
	imshow("origin", originalImage);
	Mat dst = Mat::zeros(originalImage.size(), originalImage.type());
	
	/*int rows = originalImage.rows;
	int channels = originalImage.channels();
	int cols = (originalImage.cols - 1) * channels;
	
	for (int i = 1; i < rows -1; i++)	{
		const uchar* previous = originalImage.ptr<uchar>(i - 1);
		const uchar* current = originalImage.ptr<uchar>(i);
		const uchar* next = originalImage.ptr<uchar>(i + 1);
		uchar* output = dst.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			output[j] = saturate_cast<uchar>(5 * current[j]
				- (current[j - channels] + current[j + channels] + previous[j] + next[j]));
			
			
		}
	}
	*/
	Mat kernal = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	filter2D(originalImage, dst, originalImage.depth(), kernal);
	//上面是一个锐度算法，看起来不难
	namedWindow("opencv test", CV_WINDOW_AUTOSIZE);
	imshow("opencv test", dst);
	waitKey(0);
	return 0;
}
