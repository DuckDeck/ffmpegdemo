#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;


//锐度算法
Mat acuity(Mat m) {
	Mat res = Mat::zeros(m.size(), m.type());

	/*int rows = m.rows;
	int channels = m.channels();
	int cols = (m.cols - 1) * channels;

	for (int i = 1; i < rows -1; i++)	{
		const uchar* previous = m.ptr<uchar>(i - 1);
		const uchar* current = m.ptr<uchar>(i);
		const uchar* next = m.ptr<uchar>(i + 1);
		uchar* output = res.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			output[j] = saturate_cast<uchar>(5 * current[j]
				- (current[j - channels] + current[j + channels] + previous[j] + next[j]));
		}
	}
	//上面是一个锐度算法，看起来不难
	*/
	Mat kernal = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	filter2D(m, res, m.depth(), kernal);
	return res;
}

//图像混合
Mat blend(Mat m1, Mat m2) {
	double alpha = 0.5;
	if (m1.rows == m2.rows && m2.cols == m2.cols && m1.type() == m2.type()) {
		Mat m3;
		addWeighted(m1, alpha, m2, 1.0 - alpha, 0.0, m3); //权重相加
		//add(m1, m2, m3, Mat());							//简单相加？
		//multiply(m1, m2, m3);								//相乘
		return m3;
	}
	else {
		cout << "The Image is not the same type!!!" << endl;
		return m1;
	}
}

//亮度
Mat bright(Mat m1,float alpha,float beta) {
	Mat m2 = Mat::zeros(m1.size(), m1.type());
	int width = m1.cols;
	int height = m1.rows;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m1.channels() == 1) {
				float v = m1.at<uchar>(i, j);
				m2.at<uchar>(i, j) = saturate_cast<uchar>(v*alpha + beta);
			}
			else if (m1.channels() == 3) {
				float b = m1.at<Vec3b>(i, j)[0];
				float g = m1.at<Vec3b>(i, j)[1];
				float r = m1.at<Vec3b>(i, j)[2];

				m2.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(b*alpha + beta);
				m2.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(g*alpha + beta);
				m2.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(r*alpha + beta);
			}
		}
	}
	return m2;
}

Mat shape(Mat m1) {
	line(m1, Point(20, 30), Point(300, 300), Scalar(0, 0, 255),3,LINE_8);
	rectangle(m1, Rect(200, 100, 300, 400), Scalar(0, 255, 0), 5, LINE_8);
	return m1;
}

int opencvtest()
{
	Mat m1 = imread("asset/001.jpg");
	Mat m2 = imread("asset/002.jpg");
	if (m1.empty())
	{
		cout << "fail to load image 1  !" << endl;
		return -1;
	}
	
	if (m2.empty())
	{
		cout << "fail to load image 2 !" << endl;
		return -1;
	}

	namedWindow("origin", CV_WINDOW_AUTOSIZE);
	imshow("origin", m1);
	
	namedWindow("opencv test", CV_WINDOW_AUTOSIZE);
	imshow("opencv test", m2);

	Mat m3 = shape(m1);

	namedWindow("brightness", CV_WINDOW_AUTOSIZE);
	imshow("brightness", m3);
	
	waitKey(0);
	return 0;
}

