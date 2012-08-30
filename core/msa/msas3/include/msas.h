#ifndef __MSASDEF__
#define __MSASDEF__


#include <netinet/ether.h>
#include "pubfun.h"

#define NAS_MAX_COREFUNNUM    500
#define NAS_MAX_FUNNUM        500
#define NAS_DEFAULT_FUNTIME   900

char *pubconfdir;

/*ע�ắ����*/
typedef struct corefunList_s{
	char strFunName[64]; /*��������*/
  int (*op)();
} corefunList;



typedef struct funList_s{
	unsigned int lFunCode;  /*����*/
	unsigned int funFlag;   /*0 ֱ�ӵ��� 1 dll��̬���� 2java�� 3Ӧ�ó������*/
	char strNode[32]; /*����˵��*/
	char strFunName[64]; /*��������*/
	char strFunUrl[128]; /*dll���õ�url*/
	unsigned int  maxruntime;    /*�ú��������ʱ��*/
	unsigned int  activeflag;    /*��������� 0�뼤���޹� 1�뼤����� */
	unsigned int  rightflag;    /*Ȩ�������  0��Ȩ���޹� 1��Ȩ�����  */
  int (*op)();
} funList;

corefunList  _ltcorefunList[NAS_MAX_COREFUNNUM];
funList      _ltfunList[NAS_MAX_FUNNUM];
int          _maxfuntonnum;

typedef struct nasAdmSession_s {
	unsigned long  lSip;       /* ԴIP��ַ     */
	unsigned long  lSid;       /* session id  4λ����  */
	char           lright[100];     /* Ȩ��       */
	char           caname[64];     /* Ȩ��       */
  int            allgroup;   /* ������   */
  unsigned int   trytimes;
	unsigned long  lasttime;   /*���һ�ε�½ʱ�� */
} nasAdmSession;

#define MAX_LANG_LIST_CONSIZE        256
#define MAX_SYSTEM_MSG               500
/*ÿ�仰���512�ֽ�*/
/*
0:chinese
1:en
*/
typedef struct conListType_s {
    char            conStr[MAX_LANG_LIST_CONSIZE];
} conListType;


typedef struct msasPubInfo_s {
	  unsigned int  lRunning;
	  char          _dbname[128];
	  char          _dbuser[128];
	  char          _dbpass[128];
	  int           sysActive;
	  int           maxfuntonnum;
	  nasAdmSession      admSession[20];
	  conListType        pubMsgStr[MAX_SYSTEM_MSG];
	  int           defaultLan;    /*Ĭ������*/      
}msasPubInfo;

msasPubInfo  *_ltPubInfo;

ltDbConn     *G_DbCon; /*������������̵����ݿ�����*/

lt_shmHead *msascreateShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
lt_shmHead *msasopenShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
int msascloseShmMem(lt_shmHead *lt_MMHead);

/*nasPubUtil.c*/
int msasInitPubInfo(lt_shmHead *lt_MMHead);
int ltWebMsgErr(char *errorMsg,int confd,ltMsgHead *ltMsgPk);
int msasInitPubVar(lt_shmHead *lt_MMHead);

/*srv-test.c*/
//int ErrorTest(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTestApp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTestApp1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*Example.c*/
int msasAddAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowRulePage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAnjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAnjianTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAnJian(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleListAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertRuleAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleTypeAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowanjianPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAddanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasUpdateanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
   

/*srv-test2.c*/
#endif


