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
    unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);
    for(int i=0;i<num;i++){
        size_t count = fread(pic, 1, w * h * 3 / 2, fp);
        cout << "count " << count << endl;
        fwrite(pic, 1, w * h, fp1);
        fwrite(pic + w * h, 1, w * h / 4, fp2);
        fwrite(pic + w * h * 5 / 4, 1, w * h / 4, fp3);
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
	FILE *fp1 = fopen("Media/material/output_444_y.y", "wb+");
	FILE *fp2 = fopen("Media/material/output_444_u.y", "wb+");
	FILE *fp3 = fopen("Media/material/output_444_v.y", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3);
	for (int i = 0; i<num; i++) {
		fread(pic, 1, w * h * 3, fp);
		fwrite(pic, 1, w * h, fp1);
		fwrite(pic + w * h, 1, w * h, fp2);
		fwrite(pic + w * h * 2, 1, w * h, fp3);
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}
//(3) 将YUV420P像素数据去掉颜色（变成灰度图）
//如果想把YUV格式像素数据变成灰度图像，只需要将U、V分量设置成128即可。这是因为U、V是图像中的经过偏置处理的色度分量。色度分量在偏置处理前的取值范围是-128至127，
//这时候的无色对应的是“0”值。经过偏置后色度分量取值变成了0至255，因而此时的无色对应的就是128了。上述调用函数的代码运行后，
//将会把一张分辨率为256x256的名称为lena_256x256_yuv420p.yuv的YUV420P格式的像素数据文件处理成名称为output_gray.yuv的YUV420P格式的像素数据文件
int yuv420_gray(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("Media/material/output_yuv420_gray.yuv", "wb+");
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
//对应C语言中的unsigned char数据类型。上述调用函数的代码运行后，将会把一张分辨率为256x256的名称为lena_256x256_yuv420p.yuv的YUV420P格式的像素数据文件处理成名称为output_half.yuv的YUV420P格式的像素数据文件
int yuv420_harf_luminance(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("Media/material/output_yuv420_harf_luminance.yuv", "wb+");
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
//图像的边框的宽度为border，本程序将距离图像边缘border范围内的像素的亮度分量Y的取值设置成了亮度最大值255。上述调用函数的代码运行后，
//将会把一张分辨率为256x256的名称为lena_256x256_yuv420p.yuv的YUV420P格式的像素数据文件处理成名称为output_border.yuv的YUV420P格式
//的像素数据文件
int yuv420_add_Border(char *url, int w, int h, int border, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("Media/material/output_yuv420_add_border.yuv", "wb+");
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
int yuv420_graybar(int width, int height, int ymin, int ymax, int barnum, char *url_output)
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
	FILE *fp1 = fopen("Media/material/output_r.y", "wb+");
	FILE *fp2 = fopen("Media/material/output_g.y", "wb+");
	FILE *fp3 = fopen("Media/material/output_b.y", "wb+");
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
