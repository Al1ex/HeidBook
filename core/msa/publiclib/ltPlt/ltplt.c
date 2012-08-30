#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>
#include "publiclib.h"
#include "ltplt.h"

#define LTPLT_NOEXISTVAR_SPACES         0
#define LTPLT_NOEXISTVAR_PRNVAR         1
#define LTPLT_NOEXISTVAR_EXIT           2
#define LTPLT_MAXVARLEN      24
#define LTPLT_ALLOCSTEP      4096      /* ÿ�η���ռ�Ĵ�С    */
#define LTPLT_MARKB          "[$"      /* ������ʶ��ʼ�ַ���    */
#define LTPLT_MARKE          "$]"      /* ������ʶ����ֹ�ַ���  */
#define LTPLT_LOOPB          "begintable"    /* ѭ����������ʼ��ʶ    */
#define LTPLT_LOOPE          "endtable"    /* ѭ����������ֹ��ʶ    */

char *gb2312_to_utf8(const char *cp, int *err);

int   utPltIfVarNoExist=0;   /* �������������  0---�ÿ�   1---���滻  2---����  */

typedef struct ltPltHtmBuf_S {
        unsigned int iLen;                  /* ��ǰ�ַ�������          */
        unsigned int iMaxBytes;             /* Ŀǰ����ռ����󳤶�  */
        char     *pBuffer;                  /* ���ݻ�����              */
} ltPltHtmBuf;



ltPltHtmBuf *ltPltInitHtmBuf(int iMaxBytes)
{
    ltPltHtmBuf *psHtmBuf;
    psHtmBuf = (ltPltHtmBuf *)malloc(sizeof(ltPltHtmBuf));
    if(psHtmBuf == NULL) {
        return NULL;
    }
    else {
    	  memset(psHtmBuf,0,sizeof(ltPltHtmBuf));
        psHtmBuf->iLen = 0;
        psHtmBuf->pBuffer = malloc(iMaxBytes);
        if(psHtmBuf->pBuffer == NULL) {
            free(psHtmBuf);
            return NULL;
        }
        else {
        	  memset(psHtmBuf->pBuffer,0,iMaxBytes);
            //strcpy(psHtmBuf->pBuffer,"\n\n");
            psHtmBuf->iMaxBytes = iMaxBytes;
            return psHtmBuf;
        }
    }
}

int ltPltFreeHtmBuf(ltPltHtmBuf *psHtmBuf)
{
    if(psHtmBuf->pBuffer)
        free(psHtmBuf->pBuffer);
    free(psHtmBuf);
    return 0;
}


/* ȡ�ַ�������ʶ   */
int ltPltGetStrToMark(char *pIn, ltPltHtmBuf *psBuf, 
                    char *pVarName, char *pMarkB,char *pMarkE)
{
    register int i,j;
    int iLen,i1;
    iLen = strlen(pIn);
    for(i=0;i<iLen;i++) {
        if(pIn[i] == pMarkB[0] && pIn[i+1] == pMarkB[1]) {
            i1 = i;
            i=i+2;
            j=0;
            for(j=0;i<iLen && j<LTPLT_MAXVARLEN;i++,j++){
                if(pIn[i] == pMarkE[0] && pIn[i+1] == pMarkE[1]) {
                    i=i+2;
                    pVarName[j] = 0;
                    return i;
                }
                else {
                    pVarName[j]=pIn[i];
                }
            }
            i = i1;
        }
        if(psBuf->iLen > psBuf->iMaxBytes -1 ) {
           psBuf->pBuffer = realloc(psBuf->pBuffer,psBuf->iMaxBytes+LTPLT_ALLOCSTEP);
           if(psBuf->pBuffer == NULL) {
                    return (-1);
           }
           psBuf->iMaxBytes =  psBuf->iMaxBytes+LTPLT_ALLOCSTEP;
        }
        psBuf->pBuffer[psBuf->iLen] = pIn[i];
        psBuf->iLen ++;
    }
    return 0;
}


/*pFile ģ��ָ�� ltDbBuf xmlģ������*/
char *ltPltFileParse(char *pFile,xmlDocPtr ltDocPtr,int chSet)
{
    int  iFd,iReturn,iFileSize;
    struct stat  sStatBuf;
    char *p,*pBuffer,*utfP;
    char caFileName[256];
   int err;
  
   strcpy(caFileName,pFile);
  
    iFd = open(caFileName,O_RDONLY);
    if(iFd < 0) {
          return NULL;
    }
    
    iReturn = fstat(iFd,&sStatBuf);
    if(iReturn == (-1)) {
         close(iFd);
        return NULL;
    }
    iFileSize = sStatBuf.st_size;
    pBuffer = malloc(iFileSize + 1);
    if(pBuffer == NULL) {
        close(iFd);
        return NULL;
    }
    iReturn = read(iFd,pBuffer,iFileSize);
    close(iFd);
    if(iReturn != iFileSize) {
         free(pBuffer);
        return NULL;
    }
    pBuffer[iFileSize]=0;
    if(chSet==0){
				utfP=pBuffer;
    }
    //else{
	  //  utfP=gb2312_to_utf8(pBuffer, &err);
	  //  free(pBuffer);
    //}
    p = ltPltMemParse(utfP,ltDocPtr);
    free(utfP);
    return p;
}    

/* ��ģ��ת��Ϊ HTML�ַ���  
       pIn      ---- ģ�����ݵ��ַ���
       ltDbBuf ---- ģ������xml�ṹ
   ����:
       ���ɵ� HTML�ַ���
 
 */

char *ltPltMemParse(char *pIn,xmlDocPtr ltDocPtr)
{

        char caVarName[32];
        char caVarName0[32];
        char *p,*pValue;
				char *tempTableName;
				char *tempTableName1;
        xmlNodePtr tableNodePtr;
				xmlNodePtr bodyNode;
        int iReturn = 1;
        int iLoopNum,iLen,iStep;
        ltPltHtmBuf *htmBuf; 
				char *xmlhead;
				long xmlheadlen;        

	/*������ݻ������������ԣ����ģ�����*/
       if(ltDocPtr == NULL) {
            p = malloc(strlen(pIn) + 1);
            if(p) {
                strcpy(p,pIn);
            }
	    			return p;
        }

	bodyNode = lt_dbget_body(ltDocPtr);
	/*������ݻ������������ԣ����ģ�����*/
	if(bodyNode == NULL) {
            p = malloc(strlen(pIn) + 1);
            if(p) {
                strcpy(p,pIn);
            }
	    return p;
        }

	
	/*��ʼ��ģ�滺����*/
   	 htmBuf = ltPltInitHtmBuf(strlen(pIn)  + LTPLT_ALLOCSTEP);
     if(htmBuf == NULL) {
				return NULL;
     }
        
	   p = pIn;
         
         while(iReturn > 0) {
                iReturn = ltPltGetStrToMark(p,htmBuf,caVarName,LTPLT_MARKB,LTPLT_MARKE);
                if(iReturn > 0) {
                        if( strncmp(caVarName, LTPLT_LOOPB,10) == 0) {  /* ѭ����ʶ  */
											     tempTableName=strchr(caVarName,' ');
											     tempTableName1=NULL;
											     if(tempTableName!=NULL){
															tempTableName1=ltStrDelSpaces(tempTableName);
															if(strlen(tempTableName1)==0){
																tempTableName1=NULL;
															}
											     }
											     p = p + iReturn;
											     if(*p=='\n'){
															p=p+1;
											     }
											     /*
											 	�õ�tableNodePtr
												name=tablename	
								 			    */	
                             iReturn = ltPltDoTable(p,htmBuf,bodyNode,tempTableName1);
                             if(iReturn > 0) {
                                p = p + iReturn;
                             }
                        }
                        else {  /* һ�����  */
                                pValue = lt_dbget_Var(bodyNode,caVarName);
                                if(pValue) {
                                        iLen = strlen(pValue);
                                        if(htmBuf->iLen + iLen > htmBuf->iMaxBytes) {
                                                if(iLen < LTPLT_ALLOCSTEP) 
                                                        iStep = LTPLT_ALLOCSTEP;
                                                else {
                                                        iStep = iLen;
                                                }
                                                htmBuf->pBuffer = realloc(htmBuf->pBuffer, htmBuf->iMaxBytes + iStep);
                                                if(htmBuf->pBuffer == NULL) {
                                                       return NULL;
                                                }
                                                htmBuf->iMaxBytes = htmBuf->iMaxBytes + iStep;

                                        }
                                        memcpy(htmBuf->pBuffer + htmBuf->iLen,pValue,iLen);
                                        htmBuf->iLen = htmBuf->iLen + iLen;
                                }
                                p = p + iReturn;
                        }
                }
        }
        htmBuf->pBuffer[htmBuf->iLen]=0;
				p=htmBuf->pBuffer;
        free(htmBuf);
	return p;
}



 
int ltPltDoTable(char *pIn,ltPltHtmBuf *psBuf,xmlNodePtr ltNodePtr,char *tablename)
{
    register int i;
    int iLoopEnd,iExist,iReturn,iLen,iStep,iHtmPos,iBLoop;
    char *p,*pValue;
    char sPre0[32];
    char caVarName[16],caVarName0[16];
    int  varIndex;    
    char *tempTableName;
    char *tempTableName1;    
    xmlNodePtr ltSubNodePtr;   
    int  loopNum;
    if(ltNodePtr==NULL){
		loopNum=0;
    }else{	
		if(tablename==NULL){
			loopNum=0;
		}else{
			loopNum= lt_dbget_LoopNum(ltNodePtr,tablename);
		}
    }
    if(loopNum>0){
	    for(i=1;i<loopNum+1;i++) {
	        p=pIn;
	        iLoopEnd = 0;
	        iHtmPos = psBuf->iLen;
		
	        while(!iLoopEnd) {
	           iReturn = ltPltGetStrToMark(p,psBuf,caVarName,LTPLT_MARKB,LTPLT_MARKE);
	           if(iReturn <= 0) {  /* ����  */
	                return iReturn;
	           }
	            p = p + iReturn;
		   /* Loop Begin */
	           if( strncmp(caVarName,LTPLT_LOOPB,10) == 0) {  
	                iBLoop = psBuf->iLen;
			tempTableName=strchr(caVarName,' ');
			tempTableName1=NULL;
			if(tempTableName!=NULL){
				tempTableName1=ltStrDelSpaces(tempTableName);
				if(strlen(tempTableName1)==0){
						tempTableName1=NULL;
				}
			}
			if(*p=='\n'){
				p=p+1;
			}
			//p = p + iReturn;
			if(ltNodePtr==NULL){
				ltSubNodePtr=NULL;
			}else{	
				if(tablename!=NULL){
					ltSubNodePtr=lt_dbget_table(ltNodePtr,tablename);		
				}else{
					ltSubNodePtr=NULL;
				}
			}
	
	                iReturn = ltPltDoTable(p,psBuf,ltSubNodePtr,tempTableName1);
	
	                //if(psBuf->iLen > iBLoop)
	                //    iExist ++;
	                
	                if(iReturn > 0)
	                    p = p + iReturn;
	                else 
	                    return iReturn;
	           }
		   /* Loop End */
	           else if( strncmp(caVarName,LTPLT_LOOPE,8)==0) {  
	                //iReturn = p - pIn;
			if(*p=='\n'){
				p=p+1;
			 }
	                iLoopEnd = 1;
	           }
	           else {  /* ����  */
			/*�õ�recordֵ*/
			if(ltNodePtr==NULL){
				pValue=NULL;
			}else{	
				if(tablename==NULL){
					pValue=NULL;
				}else{
					
		                	pValue = lt_dbget_LoopVar(ltNodePtr,tablename,caVarName,i);
		                	
				}
			}
			if(pValue) {   /* �����������  */
	                    iLen = strlen(pValue);
	                    if(psBuf->iLen + iLen > psBuf->iMaxBytes) {
	                        if(iLen < LTPLT_ALLOCSTEP) 
	                            iStep = LTPLT_ALLOCSTEP;
	                        else {
	                            iStep = iLen;
	                        }
	                        psBuf->pBuffer = realloc(psBuf->pBuffer,psBuf->iMaxBytes +iStep);
	                        if(psBuf->pBuffer == NULL) {
	                              return (-1);
	                        }
	                        psBuf->iMaxBytes = psBuf->iMaxBytes + iStep;
	                    }
	                    memcpy(psBuf->pBuffer + psBuf->iLen,pValue,iLen);
	                    psBuf->iLen = psBuf->iLen + iLen;
	                }
	            }
	        }
		/*exit while*/
		/* ѭ�������ޱ���  */
		/* if(iExist == 0 ) { 
	           psBuf->iLen = iHtmPos;
	           psBuf->pBuffer[iHtmPos] = 0;
	        }*/
     	}
	/*exit for*/
     }
     else{
	/*loopNum=0*/
		p=pIn;
	        iLoopEnd = 0;
	        iHtmPos = psBuf->iLen;
		
	        while(!iLoopEnd) {
	           iReturn = ltPltGetStrToMark(p,psBuf,caVarName,LTPLT_MARKB,LTPLT_MARKE);
	           if(iReturn <= 0) {  /* ����  */
	                return iReturn;
	           }
	            p = p + iReturn;
		   /* Loop Begin */
	          if( strncmp(caVarName,LTPLT_LOOPB,10) == 0) {  
	                iBLoop = psBuf->iLen;
			tempTableName=strchr(caVarName,' ');
			tempTableName1=NULL;
			if(tempTableName!=NULL){
				tempTableName1=ltStrDelSpaces(tempTableName);
				if(strlen(tempTableName1)==0){
						tempTableName1=NULL;
				}
			}
			if(*p=='\n'){
				p=p+1;
			 }
			//p = p + iReturn;
			if(ltNodePtr==NULL){
				ltSubNodePtr=NULL;
			}else{	
				if(tablename!=NULL){
					ltSubNodePtr=lt_dbget_table(ltNodePtr,tablename);		
				}else{
					ltSubNodePtr=NULL;
				}
			}
	
	                iReturn = ltPltDoTable(p,psBuf,ltSubNodePtr,tempTableName1);
	
	                //if(psBuf->iLen > iBLoop)
	                //    iExist ++;
	                
	                if(iReturn > 0)
	                    p = p + iReturn;
	                else 
	                    return iReturn;
	           }
		   /* Loop End */
	           else if( strncmp(caVarName,LTPLT_LOOPE,8)==0) {  
	                //iReturn = p - pIn;
			if(*p=='\n'){
				p=p+1;
			 }
	                iLoopEnd = 1;
	           }
	           else {  /* ����  */
			
	            }
	        }
		//psBuf->iLen = iHtmPos;
	       // psBuf->pBuffer[iHtmPos] = 0;
     }
     iReturn = p - pIn; 
     return iReturn;

}       
