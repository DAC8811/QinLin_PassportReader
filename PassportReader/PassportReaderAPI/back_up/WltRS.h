/*************************
照片解码库

记录：
2019/12/31 创建
*************************/
#ifndef __WLTRS_H__
#define __WLTRS_H__

//#include "globalMacro.h"

#define RAW_SIZE	38556
#define IN_SIZE		1024
#define OUT_SIZE	38862




extern "C" int unpack(char *src, char *dst, int bmpSave);//这里改为显示调用

//extern "C" int iDR220S_Dewlt(unsigned char* pwlt, unsigned char* pbmp);



//BMP图片头部
const unsigned char bmpHeader[54] = {0x42, 0x4D, 0xCE, 0x97, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,\
						 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,\
						 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
						 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//class WltRs
//{
//public:
	/****************************
	函数名：getBmp
	功能：bmp照片解码，调用公安部接口后，再进行转换
	参数：
		infile：输入原始数据 1024字节
		inBufsize: 输入原始数据长度
		Outfile：输出bmp图片数据
		outBufsize：Outfile的长度,成功返回输出图片的长度
	返回值：
		0：解析失败
		>0：输出图片的长度
		-2：传入参数为空指针或缓冲区大小不够
		-1：调用第三方照片解码接口失败
		-99：未知错误
	*****************************/
	//static int getBmp(char *inBuf, const int &inBufsize, char *OutBuf, int &outBufsize); 	
extern "C"    int getBmp(char *inBuf, const int &inBufsize, char *OutBuf, int &outBufsize,const char* soPath); 	
//};


#endif

