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
Rect frame;
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
		else if (s == 2490368) {
			if (frame.y >= 25) {  
				frame.y -= 25;
			}
		}
		else if (s == 2424832) {
			if (frame.x >= 25) {
				frame.x -= 20;
			}
		}
		//下面的判断方式其实并不好，先这样吧，还有两个小需求没弄好
		else if (s == 2621440) {
			if (frame.y + 60 <= contents.size() * 25 ) {  //如果不是最下面一行，随便跳
				
				frame.y += 25;
			}
			else if (frame.y + 40 <= contents.size() * 25) { //如果是最下面一行，要判断有没有超过最下面的边界

				if (frame.x < 5 + 20 * contents.back().length()) {
					frame.y += 25;
				}
			}

			
		}
		else if (s == 2555904) {
			
			if (frame.y + 40 <= contents.size() * 25) {// 如果是最上面的
				if (frame.x + 20 < 480) {
					frame.x += 20;
				}
			}
			else {
				if (frame.x + 20 < 5 + 20 * contents.back().length()) { //最下面的
					frame.x += 20;
				}
			}
		}
		if((s >= 48 && s <= 57) || s == -1 ) {
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
				if (s != 2490368 && s != 2424832 && s != 2621440 && s != 2555904) {
					int left = 5 + 20 * value.length();
					frame = Rect(left - 20, top - 20, 20, 23);
				}
				rectangle(current, frame, Scalar(0, 255, 0), 1);
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

