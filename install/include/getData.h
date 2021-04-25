//
// Created by 张迪 on 2020/3/21.
//

#ifndef PASSPORTREADER_GETDATA_H
#define PASSPORTREADER_GETDATA_H
#include <string>

using namespace std;

#ifdef __ANDROID__
#define EXPORT_FUN __attribute__((visibility("default")))
#else
#define EXPORT_FUN __attribute__((visibility("default")))
#endif

/******************************************************/
// 函数名   : initProgram
// 功能描述 : 设备SDK初始化，在调用任何SDK其他接口前，必须先调用该接口进行初始化。该函数在整个进程运行期间只需要调用一次。
// 参数     : WorkingFolder 工作目录，字符串
// 返回值   : 成功时，返回0
//            否则参考日志输出
/******************************************************/
extern "C" EXPORT_FUN
int initProgram(const char* workingFolder);
/******************************************************/
// 函数名   : getData
// 功能描述 :采集数据（Ocr、读卡)
// 参数     :viFront 正面白光图像完整路径
//			 viBack 背面白光图像完整路径
//返回值   :0 护照读取成功；-1 失败
//
/******************************************************/
extern "C" EXPORT_FUN
//int getData(const char* whitePath, const char* irPath,const char* uvPath, int authenFlag);
int getData(const char* viFront, const char* viBack, const char* irFront, const char* irBack, const char* uvFront, const char* uvBack);
/******************************************************/
// 函数名   :closeProgram
// 功能描述 :关闭设备
// 参数     :无
//
// 返回值   :0 成功； -1 失败
//
/******************************************************/
extern "C" EXPORT_FUN
int closeProgram();
/******************************************************/
// 函数名   :getJsonNew
// 功能描述 :获取证件各项信息
// 参数     :无
// 返回值   :Json字符串
//
/******************************************************/
extern "C" EXPORT_FUN
const char* getJsonNew();
extern "C" EXPORT_FUN
int findOrientation();
extern "C" EXPORT_FUN
string getEChipDG1();
extern "C" EXPORT_FUN
string getEChipDG11();
/******************************************************/
// 函数名   :getSFZImage
// 功能描述 :获取身份证电子头像的原始数据
// 参数     :无
// 返回值   :1024字节数据
//
/******************************************************/
extern "C" EXPORT_FUN
const char* getSFZImage();
extern "C" EXPORT_FUN
int IDcardReadInfoAndFinger(unsigned char *sInfo, unsigned int * sLength, unsigned char *cpPhoto, unsigned int *ipLength, unsigned char *cpFinger, unsigned int *ipFLength,unsigned char* idType);
#endif //PASSPORTREADER_GETDATA_H
