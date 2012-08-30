/* ltmsg.h
     Head File of Message Routine 
 */
#ifndef  __LTMSGPKDEF__
#define  __LTMSGPKDEF__

#include "lttype.h"

#define LT_MSG_ALLOCSTEP    256
#define LT_MSG_VARLEN       16
#define LT_MSG_MAXVARLEN         128

#define LT_MSG_VERSION      1000


#define LT_MSG_HOSTORDER       0
#define LT_MSG_HEADNORDER      1
#define LT_MSG_CONTENTNORDER   2
#define LT_MSG_NETWORKORDER    3

/* ltMsgHead: cFlags  */
#define LT_MSG_NORMALMSG       0
#define LT_MSG_ONLYHEADMSG     1
#define LT_MSG_NOTALLMSG       2

#define LT_MSG_NOCRYPT      	 0
#define LT_CRYPT_DES           1
#define LT_CRYPT_S             2




/* Message Head */
typedef struct {
    uint32   lCode;       /*  Message Code                        */
    uint32   lBytes;      /*  Message Bytes include the head      */
    uint32   lMsgId;      /*  lMsgId                              */
    uint32   lSendTime;   /*  Send Time                           */
    uint32   lCheckSum;   /*  Check Sum                           */
    uint16   nVersion;    /*  Message Version                     */
    uint8   cCrypt;      /*  0-- No Crypt 1--DES                       */
    uint8   cOrder;      /*  0-- Host Order  1-- Head Network Order  2---All NetWork Order */
    uint32   comFlag;     /*  ͨ�������ź� 100 msgHead OK; 200 msgBody ok  */
    uint32   lFunCode;    /* ���ܴ���  */
    uint32   lMaxBytes;
    uint8   msgpktype;   /*0 xml����1 html content��    2 ��������*/ 
    uint32   sid;        /*ϵͳ��sessionId*/
    uint32   rid;        /*ϵͳ�ı�����*/
} ltMsgHead;



/*  
ͨ�������ź� 
100 msgHead OK; 
200 msgBody ok  

11 too big msgpk
12 error msgpk

*/


typedef struct {
    uint16   nVarLen;
    uint16   nType;
    uint32   lBytes;
    char    caVar[LT_MSG_MAXVARLEN];
} ltMsgVar;


   
/* ltomsg01.c  */
void ltMsgSetValidCode(long lMsgCode);
int ltMsgIsValidCode(long lMsgCode);

long  ltMsgGetCode();
ltMsgHead *ltMsgInit(unsigned long  lCode,
                     unsigned long  lFunCode,
                     unsigned long  lMsgId,
                     unsigned long  lMaxBytes);
int ltMsgAdd_s(ltMsgHead **psMsgHead,char *pVarName,char *pVarValue);
int ltMsgAdd_v(ltMsgHead **psMsgHead,char *pVarName,char *pVarValue, 
                    unsigned long lLen);
int ltMsgAdd_l(ltMsgHead **psMsgHead,char *pVarName, long lVarValue);
int ltMsgAdd_n(ltMsgHead **psMsgHead,char *pVarName,short nVarValue);
int ltMsgHton(ltMsgHead *psMsgHead);
int ltMsgNtoh(ltMsgHead *psMsgHead);
int ltMsgHton0(ltMsgHead *psMsgHead);
int ltMsgHton1(char *pContent, unsigned long lBytes);
int ltMsgNtoh1(char *pContent,unsigned long lBytes);
int ltMsgNtoh0(ltMsgHead *psMsgHead);
char *ltMsgGetVar_s(ltMsgHead *psMsgHead,char *pVarName);
char *ltMsgGetVar_v(ltMsgHead *psMsgHead,char *pVarName,
        unsigned long *lBytes);
long ltMsgGetVar_l(ltMsgHead *psMsgHead,char *pVarName);
short ltMsgGetVar_n(ltMsgHead *psMsgHead,char *pVarName);
int ltMsgReset(ltMsgHead *psMsgHead,unsigned long  lCode,
                     unsigned long  nFunCode,
                     unsigned long  lMsgId);
void ltMsgFree(ltMsgHead *psMsgHead);


int ltMsgGetThatVar(ltMsgHead *psMsgHead,
            ltMsgVar *psMsgVar,char *pValue,short iNum);
int ltMsgAllRecord(ltMsgHead *psMsgHead,char *pHead,
        int ltMsgDoRecord(char *pHead,char *pVar,short nType,long lLen,char *Value));
int ltMsgPrint(char *pHead,char *pVar,short nType,long lLen,char *pValue); 
int ltMsgPrintMsg(ltMsgHead *psMsgHead);
int ltMsgGetSomeVar(ltMsgHead *psMsgHead,int iNumVar,...);
int ltMsgGetVar(ltMsgHead *psMsgHead,
            ltMsgVar *psMsgVar,char *pValue,char *pVarName);
int ltMsgIsConfirmMsg(long lMsgCode);
int ltMsgGetRecord(ltMsgHead *psMsgHead,int iNumVar,...);
int ltMsgAddSomeVar(ltMsgHead **psMsgHead,int iNumVar,...);
int ltMsgFPrintMsg(ltMsgHead *psMsgHead,char *p);

/*���ݰ�������У��*/

/* �� psMsgHead����У������  */
int ltMsgSetCheckSum(ltMsgHead *psMsgHead,char *pKey);
ltMsgHead *ltMsgInitN(unsigned long  lCode,
                     char *pFunName,
                     unsigned long  lMsgId,
                     unsigned long  lMaxBytes);
/* ������ݰ���CheckSum
    ����: 0---����ȷ
          1---��ȷ
 */
int ltMsgCheckSum(ltMsgHead *psMsgHead,char *pKey);
/* �������ݰ�

 */
int ltMsgDesEncrypt(ltMsgHead **psMsgHead0,char *pKey);
/* ����  */
int ltMsgDesDecrypt(ltMsgHead **psMsgHead0,char *pKey);

int ltMsgSDecrypt(ltMsgHead *psMsgHead);
int ltMsgSEncrypt(ltMsgHead *psMsgHead);



#endif

