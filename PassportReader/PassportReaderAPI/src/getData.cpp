//
// Created by 张迪 on 2020/3/21.
//

// vim: set tabstop=4 shiftwidth=4 expandtab fileencoding=utf-8
#include <unistd.h>
#include <sstream>
#include <regex>
#include <sys/stat.h>
#include "utf8.h"
#include <time.h>
#include <opencv2/imgproc/types_c.h>
#include <sys/time.h>

#include "libChip.h"
//#include "mrz.h"
#include "MRTD.h"
#include "ImageProcess.h"
#include "utils_zhangdi.h"
#include "pyContentCheck.h"

#include "getData.h"
#include <dlfcn.h>


#include "WltRS.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include<unistd.h>
#include<sys/time.h>
#include <signal.h>
#include <stdlib.h>

#include "cardRecog.h"
//#include <opencv2/opencv.hpp>
#include "cardInfoRecog.h"

#include "utils.h"
#include"json/json.h"
#include <map>

EXPORT_FUN
char LOGBUFFER[300];

// #define LOG_DEBUG
#define LOG_TAG "PassportReaderCPP-API"

#include "internalLogging.h"

using namespace std;
using namespace cv;

#ifndef USE_OLD_OCR
//MRZ OCRworker;
void* yolo = NULL;
void* ocrT = NULL;
void* ocrS = NULL;
#endif

string workingFolder;
struct MrzInfoStruct mrzInfo;
struct timeval start, stop;

string mrzCode, DG1, DG11, SFZinfo, SFZinfoUTF16, SFZImage, jsonInfo;
Json::Value PassportInfo;
std::map<string, string> mapinfor;
int Orientation = -2;


//-1 - error
// 0 - unknow
// 1 - sfz
// 2 - passport
// 3 - other 1-line MRZ cards
// 4 - other 3-line MRZ cards
int chipStatus, sfzStatus, insertType;
string authenticity_result,authenticity_details;
DOC_TYPE docType = UNKNOWN;

int borderResult = -1;

//1,SFZ;   0. Passport/HK
int getCardType(){
    int thisCardType = PassportTest();
    if(thisCardType == 0){
        return 1;//SFZ
    } else if(thisCardType == 1){
        return 0;//Passport
    } else if(thisCardType == -1){
        return -1;//无芯片
    } else {
        return thisCardType;//-2 设备没打开
    }
}

int LogOpen(string logDir){
    mkdir(logDir.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    FLAGS_log_dir = logDir;
    google::InitGoogleLogging("PassportReaderCIS");
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 30;
    FLAGS_stop_logging_if_full_disk = true;
    LOG(INFO)<< "PassportReader Version: 2020年12月16日21：00"<<std::endl;
    return 0;
}
int LogClose(){
    google::ShutdownGoogleLogging();
    return 0;
}
//保存最终图片（经过剪裁）并返回Mat, n取0~3分别代表 红外，白光，紫外 的路径;docType：0--护照；1--身份证；2--港澳通行证
Mat saveFinalImage(const Mat &trans, Mat& src, Size outputsize, int n)
{
    if (n<0 || n>2)
        return Mat();
    Mat Oring;
    src.copyTo(Oring);
    Mat FinalResize;
    warpAffine(Oring, FinalResize, trans, outputsize, INTER_LINEAR, BORDER_REPLICATE);
    if(n==0){
        imwrite(workingFolder+"/USB_TEMP/IR.bmp", FinalResize);
    }else if(n==1){
        imwrite(workingFolder+"/USB_TEMP/White.bmp", FinalResize);
    }else{
        imwrite(workingFolder+"/USB_TEMP/UV.bmp", FinalResize);
    }

    return FinalResize;
}
string getAuthenticity(const char* whitePath, const char* irPath, const char* uvPath, int resultType){

    //jsonInfo = processDG("CDL138092367<2903149<9205266<2MOLKMOKM<<<<WEI<<WEI<<<<<<<<<<FAAB16<<43<<<<<<<<<<<<<<<<<<<<","张三",TAIWAN_PASSPORT);
    string CodePath =  workingFolder + "/PatternCode/";
    long long result = pyTrueResult(const_cast<char *>(whitePath),
                                    const_cast<char *>(irPath),
                                    const_cast<char *>(uvPath),
                                    const_cast<char *>(mrzCode.c_str()),
                                    const_cast<char *>(CodePath.c_str()));
    string re = to_string(result);
    //得到RCode的字符串形式，从右到左依次是鉴伪点1、鉴伪点2、鉴伪点3...
    authenticity_details = re.substr(0,14);
    string checkResult = "0";
    checkResult = re.substr(14,1);
    if(checkResult == "2")
        return "-1";
    else
        return checkResult;
}

extern "C" __attribute__((visibility("default"))) int initProgram(const char* folder){
    string stringFolder = folder;
    mkdir((stringFolder + "/USB_TEMP").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    workingFolder = stringFolder;
    LogOpen(workingFolder + "/logs");
    time_t tt = time(NULL);
    struct tm* t = localtime(&tt);
    int year = t->tm_year + 1900;
    int mon = t->tm_mon;
    int day = t->tm_mday;
    //todo
//    if(year >2021||mon>9||year<2021){
//        LOGE("SDK使用权限到期，无法继续使用");
//        return -1;
//    }
    string authFilePath = workingFolder + "/auth.ini";
    if(!AuthDev(authFilePath)){
        LOGE("设备认证失败，退出...");
        return -1;
    }
    // choose the right model: mrz32_61.pb or mrz32_20190523.pb
#ifndef USE_OLD_OCR
//    if (OCRworker.initNet( workingFolder + "/model/mrz32_61.pb") != 0) {
//        LOGE("OCR model failed to load");
//        return -2;
//    }
    string yolo_model_path = workingFolder + "/models";
    yolo = init_yolo_model(yolo_model_path.c_str());

    string crnn_model_pathS = workingFolder + "/crnn_model";
    ocrT = init_crnn_net(crnn_model_pathS.c_str());

    string crnn_model_pathT = workingFolder + "/crnn_model";
    ocrS = init_crnn_net(crnn_model_pathT.c_str());
#endif
    return 0;
}

//extern "C" __attribute__((visibility("default"))) int getData(const char* whitePath, const char* irPath,const char* uvPath, int authenFlag){
extern "C" __attribute__((visibility("default"))) int getData(const char* viFront, const char* viBack, const char* irFront, const char* irBack, const char* uvFront, const char* uvBack){
    //cleanup everything
    docType = UNKNOWN, chipStatus = 0, sfzStatus = 0, insertType=-1;
    mrzCode.clear(), DG1.clear(), DG11.clear(), SFZinfo.clear(), SFZinfoUTF16.clear(), SFZImage.clear(), jsonInfo.clear(),authenticity_result = "0",authenticity_details="";
    PassportInfo.clear();
    mapinfor.clear();

    existAndDelete(workingFolder);

    //int chipType = getCardType();
    /********************先是对三种图像进行裁剪和旋转**********************/

    //string stringIRPath = irPath;
    //Mat irImg = imread(stringIRPath);
    //if(irImg.empty()){
    //    docType = UNKNOWN;
    //    return 2;//读取图像失败
    //}

//    Size outputSize;
//    Mat trans = edgeDetectionAny(insertType,outputSize,irImg);
//    if(trans.empty()){
//        docType = UNKNOWN;
//        return 3;
//    }
//    //仿射变换并保存图片，返回的是裁剪之后的图片数据
//    Mat finalIr = saveFinalImage(trans,irImg,outputSize,0);
//    Mat uvSrc = imread(uvPath);
//    Mat whiteSrc = imread(whitePath);
//    saveFinalImage(trans, whiteSrc, outputSize, 1);
//    saveFinalImage(trans, uvSrc, outputSize, 2);


    /********************然后先按照身份证来读**********************/
    string output, img;
    sfzStatus = IDCardRead(output, img);
    output = output.substr(0,220);
    if (sfzStatus == 1) {
        //convert to utf-8
        try {
            vector<unsigned short> utf16line((unsigned short *) output.data(),
                                             (unsigned short *) output.data() + output.size()/2);
            utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(SFZinfo));
        }
        catch (const utf8::invalid_utf16 &) {
            // bad UTF-16 data!
        }
        docType = ID_CARD;
        SFZImage = img;
        SFZinfoUTF16 = output;

        /******************save bmp**********************************************/
        int inBufsize = 1024;
        int iPhotoDataLen = 38862;
        char arrPhotoData[iPhotoDataLen];
        char test[1024];
        //string soPath = workingFolder + "/libwlt2019.so";//
        int iRet = WltRs::getBmp((char *)SFZImage.c_str(), 1024, arrPhotoData, iPhotoDataLen);
        //int len = getBmp((char*)SFZImage.c_str(),inBufsize,test,outBufsize,soPath.c_str());
        //printf("\niRet == %d\n",iRet);
        if(iRet > 0){
                string imagePath = workingFolder + "/USB_TEMP/DG2.bmp";
                FILE* fp_photo = fopen(imagePath.c_str(),"wb+");
                if(fp_photo == NULL)
                {
                        printf(" write Photo error\n");
                        return -1;
                }
                fwrite(arrPhotoData, 1, iPhotoDataLen, fp_photo);
                fclose(fp_photo);
         }
        else{
                printf(" WltRs::getBmp error:%d\n", iRet);
        }

        return 1;
    }
    /********************读身份证失败之后再按照其他证件来处理**********************/

    LOGI("-----开始读取数据-----");
    gettimeofday(&start,NULL);
    //cvtColor(irImg,irImg,CV_RGB2GRAY);//finalIr
    //如果是小卡片
//    if(insertType == 1){
        //判断是1行机读码还是3行机读码，1行是1  3行是3
        //insertType = OCRworker.getMrzNums(irImg);//finalIr
//    }
//    printf("\ninsertType == %d\n",insertType);

/*    int OCRType = 0;
    if(insertType == 1){
        //港澳通行证  台湾通行证
        OCRType = 1;
        //docType = HK_MACAO_PASSPORT;
    } else if(insertType == 2){
        //护照
        OCRType = 0;
        docType = PASSPORT;
    } else if(insertType == 3){
        //三行证件
        OCRType = 2;
    }else{
        docType == UNKNOWN;
        return 3;
    }*/

#ifndef USE_OLD_OCR
    std::string outputPath = workingFolder + "/USB_TEMP/";
    cv::Mat vi_A = cv::imread(viFront);
    cv::Mat vi_B = cv::imread(viBack);
    if (vi_A.empty() || vi_B.empty())
    {
            LOG(WARNING) << "getData()-read Image Failed";
            return 2;
    }
    string mrz_modelPath = workingFolder + "/mrz_model/mrz32_61.pb";
    int res = getCardInfo(outputPath.c_str(), ocrS,ocrT,yolo, mrz_modelPath.c_str(),viFront, viBack);
    int ocrResult = res / 10;
    Orientation = res % 10;
    if(ocrResult < 0){
        LOG(WARNING) << "mrzRecog()-ocr failed!ocrResult: " << ocrResult;
    }

    /**********************save the head portrait*************************/
    string image_PathName = "/USB_TEMP/DG2.bmp";
    string image_Path = workingFolder + image_PathName;
    if (Orientation == 1) {//切割小卡片头像
            flip(vi_A, vi_A, 0);
            flip(vi_A, vi_A, 1);
            cutPhoto(vi_A, ocrResult,image_Path);
    }
    else if (Orientation == 2) {
            cutPhoto(vi_B, ocrResult,image_Path);
    }
    else if (Orientation == 3) {
            flip(vi_B, vi_B, 0);
            flip(vi_B, vi_B, 1);
            cutPhoto(vi_B, ocrResult,image_Path);
    }
    else{
            cutPhoto(vi_A, ocrResult,image_Path);
    }
	
    /********************读取机读码****************************************/
    std::string filename = workingFolder+"/USB_TEMP/mrz.txt";
    ifstream t;
    t.open(filename, ios::in);
    if (!t.is_open())
    {
            LOG(WARNING) << "mrz.txt open failed! mrz.txt Path: " << filename;
            return 1;
    }
    std::stringstream buffer;
    buffer << t.rdbuf();
    mrzCode = buffer.str();
    t.close();
    t.open(filename, ios::in);
    int mrzRows = 0;
    while (!t.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
    {
            string inbuf;
            getline(t, inbuf, '\n');
            if (trim(inbuf).size() == 0) {
                    continue;
            }
            mrzRows++;
    }
    t.close();
    if (mrzRows == 2) {
            insertType = 0;
    }
    else if (mrzRows == 1) {
            insertType = 1;
    }
    else if (mrzRows == 3) {
            insertType = 3;
    }
    //printf("\nmrzRows == %d\n",mrzRows);
    /********************判断类型******************************************/

    int OCRType = 0;
    if(insertType == 1){
        //港澳通行证  台湾通行证
        OCRType = 1;
        //docType = HK_MACAO_PASSPORT;
    } else if(insertType == 2){
        //护照
        OCRType = 0;
        docType = PASSPORT;
    } else if(insertType == 3){
        //三行证件
        OCRType = 2;
    }else{
        docType == UNKNOWN;
        return 3;
    }
#else
        Mat MRZImage = findMrzAPI(camera.IRImage, 0, 0);
        mrzCode = mrzOcrAPI(MRZImage, 0, "");
#endif
    if (mrzCode.length() < 10) {
        LOGE("OCR found nothing, aborting...");
        docType = UNKNOWN;
        return 3;
    }
    LOGI("OCR RESULT: %s", mrzCode.c_str());
    //printf("OCR RESULT: %s\r\n", mrzCode.c_str());

    gettimeofday(&stop,NULL);
    int timesss = (stop.tv_sec - start.tv_sec) * 1000000 +(stop.tv_usec - start.tv_usec);
    printf("OCR finished and used time: %d us\r\n",timesss);

//    printf("\nOCRType == %d\n",OCRType);


    if(OCRType == 2){
        //如果是三行证件，根据OCR的结果来判断是回乡证还是台胞证
        if(mrzCode.substr(0,2).compare("CT")==0){
            docType = TAIWAN_VISIT_MAINLAND;
        } else {
            docType = HK_MACAO_VISIT_MAINLAND;
        }

    } else if(OCRType == 1){
        if(mrzCode.substr(0,2).compare("CD")==0){
            docType = TAIWAN_PASSPORT;
        } else{
            docType = HK_MACAO_PASSPORT;
        }
    }

    OCRRead(workingFolder,docType,PassportInfo,mapinfor);

    //if((docType == PASSPORT)&&((authenFlag == 1) || (authenFlag == 2))){
        //string myUvPath = workingFolder+"/USB_TEMP/UV.bmp";
        //string myWhitePath = workingFolder+ "/USB_TEMP/White.bmp";
        //string myIRPath = workingFolder+ "/USB_TEMP/White.bmp";
        //authenticity_result = getAuthenticity(myWhitePath.c_str(),myIRPath.c_str(),myUvPath.c_str(),authenFlag);
    //}
    printf("Read chip start...\r\n");
    gettimeofday(&start,NULL);

    //mrzCode = "POCHNYUAN<<PEIPEI<<<<<<<<<<<<<<<<<<<<<<<<<<<\nE561240497CHN9008168F2507204NEKMMFOFMFOFA920";
    //docType = PASSPORT;
    //mrzCode = "CSC531580954<2612199<9712087<2";
    //mrzCode = "CSC917260184<2808059<8502038<4";
    //docType = HK_MACAO_PASSPORT;
    //mrzCode = "P<NLDMEULENDIJK<<LOES<ALBERTINE<<<<<<<<<<<<<\nXA12345624NLD8210070F1401280123456782<<<<<02";
    //docType = PASSPORT;
    //mrzCode = "CDL138092367<2903149<9205266<2";
    //docType = TAIWAN_PASSPORT;

//    printf("\ndocType == %d\n",docType);
    switch (docType) {
        case HK_MACAO_PASSPORT: //单行卡片
            chipStatus = EchipOneCardRead(mrzCode + "\n", DG1, DG11, workingFolder + "/USB_TEMP/DG2.bmp", mrzInfo);
            break;
        case TAIWAN_PASSPORT:
            chipStatus = EchipOneCardRead(mrzCode + "\n", DG1, DG11, workingFolder + "/USB_TEMP/DG2.bmp", mrzInfo);
            break;
        case PASSPORT://双行卡片
            chipStatus = EchipTwoCardRead(mrzCode + "\n", DG1, DG11,workingFolder + "/USB_TEMP/DG2.bmp", mrzInfo);
            break;
        default: //三行证件
            chipStatus = EchipThreeCardRead(mrzCode + "\n", DG1, DG11, workingFolder + "/USB_TEMP/DG2.bmp", mrzInfo);
            break;
    }
    gettimeofday(&stop,NULL);
    //timesss = (stop.tv_sec - start.tv_sec) * 1000000 +(stop.tv_usec - start.tv_usec);
    //printf("Read chip finished and used time %d us\r\n",timesss);
    if (chipStatus){
        LOGI("chip reader status: %d, DG1: %s, DG11: %s", chipStatus, DG1.c_str(),
             DG11.c_str());
    }else{
        LOGE("chip reader status: %d, read failed or no chip", chipStatus);
        chipStatus = -1;
    }
    return 0;
}

extern "C" __attribute__((visibility("default"))) int closeProgram(){
    LogClose();
    return 0;
}

extern "C" __attribute__((visibility("default"))) const char* getJsonNew(){
    if (docType < 0) {
        //未识别出证件
        return  "";
    }
    if(docType == UNKNOWN){
        jsonInfo = "";
    }
    if(docType == ID_CARD){
        jsonInfo = processSFZInfo(SFZinfoUTF16);
    } else if(!DG1.empty()){
        string info = getEChipDG1();
        string fullName = getEChipDG11();
        jsonInfo = processDG(info,fullName,docType,authenticity_result,authenticity_details,PassportInfo,mapinfor);
    } else if(!mrzCode.empty()){
        jsonInfo = processMRZ(mrzCode,docType,authenticity_result,authenticity_details,PassportInfo,mapinfor);
    }
    return jsonInfo.c_str();
}
extern "C" __attribute__((visibility("default"))) string getEChipDG1() {
    if(docType == ID_CARD){
        return SFZinfo;
    }
    if (!chipStatus)
        return ("");
    else
        return DG1;
}
extern "C" __attribute__((visibility("default"))) string getEChipDG11() {
    if (!chipStatus)
        return ("");

    //find second
    std::string UTF8_name_tag("\x5f\x0e", 2);
    size_t offset = DG11.find(UTF8_name_tag);
    if (offset == std::string::npos)
        return ("");
    offset = DG11.find(UTF8_name_tag, offset + 2);
    if (offset == std::string::npos)
        return ("");

    unsigned char size = DG11[offset + 2];

    return (DG11.substr(offset + 3, size));
}
extern "C" __attribute__((visibility("default"))) const char* getSFZImage(){
    if (docType != 1)
        return NULL;
    else{
        static char sfzImage[1024];
        for(int i = 0 ; i < 1024; i++){
            sfzImage[i] = SFZImage[i];
        }
        return sfzImage;
    }
}

extern "C" __attribute__((visibility("default"))) int findOrientation() {
        return Orientation;
}
