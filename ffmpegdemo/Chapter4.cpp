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

list<string> contents;

void practice1better() {
	int numOfLines = 24;
	int top = 20;
	while (true)
	{
		current = Mat::zeros(500, 500, CV_8UC3);
		int s = waitKeyEx(0);
		if (s == 13) { //回车
			contents.push_back("");
		
			continue;
		}
		if (s == 8) {
			if (contents.empty()) {
				continue;
			}
			if (contents.back().length() == 1) {
				contents.pop_back();
			}
			else {
				contents.back() = 	contents.back().substr(0, contents.back().length() - 1);
			}
			
		}
		else if (s == 249386) {

		}
		else if (s == 2424832) {

		}
		else if (s == 2621440) {

		}
		else if (s == 2555904) {

		}
		else {
			const char c = (char)s;
			cout << s << endl;
			if (contents.empty()) {
				contents.push_back("");
			}
			string last = contents.back();
			if (last.length() == numOfLines) {
				contents.push_back("");
			}
			/*last = contents.back();
			last += c;*/
			contents.back() += c;
		}


		list<string>::iterator itor;
		itor = contents.begin();
		int index = 0;
		while (itor != contents.end())
		{
			//cout << *itor++ << endl;
			string value = *itor++;
			putText(current,value, Point(5, top), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
			if (index == contents.size() - 1) {
				int left = 5 + 20 * value.length();
				rectangle(current, Rect(left - 20, top - 20, 20, 23), Scalar(0, 255, 0), 1);
			}
			imshow("dstImage", current);
			top = top + 25;
			index += 1;
		}
		
		if (top >= 500) {
			break;
		}
		top = 20; //要复位一下
	}
}

int learnOpenCV4()
{
	practice1better();
	
	waitKey(0);
	return 0;
}

