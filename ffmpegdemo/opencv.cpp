#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int element_size = 3;
Mat m;
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
	line(m1, Point(20, 30), Point(30, 400), Scalar(0, 0, 255),3,LINE_AA);//LINE_8 线不是直的
	rectangle(m1, Rect(200, 100, 300, 400), Scalar(0, 255, 0), 5, LINE_8);
	ellipse(m1, Point(m1.cols / 2, m1.rows / 2), Size(300, 200), 45, 90, 270, Scalar(255, 0, 0), 5, LINE_8);
	ellipse(m1, Point(m1.cols / 2 + 10, m1.rows / 2 + 10), Size(300, 200), 45, 0, 90, Scalar(255, 0, 0), 5, LINE_8);
	ellipse(m1, Point(m1.cols / 2 + 10, m1.rows / 2 + 10), Size(300, 200), 45, 270, 360, Scalar(255, 0, 0), 5, LINE_8);
	circle(m1, Point(m1.cols / 2, m1.rows / 2), 100, Scalar(255, 155, 0), 5, LINE_8);
	
	putText(m1, "Shadow_Edge", Point(400, 400), CV_FONT_HERSHEY_COMPLEX, 3.0, Scalar(255, 0, 255), 4, 8);
	Point pt[1][6];
	pt[0][0] = Point(100, 100);
	pt[0][1] = Point(100, 200);
	pt[0][2] = Point(200, 300);
	pt[0][3] = Point(300, 400);
	pt[0][4] = Point(400, 200);
	pt[0][5] = Point(200, 100);
	const Point* ppts[] = { pt[0] };
	int npt[] = { 6 };
	fillPoly(m1, ppts, npt, 1, Scalar(0, 255, 255), 8);

	//随机加一条线
	RNG rng(12345);
	Point p1;
	Point p2;
	p1.x = rng.uniform(0, m1.cols);
	p2.x = rng.uniform(0, m1.cols);
	p1.y = rng.uniform(0, m1.rows);
	p2.y = rng.uniform(0, m1.rows);
	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	line(m1, p1, p2, color, 3, LINE_AA);

	return m1;
}

//随机生成线
Mat randomLine() {
	Mat m1 = Mat(500, 500, 16);
	RNG rng(12345);
	Point p1;
	Point p2;
	for (int i = 0; i < 10; i++)
	{
		
		p1.x = rng.uniform(0, m1.cols);
		p2.x = rng.uniform(0, m1.cols);
		p1.y = rng.uniform(0, m1.rows);
		p2.y = rng.uniform(0, m1.rows);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		if (waitKey(50)>0) {
			break;
		}
		line(m1, p1, p2, color, 3, LINE_AA);
	}
	return m1;
}

//模糊
Mat blur(Mat m1) {
	Mat m2 = Mat::zeros(m1.size(), m1.type());

	//blur(m1, m2, Size(10, 10), Point(-1, -1));
	//GaussianBlur(m1, m2, Size(7, 7), 11, 11); //只能用奇数
	//medianBlur(m1, m2, 7);					//中值
	bilateralFilter(m1, m2, 13, 50, 3);		//双边
	return m2;
}


//形态操作
void pattern(int,void*){
	int s = element_size * 2 + 1;
	Mat m2;
	Mat structureElement = getStructuringElement(MORPH_RECT, Size(s, s), Point(-1, -1));
	//dilate(m, m2, structureElement, Point(-1, -1)); //膨胀
	//erode(m, m2, structureElement); //腐蚀
	//开操作 ，先腐蚀后膨胀，去掉小的对象
	//morphologyEx(m, m2, CV_MOP_OPEN, structureElement);
	//闭操作 ，先膨胀后腐蚀，
	//morphologyEx(m, m2, CV_MOP_CLOSE, structureElement);
	//梯度
	//morphologyEx(m, m2, CV_MOP_GRADIENT, structureElement);
	//morphologyEx(m, m2, CV_MOP_TOPHAT, structureElement);
	morphologyEx(m, m2, CV_MOP_BLACKHAT, structureElement);
	imshow("pattern", m2);
	return ;
}


//从图片里面提取信息
Mat  getInfo(Mat m1) {
	Mat mGray;
	//变成灰度图像
	cvtColor(m1, mGray, CV_BGRA2GRAY);
	imshow("Gray", mGray);

	Mat binImg;
	adaptiveThreshold(~mGray, binImg, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, -1);//这里要取反？？？
	imshow("Bin", binImg);

	Mat hline = getStructuringElement(MORPH_RECT, Size(m1.cols / 16, 1));
	//水平结构元素，垂直上的值比较大，所以垂直上的东西都被腐蚀掉了
	Mat vline = getStructuringElement(MORPH_RECT, Size(1, m1.rows / 16));
	//垂直结构元素
	Mat text = getStructuringElement(MORPH_RECT, Size(7, 7)); 
	//提取文字，之所以会取出文字是因为文字比较粗，其他元素比较细
	Mat dst, tmp;
	erode(binImg, tmp, hline);
	imshow("temp image", tmp);
	dilate(tmp, dst, hline);
	//morphologyEx(binImg, dst, CV_MOP_OPEN, text);

	blur(dst, dst, Size(2, 2));

	return dst;
}


//上下采样
Mat sample(Mat m1) {
	Mat dst;
	//pyrUp(m1, dst, Size(m1.cols * 2, m1.rows * 2)); //图片变大2倍
	pyrDown(m1, dst, Size(m1.cols / 2, m1.rows / 2)); //图片变小2倍
	return dst;
}


//高斯不同 目前还不知道有什么用
Mat gussDiff(Mat m1) {
	Mat tmp,g1, g2, dst;
	cvtColor(m1, tmp, CV_BGR2GRAY);
	GaussianBlur(tmp, g1, Size(3, 3), 0, 0);
	GaussianBlur(g1, g2, Size(3, 3), 0, 0);
	subtract(g1, g2, dst, Mat());
	normalize(dst, dst, 255, 0, NORM_MINMAX);
	return dst;
}
int opencvtest()
{
	Mat m1 = imread("asset/001.jpg");
	Mat m2 = imread("asset/002.jpg");
	Mat m3 = imread("asset/004.png");
	if (m1.empty()){
		cout << "fail to load image 1  !" << endl;
		return -1;
	}
	if (m2.empty()){
		cout << "fail to load image 2 !" << endl;
		return -1;
	}
	if (m3.empty()) {
		cout << "fail to load image 3 !" << endl;
		return -1;
	}
	namedWindow("origin", CV_WINDOW_AUTOSIZE);
	imshow("origin", m3);
	/*namedWindow("opencv test", CV_WINDOW_AUTOSIZE);
	imshow("opencv test", m2);*/


	//m = m1.clone();
	//namedWindow("pattern", CV_WINDOW_AUTOSIZE);
	//createTrackbar("Element size:", "pattern", &element_size, 21, pattern);
	//pattern(0, 0);
	
	Mat mx = gussDiff(m1);
	imshow("sample", mx);

	

	waitKey(0);
	return 0;
}

