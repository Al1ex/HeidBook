#ifndef LTDEFCGI_DEF
#define LTDEFCGI_DEF

#define LT_CGI_MAXSERVER      8
#define LT_CGI_SERVER_NAME    0
#define LT_CGI_USER_AGENT     1
#define LT_CGI_SERVER_PORT    2
#define LT_CGI_HTTP_REFERER   3
#define LT_CGI_REMOTE_ADDR    4
#define LT_CGI_REQUEST_URI    5
#define LT_CGI_REMOTE_USER    6
#define LT_CGI_AUTH_TYPE      7
#define LT_CGI_REMOTE_PORT    8
#define LT_CGI_REMOTE_IDENT   9

#define LT_CGI_MAX_FILE      10240000

typedef struct{
  char  *name;             /* ����  */
  char  *val;              /* ֵ    */
}ltCgiEntry;


typedef struct{
  char  *name;             /* ����  */
  char  *val;              /* ֵ    */
  int   iLen;              /* ����  */
  int   iType;             /* ����  0--һ�����  1--�ļ��ַ�   2---�ļ��ṹ  */
}ltCgiEntryU;


typedef struct{
    char   ipAddr[24];    		/* IP��ַ      */
    int    nPort;         		/* �˿ں�      */
    long   lTimeOut;                  /* ��ʱʱ��                */
    char   upLoadPath[256]; 	/* �����ļ���ŵ�Ŀ¼      */
    long   lMsgCode;                  /* ��Ϣ��ʶ                */
    long   lMaxMsgPkg;
}ltCgiConfInfo;
     
typedef struct{
    ltCgiConfInfo  *sCgiConf;         /* �����ļ���Ϣ  */
    int    iSumVar;                  /* ��������    */
    int    iSumCookies;              /* ���Cookies   */
    ltCgiEntry *entries;           /* ������Ϣ      */
    ltCgiEntry *cookies;           /* Cookies��Ϣ   */
}ltFcgiHead;


char *ltCgiMakeWord(char *line, char stop);
char ltCgiX2C(char *what);
void ltCgiUnEscapeUrl(char *url);
void ltCgiPlusToSpace(char *str);
ltCgiEntry *ltCgiRetrieveArgs(int *iSumVar);
ltCgiEntry *ltCgiRetrieveCookie(char *cpGetCookie,int *iSum);
char *ltCgiMsgErr(char *errorMsg);
int ltCgiGetIpPort(char *caIp,long *lIpAddress,short *nPort);
int ltCgiFreeHead(ltFcgiHead *psCgiHead);
int ltCgiConnect(ltCgiConfInfo *psCgiConf);
int ltCgiPrintf(char *pFormat,...);
int ltCgiFreeEntries(ltCgiEntry *psEntries,int iSum);

char *ltCgiMakeWord(char *line, char stop);
char *ltCgiFmakeWord(FILE *fp, char stop, int *length);
char ltCgiX2c(char *what);
void ltCgiUnEscapeUrl(char *url);
void ltCgiPlusToSpace(char *str);
ltCgiEntry *ltCgiRetrieveArgs(int *iSumVar);
ltCgiEntry *ltCgiRetrieveCookie(char *cpGetCookie, int *iSum);
char *ltCgiMsgErr(char *);
int ltCgiFreeEntries(ltCgiEntry *psEntries,int iSum);
int ltCgiPrintf(char *pFormat,...);
int ltCgiDo(ltCgiConfInfo *psCgiConf,char *curPath);
int ltCgiSendEnvTo(ltMsgHead **psMsgHead);


/*  ltocgi02.c  */
ltCgiEntryU *ltCgiUpLoad(FILE *fp,char *pTempPath,int *iSumVar,int *err);
int ltCgiFreeEntriesU(ltCgiEntryU *psEntries,int iSum);
int ltCgiFgets(char *pBuffer,int iMaxLen,int *iReadLen,FILE *fp);
int ltCgiReadUntilF(FILE *fpi,FILE *fpo,char *pTerm,int iLen);
int ltCgiReadUntilS(FILE *fpi,char *pBuffer,int *iReadLen,char *pTerm,int iLen);
char *ltCgiReadUntilSP(FILE *fpi,int *iReadLen,char *pTerm,int iLen);
int ltCgiFReadHead(FILE *fp,char *pBuffer);
int ltCgiAnalyLine(char *pBuffer,char *caVarName,char *caFileName,char *caContentType);
int ltCgiCvtSaveFile(char *caFileName,char *pTempPath,char *caSaveName);
int ltCgiDoUpload(ltCgiConfInfo *psCgiConf,char *curPage);
int ltCgiDoSlow(ltCgiConfInfo *psCgiConf,char *curPath);

#endif
