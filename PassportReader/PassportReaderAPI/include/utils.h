#include <iostream>
#include <map>
#include "utils_zhangdi.h"
#include "json/json.h"
#include <opencv2/opencv.hpp>



using namespace std;
using namespace cv;


string trim(string s);
void mySplit(vector<string>& Result, string Input, const char* Rege);
void OCRRead(string cardInfoPath,DOC_TYPE docType,Json::Value& PassportInfo,map<string, string>& mapinfor);
void existAndDelete(const string& workfolder);
bool RemoveDir(const char* szFileDir);
void cutPhoto(Mat viF, int ocrResult,const string& image_Path);
