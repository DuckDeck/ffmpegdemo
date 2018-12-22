#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int element_size = 3;
Mat m, res,tmp,dst;
Mat m1,m2,m3,m4,m5;

Mat hsv, hue;
int threshold_value = 12;
int threshold_max = 255;
int type_value = 2;
int type_max = 4;
int index = 0;
int bins = 12;
int match_method = CV_TM_SQDIFF;
int max_track = 5;
Scalar randomColor() {
	RNG rng = RNG(123456789);
	printf("获取的色%i", rng.uniform(0, 255));
	return  Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
}
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

void threshold(int, void*) {
	threshold(m, res, threshold_value, threshold_max, type_value);
	imshow("threshold", res);
}

void threshold_oper(Mat m1) {
	m = m1.clone();
	cvtColor(m, m, CV_BGR2GRAY);
	namedWindow("threshold", CV_WINDOW_AUTOSIZE);
	createTrackbar("threshold value", "threshold", &threshold_value, threshold_max, threshold);
	createTrackbar("threshold type", "threshold", &type_value, type_max, threshold);
	threshold(0, 0);
}

//Sobel算子
void sobel(Mat m1) {
	Mat tmp;
	GaussianBlur(m1, tmp, Size(3, 3), 0);
	cvtColor(tmp, tmp, CV_BGR2GRAY);
	Mat xrad, yrad,xyrad;
	Sobel(tmp, xrad, CV_16S, 1, 0, 3);
	Sobel(tmp, yrad, CV_16S, 0, 1, 3);
	//Scharr(tmp, xrad, CV_16S, 1, 0, 3);  //这个API效果太强了
	//Scharr(tmp, yrad, CV_16S, 0, 1, 3);
	convertScaleAbs(xrad, xrad);
	convertScaleAbs(yrad, yrad);
	imshow("Xrad", xrad);
	imshow("Yrad", yrad);

	/*addWeighted(xrad, 0.5, yrad, 0.5, 0, xyrad);
	imshow("XYrad", xyrad);*/

	/*Sobel(tmp, xyrad, CV_16S, 1, 1);
	convertScaleAbs(xyrad, xyrad);
	imshow("XYrad", xyrad);*/


	xyrad = Mat(xrad.size(), xrad.type());
	for (int i = 0; i < xrad.rows; i++)
	{
		for (int j = 0; j < xrad.cols; j++)
		{	
			int xg = xrad.at<uchar>(i, j);
			int yg = yrad.at<uchar>(i, j);
			int xy = saturate_cast<uchar>(xg + yg);
			xyrad.at<uchar>(i, j) = xy;
		}
	}

	imshow("XYrad", xyrad);
}

//Laplance算子
void laplance(Mat m1) {
	Mat tmp, res;
	GaussianBlur(m1, tmp, Size(3, 3), 0);
	cvtColor(tmp, tmp, CV_BGR2GRAY);
	Laplacian(tmp, res, CV_16S, 3);
	convertScaleAbs(res, res);
	imshow("Laplance", res);
	threshold(res, res, 0, 255, THRESH_OTSU | THRESH_BINARY);//因为用laplance算子边缘不够明显，所以用了threshold加强一下
	imshow("Laplance with threshold", res);
}


//Canny边缘检测
void cannyTest(int, void*) {
	Mat res, dst;
	
	blur(m, m, Size(3, 3));
	Canny(m, res, threshold_value, threshold_value * 2, 3, false);
	imshow("cannyTest",~res);
	dst.create(m.size(), m.type());
	Mat mask = Mat::zeros(m.size(), m.type());
	m2.copyTo(dst, ~res);
	imshow("another cannyTest", dst);
}


void houghLine(Mat mx) {
	Mat tmp,dst;
	Canny(mx, tmp, 100, 200);
	imshow("canny", ~tmp);
	cvtColor(tmp, dst, CV_GRAY2BGR);
	vector<Vec4f> lines;
	HoughLinesP(tmp, lines, 1, CV_PI / 180.0, 10, 0, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4f l = lines[i];
		line(dst, Point(l[0], l[1]), Point(l[2], l[3]), randomColor(), 3, LINE_AA);
	}
	imshow("Lines", dst);
}


void houghRound(Mat mx) {
	Mat tmp,dst;
	medianBlur(mx, tmp, 3);
	cvtColor(tmp, tmp, CV_BGR2GRAY);
	vector<Vec3f> points;
	HoughCircles(tmp, points, CV_HOUGH_GRADIENT, 1, 10, 100, 30, 5, 50);
	m3.copyTo(dst);
	for (size_t i = 0; i < points.size(); i++)
	{
		Vec3f cc = points[i];
		circle(dst, Point(cc[0], cc[1]), cc[2], randomColor(), 2, LINE_AA);
		circle(dst, Point(cc[0], cc[1]), 2, randomColor(), 2, LINE_AA);
	}
	imshow("round detect", dst);
}


//像素重映射
void pixelReMap(Mat m1) {
	Mat mat_x, mat_y,dst;
	mat_x.create(m1.size(), CV_32FC1);
	mat_y.create(m1.size(), CV_32FC1);
	for (int i = 0; i < m1.rows; i++)
	{
		for (int j = 0; j < m1.cols; j++)
		{
			switch (index)
			{
			case 0:
				if (j > m1.cols * 0.25 && j < m1.cols * 0.75 && i > m1.rows * 0.25 && i < m1.rows * 0.75)
				{
					mat_x.at<float>(i, j) = 2 * (j - m1.cols * 0.25);
					mat_y.at<float>(i, j) = 2 * (i - m1.rows * 0.25);
					//mat_x.at<float>(i, j) = (j - m1.cols * 0.5);
					//mat_y.at<float>(i, j) = (i - m1.rows * 0.5);
				}
				else {
					mat_x.at<float>(i, j) = 0;
					mat_y.at<float>(i, j) = 0;
				}
				break;
			case 1:
				mat_x.at<float>(i, j) = m1.cols - j - 1;
				mat_y.at<float>(i, j) = i;
				break;
			case 2:
				mat_x.at<float>(i, j) = j;
				mat_y.at<float>(i, j) = m1.rows - i - 1;
				break;
			case 3:
				mat_x.at<float>(i, j) = m1.cols - j - 1;
				mat_y.at<float>(i, j) = m1.rows - i - 1;
				break;
			default:
				break;
			}
		}
	}
	remap(m1, dst, mat_x, mat_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 255, 255));
	imshow("Remap Image", dst);
	//imshow("mat_x", mat_x); 这个是空白
}

void changeReMap() {
	int c = 0;
	while (true)
	{
		c = waitKey(500);
		printf("%i",c);
		if (c == -1) {
			continue;
		}
		index = c % 4;
		if ((char)c == 27)
		{
			break;
		}
		pixelReMap(m1);
	}
}



//直方图均衡化 可以加强对比度
void equaliz(Mat m1)
{
	Mat tmp,dst;
	cvtColor(m1, tmp, CV_BGR2GRAY);
	imshow("灰阶图", tmp);
	equalizeHist(tmp, dst);
	imshow("直方图均衡化", dst);
}

//显示直方图
void showCalcHist(Mat m1) {
	//获取三个通道
	vector<Mat> bgr_channels;
	split(m1, bgr_channels);
	//imshow("B channel", bgr_channels[0]);
	//imshow("G channel", bgr_channels[1]);
	//imshow("R channel", bgr_channels[2]);

	//获取直方图
	int histSize = 256;
	float range[] = { 0,256 };
	const float *histRagnes = { range };
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_channels[0], 1, 0, Mat(), b_hist,1, &histSize, &histRagnes, true, false);
	calcHist(&bgr_channels[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRagnes, true, false);
	calcHist(&bgr_channels[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRagnes, true, false);


	//绘制直方图
	int hist_h = 400, hist_w = 512;
	int bin_w = hist_w / histSize;
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1); //转换分辨率
	normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1);
	normalize(r_hist, r_hist, 0, hist_h, NORM_MINMAX, -1);

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(i * bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, LINE_AA);
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(i * bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, LINE_AA);
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(i * bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, LINE_AA);
	}
	imshow("输出直方图", histImage);

}


void showCalcImage() {
	int hist_h = 400, hist_w = 512;
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	//normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1); //转换分辨率
	//normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1);
	//normalize(r_hist, r_hist, 0, hist_h, NORM_MINMAX, -1);
}

void HistAndback(int, void*) {
	float range[] = { 0,180 };
	const float *histRanges = { range };
	Mat h_hist;
	calcHist(&hue, 1, 0, Mat(), h_hist, 1, &bins, &histRanges, true, false);
	normalize(h_hist, h_hist, 0, 255, NORM_MINMAX, -1, Mat());
	Mat backProjectImage;
	calcBackProject(&hue, 1, 0, h_hist, backProjectImage, &histRanges, 1, true);
	imshow("BackProjectImage", backProjectImage);


	int hist_h = 400, hist_w = 400;
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	int bin_w = hist_w / bins;
	for (int i = 1; i < bins; i++)
	{
		rectangle(histImage, Point((i - 1)*bin_w, cvRound(hist_w - h_hist.at<float>(i - 1) * 400 / 255))
			, Point(i*bin_w, hist_h), Scalar(0, 0, 255));
	}

	imshow("Histogram", histImage);

}
//直方图反射投影
void calcmap(Mat m1) {
	cvtColor(m1, hsv, CV_BGR2HSV);
	hue.create(hsv.size(), hsv.depth());
	int nchannels[] = { 0, 0 };
	mixChannels(&hsv, 1, &hue, 1, nchannels, 1);
	namedWindow("直方图反射投影", CV_WINDOW_AUTOSIZE);
	createTrackbar("Histogram Bins", "直方图反射投影", &bins, 180, HistAndback);
	imshow("直方图反射投影", m1);
	HistAndback(0,0);
}

//模板匹配
void matchOper(int, void*) {
	int width = m.cols - tmp.cols + 1;
	int height = m.rows - tmp.rows + 1;
	Mat result(width, height, CV_32FC1);
	matchTemplate(m, tmp, result, match_method, Mat());
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	Point minLoc, maxLoc;
	double min, max;
	m.copyTo(dst);
	Point temloc;
	minMaxLoc(result, &min, &max, &minLoc, &maxLoc, Mat());
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
		temloc = minLoc;
	}
	else {
		temloc = maxLoc;
	}

	//绘制矩形
	rectangle(dst, Rect(temloc.x, temloc.y, tmp.cols, tmp.rows), Scalar(0, 0, 255), 2, 8);
	rectangle(result, Rect(temloc.x, temloc.y, tmp.cols, tmp.rows), Scalar(0, 0, 255), 2, 8);
	imshow("模板匹配", result);
	imshow("Match", dst);
	imshow("Match Image", tmp);
}

void templaTematch() {
	namedWindow("模板匹配", CV_WINDOW_AUTOSIZE);
	createTrackbar("Match Algo Type", "模板匹配", &match_method, max_track, matchOper);
	matchOper(0, 0);
}

void findBorderCallback(int ,void*) {
	
	Mat canny_output;
	vector<vector<Point>> points;
	vector<Vec4i> hierachy;
	Canny(m, canny_output, threshold_value, threshold_value * 2, 3, false);
	findContours(canny_output, points, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	dst = Mat::zeros(m.size(), CV_8UC3);

	printf("找出%i条轮廓", points.size());

	for (size_t i = 0; i < points.size(); i++)
	{
		drawContours(dst, points, i, randomColor(), 2, 8, hierachy, 0, Point(0, 0));
	}
	imshow("轮廓发现", dst);
}



//轮廓发现
void findBorder() {
	threshold_value = 200;
	cvtColor(m, m, CV_BGR2GRAY);
	namedWindow("轮廓发现", CV_WINDOW_AUTOSIZE);
	createTrackbar("Threshold Value", "轮廓发现", &threshold_value, threshold_max, findBorderCallback);
	findBorderCallback(0, 0);
}


void topBagCallback(int,void*) {
	vector<vector<Point>> points;
	vector<Vec4i> hierachy;
	threshold(m, tmp, threshold_value, threshold_max, THRESH_BINARY);
	findContours(tmp, points, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point>> convexhulls(points.size());
	for (size_t i = 0; i < points.size(); i++)
	{
		convexHull(points[i], convexhulls[i], false, true);
		
	}

	dst = Mat::zeros(tmp.size(), CV_8UC3);
	for (size_t i = 0; i < points.size(); i++)
	{
		drawContours(dst, convexhulls, i, randomColor(), 2, 8, hierachy, 0, Point(0, 0));
		drawContours(dst, points, i, randomColor(), 2, 8, hierachy, 0, Point(0, 0));
	}
	imshow("凸包", dst);

}

//凸包
void topBag() {
	threshold_value = 200;
	cvtColor(m, m, CV_BGR2GRAY);
	blur(m, m, Size(3, 3), Point(-1, -1), BORDER_DEFAULT);
	namedWindow("凸包", CV_WINDOW_AUTOSIZE);
	createTrackbar("Threshold Value", "凸包", &threshold_value, threshold_max, topBagCallback);
	topBagCallback(0, 0);
}

int opencvtest()
{
	 m1 = imread("asset/001.jpg");
	 m2 = imread("asset/002.jpg");
	 m3 = imread("asset/004.png");
	 m4 = imread("asset/010.jpg");
	 m5 = imread("asset/timg.jpg");
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
	imshow("origin", m2);
	/*namedWindow("opencv test", CV_WINDOW_AUTOSIZE);
	imshow("opencv test", m2);*/


	//m = m1.clone();
	//namedWindow("pattern", CV_WINDOW_AUTOSIZE);
	//createTrackbar("Element size:", "pattern", &element_size, 21, pattern);
	//pattern(0, 0);
	
	//Mat mx = gussDiff(m1);
	//imshow("sample", mx);
	
	//threshold_oper(m2);

	//sobel(m1);

	//laplance(m3);

	
	/*
	m = m2.clone();
	cvtColor(m, m, CV_BGR2GRAY);
	namedWindow("cannyTest", CV_WINDOW_AUTOSIZE);
	createTrackbar("Canny value", "cannyTest", &threshold_value, threshold_max, cannyTest);
	cannyTest(0, 0);
	*/

	//houghLine(m3);

	//houghRound(m3);
	
	//changeReMap();
	
	//equaliz(m1);

	//showCalcHist(m2);


	//calcmap(m2);

	//m4.copyTo(m);
	//tmp = imread("asset/match2.jpg");
	//templaTematch();
	m3.copyTo(m);
	//findBorder();
	
	//topBag();

	waitKey(0);
	return 0;
}

