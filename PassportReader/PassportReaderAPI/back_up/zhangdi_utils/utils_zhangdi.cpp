//
// Created by 张迪 on 2020/5/15.
//

#include "utils_zhangdi.h"

std::map<std::string, std::string> ETHNICITY{
        {"01", "汉族"},{"02", "蒙古族"},{"03", "回族"},{"04", "藏族"},{"05", "维吾尔族"},{"06", "苗族"},{"07", "彝族"},
        {"08", "壮族"},{"09", "布依族"},{"10", "朝鲜族"},{"11", "满族"},{"12", "侗族"},{"13", "瑶族"},{"14", "白族"},
        {"15", "土家族"},{"16", "哈尼族"},{"17", "哈萨克族"},{"18", "傣族"},{"19", "黎族"},{"20", "傈傈族"},{"21", "佤族"},
        {"22", "畲族"},{"23", "高山族"},{"24", "拉祜族"},{"25", "水族"},{"26", "东乡族"},{"27", "纳西族"},{"28", "景颇族"},
        {"29", "柯尔克孜族"},{"30", "土族"},{"31", "达斡尔族"},{"32", "仫佬族"},{"33", "羌族"},{"34", "布朗族"},{"35", "撒拉族"},
        {"36", "毛难族"},{"37", "仡佬族"},{"38", "锡伯族"},{"39", "阿昌族"},{"40", "普米族"},{"41", "塔吉克族"},{"42", "怒族"},
        {"43", "乌孜别克族"},{"44", "俄罗斯族"},{"45", "鄂温克族"},{"46", "崩龙族"},{"47", "保安族"},{"48", "裕固族"},{"49", "京族"},
        {"50", "塔塔尔族"},{"51", "独龙族"},{"52", "鄂伦春族"},{"53", "赫哲族"},{"54", "门巴族"},{"55", "珞巴族"},{"56", "基洛族"},
        {"57", "其他"},{"58", "外国血统中国籍人士"},{"59", "港澳台"}};
std::map<std::string, std::string> GENDER{
        {"F", "F"},{"M", "M"},{"1", "M"},{"0", "F"}};

void split(const std::string &s, std::vector<std::string> &tokens, const std::string &delimiters) {
    std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    std::string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos) {
        tokens.push_back(s.substr(lastPos, pos - lastPos));//use emplace_back after C++11
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}
//工具：判断year(只有年份的后两位)的前两位是20还是19
bool isTwentiethCentury(std::string year)
{
    if (strncmp(year.c_str(), "50", 2) <= 0)
        return true;
    return false;
}
std::string delN(std::string ocrInfo){
    std::string temp = ocrInfo;
    size_t index1 = temp.find("\n");//找到\n的下标
    if(index1 == std::string::npos){
        return temp;                //第一次就没有找到说明是一行机读码，直接返回
    }
    temp.replace(index1,1,"");      //将第一个\n替换
    size_t index2 = temp.find("\n");//开始找第2个
    if(index2 == std::string::npos){
        return temp;                //没有，说明是两行机读码，可以返回
    }
    temp.replace(index2,1,"");      //找到了，就是2行机读码，替换之后返回
    return temp;
}
std::vector<int> str2num1(std::string s) {
    std::vector<int> num;

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c >= 48 && c <= 57) {
            num.push_back(c - 48);
        } else if (c >= 65 && c <= 90) {
            num.push_back(c - 45);
        } else if (60 == c) {
            num.push_back(0);
        } else {
            return std::vector<int>(0);
        }
    }
    return num;
}

static bool PassportCheck(std::string number, char checkBit) {
    int sum = 0;
    int check = (checkBit - 48) & 0x0f;
    int BaseWeight[] = {7, 3, 1};

    std::vector<int> intNumber = str2num1(number);
    for (int i = 0, k = 0; i < intNumber.size(); i++) {
        sum += intNumber[i] * BaseWeight[k];
        k = (k + 1) % 3;
    }
    return ((sum % 10) == check);
}

bool checkPassportMRZ(std::string mrzcode)
{
    //读护照的机读码进行校验位比对
    //注意： 第一行没有校验位，所以OCR错误无法识别出来

    //机读码分行,并判断每一行的长度
    std::vector<std::string> splitstr;
    size_t location = mrzcode.find("\n");
    if (location < 44 || location == std::string::npos) {
        return false;
    }
    splitstr.push_back(std::string(mrzcode, 0, 44));
    splitstr.push_back(std::string(mrzcode, location + 1, 44));

    if (splitstr[1].length() < 44) {
        return false;
    }

    //对证件号码进行校验
    bool idNumCheck = PassportCheck(splitstr[1].substr(0, 9), splitstr[1].substr(9, 1).at(0));
    //对证件出生日期进行校验
    bool birthCheck = PassportCheck(splitstr[1].substr(13, 6), splitstr[1].substr(19, 1).at(0));
    //对证件有效日期进行校验
    bool validCheck = PassportCheck(splitstr[1].substr(21, 6), splitstr[1].substr(27, 1).at(0));
    //附加校验位
    bool additionCheck = PassportCheck(splitstr[1].substr(28, 14), splitstr[1].substr(42, 1).at(0));
    //第2行整个校验位
    std::string temp = splitstr[1].substr(0, 10) + splitstr[1].substr(13, 7) + splitstr[1].substr(21, 22);
    bool fullCheck = PassportCheck(temp, splitstr[1].substr(43, 1).at(0));
    if(idNumCheck && birthCheck && validCheck && additionCheck && fullCheck)
        return true;
    return false;

}

bool checkHXZMRZ(std::string mrzcode)
{
    //对回乡证的机读码进行校验, 不支持老版回乡证
    if(mrzcode.length() < 92)
        return false;
    std::string info = delN(mrzcode);//把OCR字符串中间的换行符号去除
    std::string firstLine = info.substr(0,30);
    std::string secondLine = info.substr(30,30);
    //std::string thirdLine = info.substr(60,30);

    //证件号校验
    bool idNumCheck = PassportCheck(firstLine.substr(2, 9), mrzcode.substr(11,1).at(0));
    //签发次数校验
    bool renewalTimesCheck = PassportCheck(firstLine.substr(12, 2), mrzcode.substr(14,1).at(0));
    //证件有效期校验
    bool validCheck = PassportCheck(firstLine.substr(15, 6), mrzcode.substr(21, 1).at(0));
    //生日校验
    bool birthCheck = PassportCheck(firstLine.substr(23, 6), mrzcode.substr(29, 1).at(0));
    //香港身份证号校验
    bool hkidCheck = PassportCheck(secondLine.substr(19, 9), mrzcode.substr(28, 1).at(0));

    //总校验 第一行3-22位、24-30位，第二行20-29位
    std::string temp = firstLine.substr(2, 20) + firstLine.substr(23, 7) + secondLine.substr(19, 10);
    bool fullCheck = PassportCheck(temp, secondLine.substr(29, 1).at(0));
    if(idNumCheck && renewalTimesCheck && birthCheck && hkidCheck && validCheck && fullCheck)
        return true;
    return false;

}
bool checkTWMMRZ(std::string mrzcode)
{
    //对台湾居民往来大陆通行证的机读码进行校验. 传入的mrzcode均包含换行符
    if(mrzcode.length() < 92)
        return false;
    std::string info = delN(mrzcode);//把OCR字符串中间的换行符号去除
    std::string firstLine = info.substr(0,30);
    std::string secondLine = info.substr(30,30);
    //std::string thirdLine = info.substr(60,30);

    //证件号校验
    bool idNumCheck = PassportCheck(firstLine.substr(2, 9), mrzcode.substr(11,1).at(0));
    //签发次数校验
    bool renewalTimesCheck = PassportCheck(firstLine.substr(12, 2), mrzcode.substr(14,1).at(0));
    //证件有效期校验
    bool validCheck = PassportCheck(firstLine.substr(15, 6), mrzcode.substr(21, 1).at(0));
    //生日校验
    bool birthCheck = PassportCheck(firstLine.substr(23, 6), mrzcode.substr(29, 1).at(0));

    //总校验 第一行3-22位、24-30位，第二行20-29位
    std::string temp = firstLine.substr(2, 20) + firstLine.substr(23, 7) + secondLine.substr(19, 10);
    bool fullCheck = PassportCheck(temp, secondLine.substr(29, 1).at(0));
    if(idNumCheck && renewalTimesCheck && birthCheck && validCheck && fullCheck)
        return true;
    return false;

}
bool checkHKTWMRZ(std::string mrzcode)
{
    //对港澳通行证和台湾通行证的校验位进行校验
    if(mrzcode.length() < 30)
        return false;
    bool idNumCheck = PassportCheck(mrzcode.substr(2, 9), mrzcode.substr(11,1).at(0));
    bool validCheck = PassportCheck(mrzcode.substr(13, 6), mrzcode.substr(19, 1).at(0));
    bool birthCheck = PassportCheck(mrzcode.substr(21, 6), mrzcode.substr(27, 1).at(0));
    std::string temp = mrzcode.substr(2, 10) + mrzcode.substr(13, 7) + mrzcode.substr(21, 7);
    bool fullCheck = PassportCheck(temp, mrzcode.substr(29, 1).at(0));
    if(idNumCheck && birthCheck && validCheck && fullCheck)
        return true;
    return false;

}
std::string processMRZ(std::string mrzCode, DOC_TYPE docType, std::string checkResult, std::string checkDetails){
    if(mrzCode.empty()||mrzCode.length() == 0)
        return "";
    std::string jsonStr;
    switch(docType) {
        case PASSPORT: {
            jsonStr = processPassportJson(mrzCode,"",checkResult,checkDetails,docType,-1);
            break;
        }
        case HK_MACAO_PASSPORT: {
            jsonStr = processHKTWJson(mrzCode,"",checkResult,checkDetails,docType,-1);
            break;
        }
        case HK_MACAO_VISIT_MAINLAND: {
            jsonStr = processHXZJson(mrzCode,"",checkResult,checkDetails,docType,-1);
            break;
        }
        case TAIWAN_VISIT_MAINLAND:{
            jsonStr = processTWMJson(mrzCode,"",checkResult,checkDetails,docType,-1);
            break;
        }
        case TAIWAN_PASSPORT:{
            jsonStr = processHKTWJson(mrzCode,"",checkResult,checkDetails,docType,-1);
            break;
        }
        default: {
            std::stringstream json;
            json << "{"
                 << "\"ChipReadResult\":\"" << "-1" << "\","
                 << "\"DocumentType\":\"" << -1 << "\","
                 << "\"IDcardInfo\":\"" << "" << "\","
                 << "\"PassportInfo\":\"" << "" << "\""
                 << "}" << std::ends;
            jsonStr = json.str();
            break;
        }
    }
    return jsonStr;
}
std::string processDG(std::string DG1, std::string DG11, DOC_TYPE docType, std::string checkResult, std::string checkDetails){
    if(DG1.empty()||DG1.length()==0)
        return "";
    std::string jsonStr;
    switch (docType){
        case PASSPORT: {
            jsonStr = processPassportJson(DG1,DG11,checkResult,checkDetails,docType,1);
            break;
        }
        case HK_MACAO_PASSPORT: {
            jsonStr = processHKTWJson(DG1,DG11,checkResult,checkDetails,docType,1);
            break;
        }
        case HK_MACAO_VISIT_MAINLAND: {
            jsonStr = processHXZJson(DG1,DG11,checkResult,checkDetails,docType,1);
            break;
        }
        case TAIWAN_VISIT_MAINLAND: {
            jsonStr = processTWMJson(DG1,DG11,checkResult,checkDetails,docType,1);
            break;
        }
        case TAIWAN_PASSPORT:{
            jsonStr = processHKTWJson(DG1,DG11,checkResult,checkDetails,docType,1);
            break;
        }
        default: {
            std::stringstream json;
            json << "{"
                 << "\"ChipReadResult\":\"" << "-1" << "\","
                 << "\"DocumentType\":\"" << -1 << "\","
                 << "\"IDcardInfo\":\"" << "" << "\","
                 << "\"PassportInfo\":\"" << "" << "\""
                 << "}" << std::ends;
            jsonStr = json.str();
            break;
        }
    }
    return jsonStr;
}
//"张迪             10119970503河南省方城县博望镇杨岗村张八庄５０号                 411322199705032911方城县公安局         2013071620230716                  \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
//15                1 2   8             35                                          18                15                           8 8

std::string utf16Toutf8(std::string str){
    if(str.empty() || str.length() == 0)
        return "";
    try {
        std::vector<unsigned short> utf16line((unsigned short *) str.data(),
                                              (unsigned short *) str.data() + str.size()/2);
        str.clear();
        utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(str));

    }
    catch (const utf8::invalid_utf16 &) {
        // bad UTF-16 data!
    }
    return str;
}
void trim(std::string &s)
{
    if (s.empty())
    {
        return ;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
}
std::string processSFZInfo(std::string sfzInfo){
    if(sfzInfo.empty() || sfzInfo.length() == 0)
        return "";
    std::string name, address, authority,birthday, ethnicity, idnumber, validate,gender;

    name = utf16Toutf8(sfzInfo.substr(0,30));
    address = utf16Toutf8(sfzInfo.substr(52,70));
    authority = utf16Toutf8(sfzInfo.substr(158,30));

    trim(name);
    trim(address);
    trim(authority);

    birthday = utf16Toutf8(sfzInfo.substr(36,16));
    ethnicity = utf16Toutf8(sfzInfo.substr(32,4));
    idnumber = utf16Toutf8(sfzInfo.substr(122,36));
    validate = utf16Toutf8(sfzInfo.substr(188,32));
    gender = utf16Toutf8(sfzInfo.substr(30,2));

    if(gender == "1")
        gender = "M";
    else
        gender = "F";
    //std::vector<std::string> buffers;
    std::stringstream json;
    //split(sfzInfo,buffers," ");
    json << "{"
         << "\"ChipReadResult\":\"" << "1" << "\","
         << "\"DocumentType\":\"" << "1" << "\","
         << "\"IDcardInfo\":"
         << "{"
//         << "\"Address\":\"" << buffers[1].substr(11, std::string::npos) << "\","
//         << "\"Authority\":\"" << buffers[2].substr(18, std::string::npos) << "\","
//         << "\"Birthdate\":\"" << buffers[2].substr(6, 8) << "\","
//         << "\"Ethnicity\":\"" << ETHNICITY[buffers[1].substr(1, 2)] << "\","
//         << "\"Gender\":\"" << GENDER[buffers[1].substr(0, 1)] << "\","
//         << "\"IDnumber\":\"" << buffers[2].substr(0, 18) << "\","
//         << "\"Name\":\"" << buffers[0] << "\","
//         << "\"Validity_Start\":\"" << buffers[3].substr(0, 8) << "\","
//         << "\"Validity_Termi\":\"" << buffers[3].substr(8, std::string::npos) << "\""
            << "\"Address\":\"" << address << "\","
            << "\"Authority\":\"" << authority << "\","
            << "\"Birthdate\":\"" << birthday << "\","
            << "\"Ethnicity\":\"" << ETHNICITY[ethnicity] << "\","
            << "\"Gender\":\"" << gender << "\","
            << "\"IDnumber\":\"" << idnumber << "\","
            << "\"Name\":\"" << name << "\","
            << "\"Validity_Start\":\"" << validate.substr(0,8) << "\","
            << "\"Validity_Termi\":\"" << validate.substr(8, std::string::npos) << "\""
         << "},"
         << "\"PassportInfo\":\"" << "" << "\""
         << "}" << std::ends;
    return json.str();
}

void parseName(std::string nameLine, std::string &firstName, std::string &lastName){
    int index = nameLine.find_first_of("<<");
    std::string lastnameLine = nameLine.substr(0,index);//姓
    std::string firstnameLine = nameLine.substr(index,std::string::npos);//名
    std::vector<std::string> buffers1;
    std::vector<std::string> buffers2;
    split(lastnameLine,buffers1,"<");
    split(firstnameLine,buffers2,"<");
    for(int i=0; i < buffers2.size(); i++){
        firstName = firstName + buffers2[i];
    }
    for(int i = 0; i < buffers1.size(); i++){
        lastName = lastName + buffers1[i];
    }
}

std::string processHXZJson(std::string mrzCode, std::string DG11,std::string checkResult, std::string checkDetails,DOC_TYPE docType, int chipStatus){
    std::stringstream json;
    std::string jsonStr;
    std::string info = delN(mrzCode);//把OCR字符串中间的换行符号去除
    std::string firstLine = info.substr(0,30);
    std::string secondLine = info.substr(30,30);
    std::string nameLine = info.substr(60,30);//这个是第三行，我把它给分割
    std::string firstname="";
    std::string lastname ="";
    std::string birthPre = "";
    std::string validPre = "";
    parseName(nameLine,firstname,lastname);
    if(mrzCode.substr(0,2).compare("C<")==0){
        //老版回乡证
        if(secondLine.substr(27,1)=="0"){
            birthPre = "18";
        } else if(secondLine.substr(27,1)=="1"){
            birthPre = "19";
        } else{
            birthPre = "20";
        }
        if(secondLine.substr(28,1)=="0"){
            validPre = "18";
        } else if(secondLine.substr(28,1)=="1"){
            validPre = "19";
        } else{
            validPre = "20";
        }
        json << "{"
             << "\"ChipReadResult\":\"" << chipStatus << "\","
             << "\"DocumentType\":\"" << docType << "\","
             << "\"IDcardInfo\":\"" << "" << "\","
             << "\"PassportInfo\":"
             << "{"
             << "\"BirthDate\":\"" << birthPre + secondLine.substr(0, 6) << "\","
             //<< "\"DocType\":\"" << mrzInfo.Type << "\","
             << "\"DocType\":\"" << "C" << "\","
             << "\"FirstName\":\"" << firstname << "\","
             << "\"Gender\":\"" << GENDER[secondLine.substr(7, 1)] << "\","
             << "\"Issuing\":\"" << firstLine.substr(2,3) << "\","
             << "\"LastName\":\"" << lastname << "\","
             << "\"MiddleName\":\"" << "" << "\","
             << "\"Nationality\":\"" << "CHN" << "\","
             << "\"Option1\":\"" << "" << "\","
             << "\"SerialNum\":\"" << secondLine.substr(15, 11) << "\","
             << "\"Validity\":\"" << validPre + secondLine.substr(8, 6) << "\","
             << "\"Authenticy\":\"" << checkResult << "\","
             << "\"FullName\":\"" << DG11 << "\","
             << "\"RenewalTimes\":\"" << "" << "\","
             << "\"AuthDetails\":\"" << checkDetails <<"\""
             << "}"
             << "}" << std::ends;
        jsonStr = json.str();
        return jsonStr;
    }
    if(secondLine.substr(18,1)=="A"){
        birthPre = "18";
    } else if(secondLine.substr(18,1)=="B"){
        birthPre = "19";
    } else{
        birthPre = "20";
    }
    if(isTwentiethCentury(firstLine.substr(15,2))){
        validPre = "20";
    } else{
        validPre = "19";
    }
    std::string hkid = secondLine.substr(19, 9);
    if (hkid.find("<") != std::string::npos) {
        hkid = secondLine.substr(19, 8);
    }
    json << "{"
         << "\"ChipReadResult\":\"" << chipStatus << "\","
         << "\"DocumentType\":\"" << 3 << "\","
         << "\"IDcardInfo\":\"" << "" << "\","
         << "\"PassportInfo\":"
         << "{"
         << "\"BirthDate\":\"" << birthPre + firstLine.substr(23, 6) << "\","
         //<< "\"DocType\":\"" << mrzInfo.Type << "\","
         << "\"DocType\":\"" << "C" << "\","
         << "\"FirstName\":\"" << firstname << "\","
         << "\"Gender\":\"" << GENDER[firstLine.substr(22, 1)] << "\","
         << "\"Issuing\":\"" << "" << "\","
         << "\"LastName\":\"" << lastname << "\","
         << "\"MiddleName\":\"" << "" << "\","
         << "\"Nationality\":\"" << "CHN" << "\","
         << "\"Option1\":\"" << hkid << "\","
         << "\"SerialNum\":\"" << firstLine.substr(2, 9) << "\","
         << "\"Validity\":\"" << validPre + firstLine.substr(15, 6) << "\","
         << "\"Authenticy\":\"" << checkResult << "\","
         << "\"FullName\":\"" << DG11 << "\","
         << "\"RenewalTimes\":\"" << firstLine.substr(12,2) << "\","
         << "\"AuthDetails\":\"" << checkDetails <<"\""
         << "}"
         << "}" << std::ends;
    jsonStr = json.str();
    return jsonStr;
}
std::string processTWMJson(std::string mrzCode, std::string DG11, std::string checkResult, std::string checkDetails,DOC_TYPE docType, int chipStatus){
    std::string info = delN(mrzCode);//把OCR字符串中间的换行符号去除
    std::string firstLine = info.substr(0,30);
    std::string secondLine = info.substr(30,30);
    std::string thirdLine = info.substr(60,30);

    std::stringstream json;
    std::string jsonStr;
    std::string firstname="";
    std::string lastname ="";
    std::string birthPre = "";
    std::string validPre = "";
    if(secondLine.substr(18,1)=="A"){
        birthPre = "18";
    } else if(secondLine.substr(18,1)=="B"){
        birthPre = "19";
    } else{
        birthPre = "20";
    }
    if(isTwentiethCentury(firstLine.substr(15,2))){
        validPre = "20";
    } else{
        validPre = "19";
    }
    parseName(thirdLine.substr(0,26),firstname,lastname);

    std::string twid = secondLine.substr(19, 10);
    //剔除后面的<
    if (twid.find("<") != std::string::npos) {
        //hkid = hkid.substr(0, 8);
        int index = twid.find("<");
        twid = twid.substr(0, index);
    }
    json << "{"
         << "\"ChipReadResult\":\"" << chipStatus << "\","
         << "\"DocumentType\":\"" << docType << "\","
         << "\"IDcardInfo\":\"" << "" << "\","
         << "\"PassportInfo\":"
         << "{"
         << "\"BirthDate\":\"" << birthPre + firstLine.substr(23, 6) << "\","
         //<< "\"DocType\":\"" << mrzInfo.Type << "\","
         << "\"DocType\":\"" << "C" << "\","
         << "\"FirstName\":\"" << firstname << "\","
         << "\"Gender\":\"" << GENDER[firstLine.substr(22, 1)] << "\","
         << "\"Issuing\":\"" << thirdLine.substr(26,4) << "\","
         << "\"LastName\":\"" << lastname << "\","
         << "\"MiddleName\":\"" << "" << "\","
         << "\"Nationality\":\"" << "CHN" << "\","
         << "\"Option1\":\"" << twid << "\","
         << "\"SerialNum\":\"" << firstLine.substr(2, 8) << "\","
         << "\"Validity\":\"" << validPre + firstLine.substr(15, 6) << "\","
         << "\"Authenticy\":\"" << checkResult << "\","
         << "\"FullName\":\"" << DG11 << "\","
         << "\"RenewalTimes\":\"" << firstLine.substr(12,2) << "\","
         << "\"AuthDetails\":\"" << checkDetails <<"\""
         << "}"
         << "}" << std::ends;
    jsonStr = json.str();
    return jsonStr;
}
std::string processPassportJson(std::string mrzCode, std::string DG11, std::string checkResult, std::string checkDetails,DOC_TYPE docType, int chipStatus){
    //如果是MRZ，会删除换行，如果是DG1，没有变化
    mrzCode = delN(mrzCode);
    std::stringstream json;
    std::string jsonStr;
    std::string firstLine = mrzCode.substr(0,44);
    std::string secondLine = mrzCode.substr(44,44);
    std::string firstname = "";
    std::string lastname = "";
    std::string birthPre = "";
    std::string validPre = "";
    if(isTwentiethCentury(secondLine.substr(13,2))){
        birthPre = "20";
    } else{
        birthPre = "19";
    }
    if(isTwentiethCentury(secondLine.substr(21,2))){
        validPre = "20";
    } else{
        validPre = "19";
    }
    parseName(firstLine.substr(5,39),firstname,lastname);
    json << "{"
         << "\"ChipReadResult\":\"" << chipStatus << "\","
         << "\"DocumentType\":\"" << docType << "\","
         << "\"IDcardInfo\":\"" << "" << "\","
         << "\"PassportInfo\":"
         << "{"
         << "\"BirthDate\":\"" << birthPre + secondLine.substr(13, 6) << "\","
         //<< "\"DocType\":\"" << mrzInfo.Type << "\","
         << "\"DocType\":\"" << "P" << "\","
         << "\"FirstName\":\"" << firstname << "\","
         << "\"Gender\":\"" << GENDER[secondLine.substr(20, 1)] << "\","
         << "\"Issuing\":\"" << firstLine.substr(2, 3) << "\","
         << "\"LastName\":\"" << lastname << "\","
         << "\"MiddleName\":\"" << "" << "\","
         << "\"Nationality\":\"" << secondLine.substr(10, 3) << "\","
         << "\"Option1\":\"" << "" << "\","
         << "\"SerialNum\":\"" << secondLine.substr(0, 9) << "\","
         << "\"Validity\":\"" << validPre + secondLine.substr(21, 6) << "\","
         << "\"Authenticy\":\"" << checkResult << "\","
         << "\"FullName\":\"" << DG11 << "\","
         << "\"RenewalTimes\":\"" << "" << "\","
         << "\"AuthDetails\":\"" << checkDetails <<"\""
         << "}"
         << "}" << std::ends;
    jsonStr = json.str();
    return jsonStr;
}
std::string processHKTWJson(std::string mrzCode, std::string DG11, std::string checkResult, std::string checkDetails,DOC_TYPE docType, int chipStatus){
    std::stringstream json;
    std::string jsonStr;

    std::string firstname="";
    std::string lastname ="";
    std::string birthPre = "";
    std::string validPre = "";
    std::string gender = "";

    std::string firstLine = mrzCode.substr(0,30);
    std::string secondLine = "";
    std::string thirdLine = "";
    if(chipStatus==1){
        //判断是芯片
        secondLine = mrzCode.substr(30,30);
        thirdLine = mrzCode.substr(60,30);
        gender = GENDER[thirdLine.substr(0, 1)];
        parseName(secondLine.substr(12,18),firstname,lastname);
    }

    if(isTwentiethCentury(firstLine.substr(21,2))){
        birthPre = "20";
    } else{
        birthPre = "19";
    }
    if(isTwentiethCentury(firstLine.substr(13,2))){
        validPre = "20";
    } else{
        validPre = "19";
    }
    json << "{"
         << "\"ChipReadResult\":\"" << chipStatus << "\","
         << "\"DocumentType\":\"" << docType << "\","
         << "\"IDcardInfo\":\"" << "" << "\","
         << "\"PassportInfo\":"
         << "{"
         << "\"BirthDate\":\"" << birthPre + firstLine.substr(21, 6) << "\","
         //<< "\"DocType\":\"" << mrzInfo.Type << "\","
         << "\"DocType\":\"" << "C" << "\","
         << "\"FirstName\":\"" << firstname << "\","
         << "\"Gender\":\"" << gender << "\","
         << "\"Issuing\":\"" << "" << "\","
         << "\"LastName\":\"" << lastname << "\","
         << "\"MiddleName\":\"" << "" << "\","
         << "\"Nationality\":\"" << "" << "\","
         << "\"Option1\":\"" << "" << "\","
         << "\"SerialNum\":\"" << firstLine.substr(2, 9) << "\","
         << "\"Validity\":\"" << validPre + firstLine.substr(13, 6) << "\","
         << "\"Authenticy\":\"" << checkResult << "\","
         << "\"FullName\":\"" << DG11 << "\","
         << "\"RenewalTimes\":\"" << "" << "\","
         << "\"AuthDetails\":\"" << checkDetails <<"\""
         << "}"
         << "}" << std::ends;
    jsonStr = json.str();
    return jsonStr;
}

