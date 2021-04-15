#include "WltRS.h"
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

int getBmp(char *inBuf, const int& inBufsize, char *OutBuf, int& outBufsize,const char* soPath)
{
	if(!inBuf || !OutBuf || inBufsize < IN_SIZE || outBufsize < OUT_SIZE)
	{
#if DEBUG
		log::logPutDebug(__FILE__, __LINE__, __PRETTY_FUNCTION__);
#endif
		return -2;
	}



/**************ARM-64**************/
        char cTransitionData[RAW_SIZE] = {0x00};
	
        int iRet = unpack(inBuf, cTransitionData, 602);
        printf("\niRet == %d\n",iRet);
        printf("\ninBuf[0] == %c\n",inBuf[0]);
        printf("\ninBuf[1] == %c\n",inBuf[1]);
        printf("\ninBuf[2] == %c\n",inBuf[2]);

        /******************************/
//        void* handle = dlopen(soPath,RTLD_LAZY);
//        if(!handle){
//            printf("\ncan not load .so file  \n");
//            return 1;
//        }
//        typedef int(*pUnpack_t)(char *, char *, int);
//        pUnpack_t pUnpack = (pUnpack_t)dlsym(handle,"unpack");
//        char* error;
//        if((error == dlerror()) != NULL){
//            printf("\ncan not load function  %s\n",error);
//            return -1;
//        }
//        int iRet = (pUnpack)(inBuf, cTransitionData, 602);
//        char* pszErr = dlerror();
//        if(pszErr != NULL)
//            dlclose(handle);
        /******************************/

        if(1 != iRet)
        {
#if DEBUG
                log::logPutDebug(__FILE__, __LINE__, __PRETTY_FUNCTION__);
#endif
                return -1;
        }

        //转换出来的数据再进行转换
        int iBmpPos = 0;
        int iTransitionPos = 0;
        memcpy(OutBuf, bmpHeader, sizeof(bmpHeader));
        iBmpPos += sizeof(bmpHeader);

        unsigned char temp;
        for(int i=0;i<RAW_SIZE;i+=3)
        {
                temp = cTransitionData[i];
                cTransitionData[i] = cTransitionData[i+2];
                cTransitionData[i+2] = temp;
        }

        for (int i=0;i<126;i++)
        {
                memcpy(OutBuf + iBmpPos, cTransitionData + iTransitionPos, 306);
                iBmpPos += 306;
                iTransitionPos += 306;
		
                OutBuf[iBmpPos++] = 0;
                OutBuf[iBmpPos++] = 0;
        }
/**************ARM-64结束**************/

/************ARM 32 begin*****************************/
//        int iRet = iDR220S_Dewlt((unsigned char *)inBuf, (unsigned char*)OutBuf);
//        if(iRet != 1)
//        {
//#if DEBUG
//                log::logPutDebug(__FILE__, __LINE__, __PRETTY_FUNCTION__);
//#endif
//                return -1;
//        }
/************ARM 32 end*****************************/

	return outBufsize = OUT_SIZE;
}


