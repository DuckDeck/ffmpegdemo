#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
using namespace std;

//本程序中的函数可以将YUV420P数据中的Y、U、V三个分量分离开来并保存成三个文件
//如果视频帧的宽和高分别为w和h，那么一帧YUV420P像素数据一共占用w*h*3/2 Byte的数据。其中前w*h Byte存储Y，
//接着的w*h*1/4 Byte存储U，最后w*h*1/4 Byte存储V。上述调用函数的代码运行后，
//将会把一张分辨率为256x256的名称为lena_256x256_yuv420p.
//yuv的YUV420P格式的像素数据文件分离成为三个文件
//output_420_y.y：纯Y数据，分辨率为256x256。
//output_420_u.y：纯U数据，分辨率为128x128。
//output_420_v.y：纯V数据，分辨率为128x128。
int yuv420_split(const char* url,int w,int h,int num){
    FILE *fp = fopen(url,"rb+");
    FILE *fp1 = fopen("asset/output_420_y.y","wb+");
    FILE *fp2 = fopen("asset/output_420_u.y","wb+");
    FILE *fp3 = fopen("asset/output_420_v.y","wb+");
    unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2); //pic 是个指针
    for(int i=0;i<num;i++){
        size_t count = fread(pic, 1, w * h * 3 / 2, fp);
        cout << "count " << count << endl;
		cout << "count " << pic << endl;
        fwrite(pic, 1, w * h, fp1);								//从pic开始处取 w * h的数据量写到文件
        fwrite(pic + w * h, 1, w * h / 4, fp2);					//从pic+w*h开始处取 w * h * 0.25的数据量写到文件
        fwrite(pic + w * h * 5 / 4, 1, w * h / 4, fp3);			//从pic+w*h+0.25*w*h开始处取 w * h * 0.25的数据量写到文件
    }
    //YUV可能是这样保存的，y分量是保存在长*宽里，剩下的各1/4保存剩下的两个分量。所以取出来的时侯先取w * h，取u分量的时侯是取剩下的1/4，但是保存的时侯
    //无从pic指针最开始的地方保存w*H的份量，这也刚好是y的分量。然后保存u分量时，要从pic + w * h的地方开始写，写入量是w * h / 4
    //那最后一个要从pic + w * h + w * h / 4在地方开始写，也就是pic + w * h * 5 / 4了
    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    return 0;
}
//(2)分离YUV444P像素数据中的Y、U、V分量
//从代码可以看出，如果视频帧的宽和高分别为w和h，那么一帧YUV444P像素数据一共占用w*h*3 Byte的数据。其中前w*h Byte存储Y，
//接着的w*h Byte存储U，最后w*h Byte存储V。上述调用函数的代码运行后，将会把一张分辨率为256x256的名称为lena_256x256_yuv444p.
//yuv的YUV444P格式的像素数据文件分离成为三个文件：
int yuv444_split(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("asset/output_444_y.y", "wb+");
	FILE *fp2 = fopen("asset/output_444_u.y", "wb+");
	FILE *fp3 = fopen("asset/output_444_v.y", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3);
	for (int i = 0; i<num; i++) {
		fread(pic, 1, w * h * 3, fp);
		fwrite(pic, 1, w * h, fp1);
		fwrite(pic + w * h, 1, w * h, fp2);
		fwrite(pic + w * h * 2, 1, w * h, fp3);
	}
	//和yuv420p比起来就简单多了刚好一个通道一个分量
	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}
//(3) 将YUV420P像素数据去掉颜色（变成灰度图）
//如果想把YUV格式像素数据变成灰度图像，只需要将U、V分量设置成128即可。这是因为U、V是图像中的经过偏置处理的色度分量。
//色度分量在偏置处理前的取值范围是-128至127，//这时候的无色对应的是“0”值。经过偏置后色度分量取值变成了0至255，
//因而此时的无色对应的就是128了。上述调用函数的代码运行后，//将会把一张分辨率为256x256的名称为lena_256x256_yuv420p.
//yuv的YUV420P格式的像素数据文件处理成名称为output_gray.yuv的YUV420P格式的像素数据文件
int yuv420_gray(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("asset/output_yuv420_gray.yuv", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);
	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, fp);
		memset(pic + w * h, 128, w * h / 2);//从pic + w * h开设置，后面的全是uv分量。而设置和数量是w * h / 2
		fwrite(pic, 1, w * h * 3 / 2, fp1);
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	return 0;
}

//(4)将YUV420P像素数据的亮度减半
//如果打算将图像的亮度减半，只要将图像的每个像素的Y值取出来分别进行除以2的工作就可以了。图像的每个Y值占用1 Byte，取值范围是0至255，
//对应C语言中的unsigned char数据类型。上述调用函数的代码运行后，将会把一张分辨率为256x256的名称为lena_256x256_yuv420p.yuv
//的YUV420P格式的像素数据文件处理成名称为output_half.yuv的YUV420P格式的像素数据文件
int yuv420_harf_luminance(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("asset/output_yuv420_harf_luminance.yuv", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);
	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, fp);
		for (int j = 0; j < w * h; j++)
		{
			unsigned char temp = pic[j] / 2;
			pic[j] = temp;
		}
		fwrite(pic, 1, w * h * 3 / 2, fp1);
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	return 0;
}
//(5)将YUV420P像素数据的周围加上边框
//图像的边框的宽度为border，本程序将距离图像边缘border范围内的像素的亮度分量Y的取值设置成了亮度最大值255。
//上述调用函数的代码运行后，将会把一张分辨率为256x256的名称为lena_256x256_yuv420p.yuv
//的YUV420P格式的像素数据文件处理成名称为output_border.yuv的YUV420P格式的像素数据文件
int yuv420_add_Border(const char *url, int w, int h, int border, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("asset/output_yuv420_add_border.yuv", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);
	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, fp);
		for (int j = 0; j < h; j++) {
			for (int k = 0; k < w; k++) {
				if (k < border || k >(w - border) || j < border || j >(h - border)) {
					pic[j * w + k] = 255; //只能用0或者255。不能用其他的数，因为这不是RGB。因为是操作亮度，所以直接操作第一个w*h就行
				}
			}
		}
		fwrite(pic, 1, w * h * 3 / 2, fp1);
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	return 0;
}

//(6) 生成YUV420P格式的灰阶测试图
//本程序一方面通过灰阶测试图的亮度最小值ymin，亮度最大值ymax，灰阶数量barnum确定每一个灰度条中像素的亮度分量Y的取值。
//另一方面还要根据图像的宽度width和图像的高度height以及灰阶数量barnum确定每一个灰度条的宽度。有了这两方面信息之后，就可以生成相应的图片了。
//上述调用函数的代码运行后，会生成一个取值范围从ymin-ymax，一共包含barnum个灰度条的YUV420P格式的测试图。
//ymax是最左边的亮度，ymin是最右边的亮度
int yuv420_graybar(int width, int height, int ymin, int ymax, int barnum, const char *url_output)
{
	float barwidth; //每条bar的宽度
	float lum_inc;  //每条bar高度的差值
	unsigned char lum_temp;
	int uv_width, uv_height;  //uv分量的高度宽高
	FILE *fp = NULL;
	unsigned char *data_y = NULL;     //y分量数据
	unsigned char *data_u = NULL;     //u分量数据
	unsigned char *data_v = NULL;     //v分量数据
	int t = 0, i = 0, j = 0;

	barwidth = (float)width / (float)barnum;
	lum_inc = (float)(ymax - ymin) / (float)(barnum - 1);
	uv_width = width / 2;
	uv_height = height / 2;

	data_y = (unsigned char *)malloc(width * height);
	data_u = (unsigned char *)malloc(uv_width * uv_height);
	data_v = (unsigned char *)malloc(uv_width * uv_height);
	if ((fp = fopen(url_output, "wb+")) == NULL) {
		printf("Error: Cannot create file!");
		return -1;
	}
	printf("Y, U, V value from picture's left to right:\n");
	for (t = 0; t < width / barwidth; t++)
	{
		lum_temp = ymin + (char)(t * lum_inc);
		printf("%3d,128,128\n", lum_temp);
	}
	for (j = 0; j<height; j++) {
		for (i = 0; i<width; i++) {
			t = i / barwidth;
			lum_temp = ymin + (char)(t * lum_inc);
			data_y[j * width + i] = lum_temp; //填充y分量，数量为w*h
		}
	}
	for (j = 0; j<uv_height; j++) {
		for (i = 0; i<uv_width; i++) {
			data_u[j * uv_width + i] = 128; //填充u分量，数量为uv_width和uv_height
		}
	}
	for (j = 0; j < uv_height; j++) {
		for (i = 0; i < uv_width; i++) {
			data_v[j * uv_width + i] = 128; //填充v分量，数量为uv_width和uv_height
		}
	}
	fwrite(data_y, width * height, 1, fp);
	fwrite(data_u, uv_width * uv_height, 1, fp);
	fwrite(data_v, uv_width * uv_height, 1, fp);
	fclose(fp);
	free(data_y);
	free(data_u);
	free(data_v);
	return 0;
}

//(7)计算两个YUV420P像素数据的PSNR
//对于8bit量化的像素数据来说，PSNR的计算公式如下所示。
//上述公式中mse的计算公式如下所示。其中M，N分别为图像的宽高，xij和yij分别为两张图像的每一个像素值。PSNR通常用于质量评价，
//就是计算受损图像与原始图像之间的差别，以此来评价受损图像的质量。
//程序计算后得到的PSNR取值为26.693。PSNR取值通常情况下都在20-50的范围内，取值越高，代表两张图像越接近，反映出受损图像质量越好
double yuv420_psnr(char *url1, char *url2, int w, int h, int num) {
	FILE *fp1 = fopen(url1, "rb+");
	FILE *fp2 = fopen(url2, "rb+");
	unsigned char *pic1 = (unsigned char *)malloc(w * h);
	unsigned char *pic2 = (unsigned char *)malloc(w * h);
	double p = 0.0;
	for (int i = 0; i < num; i++) {
		fread(pic1, 1, w * h, fp1);
		fread(pic2, 1, w * h, fp2);
		double mse_sum = 0, mse = 0, psnr = 0;
		for (int j = 0; j < w * h; j++) {
			mse_sum += pow((double)(pic1[j] - pic2[j]), 2);
		}
		mse = mse_sum / (w * h);
		psnr = 10 * log10(255.0 * 255.0 / mse);
		printf("%5.3f\n", psnr);
		fseek(fp1, w * h / 2, SEEK_CUR);
		fseek(fp2, w * h / 2, SEEK_CUR);
		p = psnr;
	}
	free(pic1);
	free(pic2);
	fclose(fp1);
	fclose(fp2);
	return p;
}

//(8) 分离RGB24像素数据中的R、G、B分量
int rgb24_split(char* url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("asset/output_r.y", "wb+");
	FILE *fp2 = fopen("asset/output_g.y", "wb+");
	FILE *fp3 = fopen("asset/output_b.y", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3);
	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3, fp);
		for (int j = 0; j < w*h * 3; j += 3) {
			fwrite(pic + j, 1, 1, fp1);
			fwrite(pic + j + 1, 1, 1, fp2);
			fwrite(pic + j + 2, 1, 1, fp3);
		}
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}

//(9)将RGB24格式像素数据转换为YUV420P格式像素数据
unsigned char clip_value(unsigned char x, unsigned char min_val, unsigned char max_val) {
	if (x > max_val)
		return max_val;
	else if (x < min_val)
		return min_val;
	else
		return x;
}

bool rgb24_to_yuv420p_buf(unsigned char *RgbBuf, int w, int h, unsigned char *yuvBuf) {
	unsigned char *ptrY, *ptrU, *ptrV, *ptrRGB;
	memset(yuvBuf, 0, w*h * 3 / 2);
	ptrY = yuvBuf;
	ptrU = yuvBuf + w * h;
	ptrV = ptrU + (w * h / 4);

	unsigned char y, u, v, r, g, b;
	for (int j = 0; j < h; j++) {
		ptrRGB = RgbBuf + w * j * 3;
		for (int i = 0; i < w; i++) {
			r = *(ptrRGB++);
			g = *(ptrRGB++);
			b = *(ptrRGB++);
			y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
			u = (unsigned char)((-38 * r - 74 * g + 122 * b + 128) >> 8) + 128;
			v = (unsigned char)((122 * r -94 * g - 18 * b + 128) >> 8) + 128;
			*(ptrY++) = clip_value(y, 0, 255);
			if (j % 2 == 0 && i % 2 == 0) {
				*(ptrU++) = clip_value(u, 0, 255);
			}
			else {
				if (i % 2 == 0) 
					*(ptrV++) = clip_value(v, 0, 255);
			}
		}
	}
	return true;
}

int rgb24_to_yuv420p(const char *url_in, int w, int h, int num, const char* url_out) {
	FILE *fp_in = fopen(url_in, "rb+");
	FILE *fp_out = fopen(url_out, "wb+");
	unsigned char *pic_rgb24 = (unsigned char *)malloc(w*h * 3);
	unsigned char *pic_yuv420 = (unsigned char *)malloc(w*h * 3 / 2);
	for (int i = 0; i < num; i++) {
		fread(pic_rgb24, 1, w*h * 3, fp_in);
		rgb24_to_yuv420p_buf(pic_rgb24, w, h, pic_yuv420);
		fwrite(pic_yuv420, 1, w*h * 3 / 2, fp_out);
	}
	free(pic_rgb24);
	free(pic_yuv420);
	fclose(fp_in);
	fclose(fp_out);
	return 0;
}

//(11)生成RGB24格式的彩条测试图
//从源代码可以看出，本程序循环输出“白黄青绿品红蓝黑”8种颜色的彩条。这8种颜色的彩条的R、G、B取值如下所示。
//颜色
//
//(R, G, B)
//
//白
//
//(255, 255, 255)
//
//黄
//
//(255, 255, 0)
//
//青
//
//(0, 255, 255)
//
//绿
//
//(0, 255, 0)
//
//品
//
//(255, 0, 255)
//
//红
//
//(255, 0, 0)
//
//蓝
//
//(0, 0, 255)
//
//黑
//
//(0, 0, 0)

int rgb24_colorbar(int width, int height, const char *url_out) {
	unsigned char * data = NULL;
	int barWidth;
	char fileName[100] = { 0 };
	FILE *fp = NULL;
	int i = 0, j = 0;

	data = (unsigned char*)malloc(width * height * 3);
	barWidth = width / 8;
	if ((fp = fopen(url_out, "wb+")) == NULL) {
		printf("Error: Cannot create file!");
		return -1;
	}
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			int barNum = i / barWidth;
			switch (barNum)
			{
			case 0: {
				data[(j*width + i) * 3 + 0] = 255;
				data[(j*width + i) * 3 + 1] = 255;
				data[(j*width + i) * 3 + 2] = 255;
				break;
			}
			case 1: {
				data[(j*width + i) * 3 + 0] = 255;
				data[(j*width + i) * 3 + 1] = 255;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			case 2: {
				data[(j*width + i) * 3 + 0] = 0;
				data[(j*width + i) * 3 + 1] = 255;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			case 3: {
				data[(j*width + i) * 3 + 0] = 0;
				data[(j*width + i) * 3 + 1] = 255;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			case 4: {
				data[(j*width + i) * 3 + 0] = 255;
				data[(j*width + i) * 3 + 1] = 0;
				data[(j*width + i) * 3 + 2] = 255;
				break;
			}
			case 5: {
				data[(j*width + i) * 3 + 0] = 255;
				data[(j*width + i) * 3 + 1] = 0;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			case 6: {
				data[(j*width + i) * 3 + 0] = 0;
				data[(j*width + i) * 3 + 1] = 0;
				data[(j*width + i) * 3 + 2] = 255;
				break;
			}
			case 7: {
				data[(j*width + i) * 3 + 0] = 0;
				data[(j*width + i) * 3 + 1] = 0;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			default:
				break;
			}
		}
	}
	fwrite(data, width*height * 3, 1, fp);
	fclose(fp);
	free(data);
	return 0;
}

//(11)分离PCM16LE双声道音频采样数据的左声道和右声道
//Test OK
int pcm16le_split_left_right(const char *url) {
	FILE *fp = fopen(url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open file!");
		return -1;
	}
	FILE *fp_left = fopen("asset/left_music_output.pcm", "wb+");
	FILE *fp_right = fopen("asset/right_music_output.pcm", "wb+");

	unsigned char *sample = (unsigned char *)malloc(4);
	while (!feof(fp)) {
		fread(sample, 1, 4, fp);  //一次读取4个Byte
		fwrite(sample, 1, 2, fp_left);  //前面两个放左边
		fwrite(sample + 2, 1, 2, fp_right);//后两两个放右边
	}
	free(sample);
	fclose(fp);
	fclose(fp_left);
	fclose(fp_right);
	return 0;
}

//(12)将PCM16LE双声道音频采样数据中左声道的音量降一半
//Test OK

int pcm16le_halfvolume_left_channel(const char *url) {
	FILE *fp = fopen(url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open file!");
		return -1;
	}
	FILE *fp_half = fopen("asset/output_leftchannel_halfvolume.pcm", "wb+");
	int cnt = 0;
	unsigned char *sample = (unsigned char *)malloc(4);
	while (!feof(fp))
	{
		short * samplenum = NULL;
		fread(sample, 1, 4, fp);
		//printf("current fp location: %d\n", fp);
		samplenum = (short *)sample;
		*samplenum = *samplenum / 2;
		fwrite(sample, 1, 2, fp_half);
		fwrite(sample + 2, 1, 2, fp_half);
		cnt++;
	}
	printf("Sample Cnt:%d\n", cnt);
	free(sample);
	fclose(fp);
	fclose(fp_half);
	return 0;
}

//(13)将PCM16LE双声道音频采样数据的声音速度提高一倍
//Test OK
int pcm16le_speedX2(const char *url) {
	FILE *fp = fopen(url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open file!");
		return -1;
	}
	int cnt = 0;
	FILE *fp_spped = fopen("asset/output_speedX2.pcm", "wb+");
	unsigned char *sample = (unsigned char *)malloc(4);
	while (!feof(fp))
	{
		fread(sample, 1, 4, fp);
		if (cnt % 2 != 0) {
			fwrite(sample, 1, 2, fp_spped);
			fwrite(sample + 2, 1, 2, fp_spped);
		}
		cnt++;
	}
	printf("Sample Cnt:%d\n", cnt);
	free(sample);
	fclose(fp);
	fclose(fp_spped);
	return 0;
}

//(14)将PCM16LE双声道音频采样数据转换为PCM8音频采样数据
//PCM16LE格式的采样数据的取值范围是 - 32768到32767，而PCM8格式的采样数据的取值范围是0到255。
//所以PCM16LE转换到PCM8需要经过两个步骤：第一步是将 - 32768到32767的16bit有符号数值转换为
//- 128到127的8bit有符号数值，第二步是将 - 128到127的8bit有符号数值转换为0到255的8bit无符号数值。
//在本程序中，16bit采样数据是通过short类型变量存储的，而8bit采样数据是通过unsigned char类型存储的
//Test Fail
//对于8bit 的PCM文件有两种形式，一种是中心 点为0的，还有一种是中心点为128的。
//下面的这个例子是加上了128的，所以中心点是128的，当然以0为中心点也没问题
//把fwrite(&samplenum8_u, 1, 1, fp8);改成fwrite(&samplenum8, 1, 1, fp8)就行
//这个时侯打开PCM文件要选择8bit PCM unsigned
int pcm16le_to_pcm8(const char *url) {
	FILE *fp = fopen(url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open file!");
		return -1;
	}
	int cnt = 0;
	FILE *fp8 = fopen("asset/output_to_pcm8.pcm", "wb+");
	unsigned char *sample = (unsigned char *)malloc(4);
	while (!feof(fp))
	{
		short *samplenum16 = NULL;
		char samplenum8 = 0;
		unsigned char samplenum8_u = 0;
		fread(sample, 1, 4, fp);
		samplenum16 = (short *)sample;
		samplenum8 = (*samplenum16) >> 8;
		samplenum8_u = samplenum8 + 128;
		fwrite(&samplenum8_u, 1, 1, fp8);
		samplenum16 = (short *)(sample + 2);
		samplenum8 = (*samplenum16) >> 8;
		samplenum8_u = samplenum8 + 128;
		fwrite(&samplenum8_u, 1, 1, fp8);
		cnt++;
	}
	printf("Sample Cnt:%d\n", cnt);
	free(sample);
	fclose(fp);
	fclose(fp8);
	return 0;
}

//将从PCM16LE单声道音频采样数据中截取一部分数据
//Test OK
int pcm16le_cut_singlechanel(const char* url, int start_num, int dur_num) {
		FILE *fp = fopen(url, "rb+");
		if (fp == NULL) {
			printf("Error: Cannot open file!");
			return -1;
		}
		int cnt = 0;
		FILE * fp_cut = fopen("asset/output_cut_singlechanel.pcm", "wb+");
		FILE *fp_stat = fopen("asset/output_cut.txt", "wb+");
		unsigned char *sample = (unsigned char *)malloc(4);
		while (!feof(fp))
		{
			fread(sample, 1, 2, fp);
			if (cnt > start_num && cnt <= (start_num + dur_num)) {
				fwrite(sample, 1, 2, fp_cut);
				short samplenum = sample[1];
				samplenum = samplenum * 256;
				samplenum = samplenum + sample[0];
				fprintf(fp_stat, "%6d", samplenum);
				if (cnt % 10 == 0)
					fprintf(fp_stat, "\n", samplenum);
			}
			cnt++;
		}
		printf("Sample Cnt:%d\n", cnt);
		free(sample);
		fclose(fp);
		fclose(fp_cut);
		fclose(fp_stat);
		return 0;
}

//将PCM16LE双声道音频采样数据转换为WAVE格式音频数据
//WAVE文件是一种RIFF格式的文件。其基本块名称是“WAVE”，其中包含了两个子块“fmt”和“data”。
//从编程的角度简单说来就是由WAVE_HEADER、WAVE_FMT、WAVE_DATA、采样数据共4个部分组成。它的结构如下所示。
//WAVE_HEADER

//WAVE_FMT

//WAVE_DATA

//PCM数据


//其中前3部分的结构如下所示。在写入WAVE文件头的时候给其中的每个字段赋上合适的值就可以了。
//但是有一点需要注意：WAVE_HEADER和WAVE_DATA中包含了一个文件长度信息的dwSize字段，
//该字段的值必须在写入完音频采样数据之后才能获得。因此这两个结构体最后才写入WAVE文件中。
//文件打开失败，应该是信息写入出现了错误，检查代码好像没什么问题
//memcpy(pcmFMT.fccID, "fmt ", strlen("fmt ")) fmt后面加上空格就行了，很神奇
int pcm16le_to_wav(const char* pcm_url, int channels, int sample_rate,const char *wav_url) {


	typedef struct WAVE_HEADER
	{
		char fccID[4];
		unsigned long dwSize;
		char fccType[4];
	}WAVE_HEADER;

	typedef struct WAVE_FMT
	{
		char fccID[4];
		unsigned long dwSize;
		unsigned short wFormatTag;
		unsigned short wChannels;
		unsigned long dwSamplesPerSec;
		unsigned long dwAvgBytesPerSec;
		unsigned short wBlockAlign;
		unsigned short uiBitsPerSample;
	}WAVE_FMT;

	typedef struct WAVE_DATA
	{
		char fccID[4];
		unsigned long dwSize;
	}WAVE_DATA;

	if (channels == 0 || sample_rate == 0) {
		channels = 2;
		sample_rate = 44100;
	}
	int bits = 16;
	WAVE_HEADER pcmHEADER;
	WAVE_FMT pcmFMT;
	WAVE_DATA pcmDATA;

	unsigned short m_pcmData;
	FILE *fp, *fp_out;

	fp = fopen(pcm_url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open input PCM file!");
		return -1;
	}
	fp_out = fopen(wav_url, "wb+");
	if (fp_out == NULL) {
		printf("Error: Cannot create output file!");
		return -1;
	}

	//WAVE_HEADER
	memcpy(pcmHEADER.fccID, "RIFF", strlen("RIFF"));
	memcpy(pcmHEADER.fccType, "WAVE", strlen("WAVE"));
	fseek(fp_out, sizeof(WAVE_HEADER), 1);

	pcmFMT.dwSamplesPerSec = sample_rate;
	pcmFMT.dwAvgBytesPerSec = pcmFMT.dwSamplesPerSec * sizeof(m_pcmData);
	pcmFMT.uiBitsPerSample = bits;
	memcpy(pcmFMT.fccID, "fmt ", strlen("fmt ")); //注意fmt后面有一个空格
	pcmFMT.dwSize = 16;
	pcmFMT.wBlockAlign = 2;
	pcmFMT.wChannels = channels;
	pcmFMT.wFormatTag = 1;

	fwrite(&pcmFMT, sizeof(WAVE_FMT), 1, fp_out);

	memcpy(pcmDATA.fccID,"data",strlen("data"));
	pcmDATA.dwSize = 0;
	fseek(fp_out, sizeof(WAVE_DATA), SEEK_CUR);

	fread(&m_pcmData, sizeof(unsigned short), 1, fp);
	while (!feof(fp))
	{
		pcmDATA.dwSize += 2;
		fwrite(&m_pcmData, sizeof(unsigned short), 1, fp_out);
		fread(&m_pcmData, sizeof(unsigned short), 1, fp);
	}
	pcmHEADER.dwSize = 44 + pcmDATA.dwSize;
	rewind(fp_out);
	fwrite(&pcmHEADER, sizeof(WAVE_HEADER), 1, fp_out);
	fseek(fp_out, sizeof(WAVE_FMT), SEEK_CUR);
	fwrite(&pcmDATA, sizeof(WAVE_DATA), 1, fp_out);
	fclose(fp);
	fclose(fp_out);

	return 0;
}