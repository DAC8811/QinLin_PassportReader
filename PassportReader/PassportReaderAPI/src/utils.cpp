#include "utils.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>


string trim(string s) {
	if (s.empty())
		return s;
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

//工具：以Regex分割字符串
void mySplit(vector<string>& Result, string Input, const char* Regex)
{
        int pos = 0;
        int npos = 0;
        int regexlen = strlen(Regex);
        while ((npos = Input.find(Regex, pos)) != -1)
        {
                string tmp = Input.substr(pos, npos - pos);
                Result.push_back(tmp);
                pos = npos + regexlen;
        }
        Result.push_back(Input.substr(pos, Input.length() - pos));
}

//全幅识别信息录入
void OCRRead(string workfolder,DOC_TYPE docType,Json::Value& PassportInfo,map<string, string>& mapinfor){
    //map<string, string> mapinfor;
    cout << "read cardInfo.txt ";
    string filename = workfolder + "/USB_TEMP/cardInfo.txt";
    ifstream text;
    text.open(filename.c_str(), ios::in);

    if (!text.is_open())
    {
            cout << "GetJson()-cardInfo text failed,path:" << filename;
            //root["PassportInfo"] = "";//护照信息字段
            return;
    }
    vector<string> strVec;
    stringstream json;
    while (!text.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
    {
            string inbuf;
            getline(text, inbuf, '\n');
            strVec.push_back(inbuf);
    }
    text.close();

    mapinfor["hxcIDNu"] = "";
    mapinfor["hxcCnName"] = "";
    mapinfor["hxcEnName"] = "";
    mapinfor["hxcSex"] = "";
    mapinfor["hxcBirth"] = "";
    mapinfor["hxcEndDate"] = "";
    mapinfor["hxcIDHKMNu"] = "";
    mapinfor["hxcSignDate"] = "";
    mapinfor["hxcUseLife"] = "";
    mapinfor["hxcSignDepart"] = "";
    mapinfor["hxcExCerNum"] = "";
    mapinfor["hxcOtherNames"] = "";
    mapinfor["hxcSignAddress"] = "";
    for (int i = 0; i < strVec.size()-1; i++) {
            vector<string> infor;
            mySplit(infor, strVec[i], " ");
            mapinfor[infor[0]] = infor[1];
    }
    if (docType == HK_MACAO_PASSPORT || docType == HK_MACAO_VISIT_MAINLAND || docType == TAIWAN_PASSPORT || docType == TAIWAN_VISIT_MAINLAND) {//港澳通行证
            vector<string> name;
            string lastName="";
            string firstName="";
            string::size_type idx1;
            string::size_type idx2;
            if (mapinfor["hxcEnName"].length()>2) {//切割英文姓名
                    idx1 = mapinfor["hxcEnName"].find("，");
                    idx2 = mapinfor["hxcEnName"].find(",");
                    if (idx1 != string::npos) {
                            mySplit(name, mapinfor["hxcEnName"].substr(0, mapinfor["hxcEnName"].length()), "，");
                            lastName = name[0];
                            firstName = name[1];
                    }
                    else if (idx2 != string::npos) {
                            mySplit(name, mapinfor["hxcEnName"].substr(0, mapinfor["hxcEnName"].length()), ",");
                            lastName = name[0];
                            firstName = name[1];
                    }
                    else{
                            firstName = mapinfor["hxcEnName"];
                    }

            }
            PassportInfo["BirthDate"] = mapinfor["hxcBirth"];
            //PassportInfo["FullName"] = GBKToUTF8(mapinfor["hxcCnName"]);//中文姓名字段
            PassportInfo["FullName"] = mapinfor["hxcCnName"];//中文姓名字段
            PassportInfo["FirstName"] = firstName;
            PassportInfo["MiddleName"] = "";
            PassportInfo["LastName"] = lastName;
            PassportInfo["Validity"] = mapinfor["hxcEndDate"];
//            PassportInfo["Gender"] = GBKToUTF8(mapinfor["hxcSex"]);
            PassportInfo["Gender"] = mapinfor["hxcSex"];
//            PassportInfo["hxcAddress"] = GBKToUTF8(mapinfor["hxcSignAddress"]);
            PassportInfo["hxcAddress"] = mapinfor["hxcSignAddress"];
            PassportInfo["hxcSignDate"] = mapinfor["hxcSignDate"];
//            PassportInfo["Issuing"] = GBKToUTF8(mapinfor["hxcSignDepart"]);
            PassportInfo["Issuing"] = mapinfor["hxcSignDepart"];
            PassportInfo["SerialNum"] = mapinfor["hxcIDNu"];

            if (docType == TAIWAN_VISIT_MAINLAND) {
                    PassportInfo["RenewalTimes"] = mapinfor["hxcExCerNum"];
                    string mrzName = "/USB_TEMP/mrz.txt";
                    ifstream mrzText;
                    mrzText.open(workfolder + mrzName, ios::in);
                    if (!mrzText.is_open())
                    {
                            cout << "GetJson()-MRZ text failed,path:" << workfolder + mrzName<<endl;
                            PassportInfo["MRZ1"] = "";
                            PassportInfo["MRZ2"] = "";
                            PassportInfo["MRZ3"] = "";
                            PassportInfo["RfIDMRZ"] = "";
                    }
                    else {
                            vector<string> mrzVec;
                            while (!mrzText.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
                            {
                                    string inbuf;
                                    getline(mrzText, inbuf, '\n');
                                    mrzVec.push_back(inbuf);
                            }
                            mrzText.close();
                            PassportInfo["MRZ1"] = mrzVec[0];
                            PassportInfo["MRZ2"] = mrzVec[1];
                            PassportInfo["MRZ3"] = mrzVec[2];
                            PassportInfo["RfIDMRZ"] = mrzVec[0]+ mrzVec[1]+ mrzVec[2];
                            PassportInfo["hxcIDTWNu"] = mrzVec[1].substr(19,10);
                    }
            }
            else if (docType == HK_MACAO_VISIT_MAINLAND) {
                    string mrzName = "/USB_TEMP/mrz.txt";
                    ifstream mrzText;
                    mrzText.open(workfolder + mrzName, ios::in);
                    if (!mrzText.is_open())
                    {
                            cout << "GetJson()-MRZ text failed,path:" << workfolder + mrzName<<endl;
                            PassportInfo["MRZ1"] = "";
                            PassportInfo["MRZ2"] = "";
                            PassportInfo["MRZ3"] = "";
                            PassportInfo["RfIDMRZ"] = "";
                    }
                    else {
                            vector<string> mrzVec;
                            while (!mrzText.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
                            {
                                    string inbuf;
                                    getline(mrzText, inbuf, '\n');
                                    mrzVec.push_back(inbuf);
                            }
                            mrzText.close();
                            PassportInfo["MRZ1"] = mrzVec[0];
                            PassportInfo["MRZ2"] = mrzVec[1];
                            PassportInfo["MRZ3"] = mrzVec[2];
                            PassportInfo["RfIDMRZ"] = mrzVec[0] + mrzVec[1] + mrzVec[2];
                    }
            }
            else if (docType == HK_MACAO_PASSPORT || docType == TAIWAN_PASSPORT) {
                    string mrzName = "/USB_TEMP/mrz.txt";
                    ifstream mrzText;
                    mrzText.open(workfolder + mrzName, ios::in);
                    if (!mrzText.is_open())
                    {
                            cout << "GetJson()-MRZ text failed,path:" << workfolder + mrzName;
                            PassportInfo["MRZ1"] = "";
                            PassportInfo["RfIDMRZ"] = "";
                    }
                    else {
                            vector<string> mrzVec;

                            while (!mrzText.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
                            {
                                    string inbuf;
                                    getline(mrzText, inbuf, '\n');
                                    mrzVec.push_back(inbuf);
                            }
                            mrzText.close();
                            PassportInfo["MRZ1"] = mrzVec[0];
                            PassportInfo["RfIDMRZ"] = mrzVec[0];
                    }
            }
    }
//    if (docType == HK_MACAO_VISIT_MAINLAND) {
//            string mrzName = "/USB_TEMP/mrz.txt";
//            ifstream mrzText;
//            mrzText.open(workfolder + mrzName, ios::in);
//            if (!mrzText.is_open())
//            {
//                    cout << "GetJson()-MRZ text failed,path:" << workfolder + mrzName;
//                    PassportInfo["MRZ1"] = "";
//                    PassportInfo["MRZ2"] = "";
//                    PassportInfo["MRZ3"] = "";
//                    PassportInfo["RfIDMRZ"] = "";
//            }
//            else {
//                    vector<string> mrzVec;
//                    while (!mrzText.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
//                    {
//                            string inbuf;
//                            getline(mrzText, inbuf, '\n');
//                            mrzVec.push_back(inbuf);
//                    }
//                    mrzText.close();
//                    PassportInfo["MRZ1"] = mrzVec[0];
//                    PassportInfo["MRZ2"] = mrzVec[1];
//                    PassportInfo["MRZ3"] = mrzVec[2];
//                    PassportInfo["RfIDMRZ"] = mrzVec[0] + mrzVec[1] + mrzVec[2];
//            }
//    }


    //root["PassportInfo"] = PassportInfo;
}

void existAndDelete(const string& workfolder) {
        string s = workfolder + "/USB_TEMP/mrz.txt";
        ifstream mrzTxt;
        mrzTxt.open(s, ios::in);

        if (mrzTxt.is_open())
        {
                mrzTxt.close();
                //删除
                remove(s.c_str());
        }
        //删除usbtemp文件夹下所有文件
        string s1 = workfolder + "/USB_TEMP";
        RemoveDir(s1.c_str());
}

bool RemoveDir(const char* szFileDir){
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(szFileDir)) == NULL){
        printf("[func:RemoveDir]cannot open directory:%s\n",szFileDir);
        return -1;
    }
    chdir(szFileDir);
    while((entry = readdir(dp)) != NULL){
        lstat(entry->d_name,&statbuf);
        if(S_ISREG(statbuf.st_mode))
            remove(entry->d_name);
    }
    return 0;
}

void cutPhoto(Mat viF, int ocrResult,const string& image_Path){//剪切头像
        if (ocrResult == 2) {//裁剪新版回乡证头像
                Rect rect(floor(viF.cols * 0.04), floor(viF.rows * 0.25), floor(viF.cols * 0.24), floor(viF.rows * 0.52));
                Mat viF_roi = viF(rect);
                imwrite(image_Path, viF_roi);
        }
        else if (ocrResult == 20) {//裁剪旧版回乡证头像
                Rect rect(floor(viF.cols * 0.04), floor(viF.rows * 0.19), floor(viF.cols * 0.31), floor(viF.rows * 0.6));
                Mat viF_roi = viF(rect);
                imwrite(image_Path, viF_roi);
        }
        else {
                Rect rect(floor(viF.cols * 0.04), floor(viF.rows * 0.22), floor(viF.cols * 0.27), floor(viF.rows * 0.6));
                Mat viF_roi = viF(rect);
                imwrite(image_Path, viF_roi);
        }
}
