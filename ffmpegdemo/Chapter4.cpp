#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//如果要做到可以删字符，不能用这种开发方式，要用保存字符串的方式
Mat current;
void practise1() {
	current = Mat::zeros(500, 500, CV_8UC3);
	imshow("dstImage", current);
	int left = 10;
	int top = 20;
	 while (true)
	 {
		 int s = waitKey(0);
		 if (s == 13) {
			 top += 25;
			 left = 10;
			 continue;
		 }
		 if (s == 8) {
			 if (left <= 10 && top <= 20){
				 continue;
			 }
			 else if (left <= 10 && top >= 20) {
				 left = 475;
				 top = top - 25;
			 }
			
			 continue;
		 }
		 const char c = (char)s;
		 cout << c << endl;
		 putText(current, string(1,c), Point(left, top), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
		 imshow("dstImage", current);
		 left += 15;
		 if (left >= 485) {
			 top += 25;
			 left = 10;
		 }
		 if (left >= 485 && top >= 490) {
			 break;
		 }
	 }
}

string content ;

//这种方法对于要换行很不友好，还是要用string数组
void practice1() {
	
	int numOfLines = 32;

	int top = 20;
	while (true)
	{
		current = Mat::zeros(500, 500, CV_8UC3);
		int s = waitKey(0);
		if (s == 13) { //用这种方便这下麻烦了,看来最好弄一个string数据
			top += 25;
			
			continue;
		}
		
		const char c = (char)s;
		content += c;
		//根据有多少内容画多少行
		int length = content.length();
		string tmp = content.substr(0, numOfLines);
		int i = 1;
		while (tmp.length() == numOfLines)
		{
			
			putText(current, tmp, Point(5, top), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
			imshow("dstImage", current);
			
			tmp = content.substr(i * numOfLines, numOfLines);
			i += 1;
			top = top +  25;
		}
		putText(current, tmp, Point(5, top), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
		imshow("dstImage", current);
		
		if (top >= 490) {
			break;
		}
		top = 20; //要复位一下
	}
	
}



int learnOpenCV4()
{
	practice1();
	
	waitKey(0);
	return 0;
}

