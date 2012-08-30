#ifndef __ICMDEF__
#define __ICMDEF__


#include <netinet/ether.h>
#include "pubfun.h"

/*���Ų�μ���*/
#define _MAX_GROUPLEVEL   5
/*ȫ����������*/
#define _MAX_ALLGRPNUM     200
#define _MAX_ALLPOLICY     60
#define _MAXSNMPNUM        7

/*ϵͳ��������*/
#define NAS_MAX_COREFUNNUM    200
#define NAS_MAX_FUNNUM        200
#define NAS_DEFAULT_FUNTIME   900


/*����Ա����*/
#define MAX_ADMIN_SESSION            300
#define MAX_ADMIN_TIMEOUT            7200
#define MAX_ADMIN_TRYTIMEOUT         120
#define MAX_ADMIN_MAXRIGHT           100

#define MSAS_RIGHT_SYSADMIN          5

/*�Զ����ַ���*/
#define MAX_SYSTEM_MSG               1000
/*�256*/
#define MAX_LANG_LIST_CONSIZE        256


#define _pubconfdir   "/etc/msa/icm"
#define _datacenterdir   "/datacenter/icm"
#define _proccessdir   "/datacenter/proccess"
#define _TMPDIR   "/datacenter/tmp"


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
	char           lright[MAX_ADMIN_MAXRIGHT];     /* Ȩ��       */
	char           caname[64];     /* Ȩ��       */
  char           allgroup[_MAX_ALLGRPNUM];   /* ������   */
  unsigned int   trytimes;
	unsigned long  lasttime;   /*���һ�ε�½ʱ�� */
} nasAdmSession;

/*
0:chinese
1:en
*/
typedef struct conListType_s {
    char            conStr[MAX_LANG_LIST_CONSIZE];
} conListType;

/*
hash ���ڷ����������ͻ������
freecon ���������취,���һ���ı��Ϊnext=-1
*/
typedef struct _shm_hash_link_onlineuserhead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    int                freecontenthead;
    int                freecontenttail;
    int                locksemid;
    int                nownum;
} shm_hash_link_onlineuserhead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_onlineuserindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_onlineuserindex;


typedef struct icmOnlineUser_s { 
	  uint32   starttime;
	  uint32   lasttime;  
	  uint32   ipadd;                /* ͨ�ŵ�ip��ַ*/
	  unsigned char  caMac[6];
	  unsigned int   userid;
    char     username[32];          /* ����      */
    char     dispname[32];          /* ����      */
    uint16   levelgroup[_MAX_GROUPLEVEL];        /*5�������*/
    uint8    policylist[_MAX_ALLPOLICY];         /**/
    uint8    userflag[32];
} icmOnlineUser;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_onlineuser {
    int           next;
    icmOnlineUser   item;
} shm_hash_link_onlineuser;


typedef struct _shm_hash_link_onlinearpiphead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    int                freecontenthead;
    int                freecontenttail;
    int                locksemid;
    int                nownum;
} shm_hash_link_onlinearpiphead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_onlinearpipindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_onlinearpipindex;

typedef struct   arpip_s {
    unsigned int   ipadd;
    unsigned char  caMac[6];
    unsigned int   requestnum;
    unsigned int   lastrequesttime;
    unsigned int   requestednum;
    unsigned int   lastrequestedtime;
    unsigned char  flag;  /*0 ����  1��ֹ�����  2��ip/mac�����  */
} icmArpIp;

typedef struct _shm_hash_link_onlinearpip {
    int           next;
    icmArpIp      item;
} shm_hash_link_onlinearpip;


typedef struct icmIpHostInfo_s { /* IP��ַ����Ϣ    */
    unsigned long   lIp;       
    unsigned char   caMac[6]; 
    unsigned char   iFlags;    /*  �û�ӳ��󶨣�  1--ip/mac�ֶ���  2:ip/mac�� dhcp  3:dhcp �����֤  8:��ip/mac����  99--���� 100��ֹ������� */
    unsigned char   snmpnum;       /* snmp group */
} icmIpHostInfo;

typedef struct icmIpHostInfo_Head_s {
    unsigned int       bufnum;
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/
    icmIpHostInfo      *item;
} icmIpHostInfo_Head;

typedef struct icmMacFaceInfo_s {
	unsigned int  vlanid;
	unsigned int  ip;
	unsigned int  mask;
	char          dev[12];
	unsigned char caMac[6]; 
}icmMacFaceInfo;

typedef struct icmPubInfo_s {
	  unsigned int  lRunning;
	  char          _dbname[128];
	  char          _dbuser[128];
	  char          _dbpass[128];
	  int           sysActive;
	  int           maxfuntonnum;
	  int           icmGeneralHour;
	  nasAdmSession      admSession[MAX_ADMIN_SESSION];
	  conListType        pubMsgStr[MAX_SYSTEM_MSG];
	  int                defaultLan;    /*Ĭ������*/ 
	  icmMacFaceInfo     _macFace[4096]; 
	  unsigned   int     _maxMacFaceNum;
	  shm_hash_link_onlineuserhead  _onlineuserhead;   
	  shm_hash_link_onlinearpiphead  _onlinearpiphead; 
	  icmIpHostInfo_Head  _icmIpHostInfoHead; 
	  unsigned   int     lIpHostNum;
	  unsigned   int     arprequestalertnum;
	  unsigned   int     arprequestedalertnum;
}icmPubInfo;

icmPubInfo  *_ltPubInfo;

ltDbConn     *G_DbCon; /*������������̵����ݿ�����*/

typedef struct msasDir_s {
    char   caDirName[128];
    char   caDirPath[256];
    int    dirTime;    
} msasDir;

typedef struct jiffy_counts_t {
	unsigned long long usr,nic,sys,idle,iowait,irq,softirq,steal;
	unsigned long long total;
	unsigned long long busy;
} jiffy_counts_t;


lt_shmHead *icmcreateShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
lt_shmHead *icmopenShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
int icmcloseShmMem(lt_shmHead *lt_MMHead);

/*nasPubUtil.c*/
int icmInitPubInfo(lt_shmHead *lt_MMHead);
int ltWebMsgErr(char *errorMsg,int confd,ltMsgHead *ltMsgPk);
int icmInitPubVar(lt_shmHead *lt_MMHead);
int logWrite(ltMsgHead *ltMsgPk,char *msg,int msgtype);
int nasTimeFormat(char *pFormat,long lTime);
int checkRight(int confd,ltMsgHead *ltMsgPk,int right,lt_shmHead *lt_MMHead);
int strgb2utf8(const char *cpInput,char *cpOut,int iInputLen);
int pubGetName(char *caKey,int start,int end,char *caGetName);
int dirtimecmp(msasDir* a,msasDir* b);
msasDir* mydirlist(char* dir,int* len);
int nasTimeGetDate(char *cDate,long lTime);
unsigned long nasCvtLongTime(char *caSdate,char *caTime);
int nasTimeLongFormat(char *pFormat,long lTime);
int nasCvtStime(unsigned long lTime,char *caDate,char *caTime);
char *nasCvtMac(unsigned char *caMac,char *caMacStr);
int nasCvtMacI(unsigned char *caMacStr,unsigned char *caMac);
int icmReportList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmDataCenetrDownLoad(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*icmUserList.c*/
int icmPrintHashUser(icmPubInfo  *ltPubInfo);
int icmDeleteUser(icmPubInfo  *ltPubInfo,unsigned long lip);
icmOnlineUser *icmHashLookUpUser(icmPubInfo  *ltPubInfo,unsigned long lip);
icmOnlineUser * icmHashInsertUser(icmPubInfo  *ltPubInfo,unsigned long k, icmOnlineUser *item);
int icmInitUserListHash(icmPubInfo  *ltPubInfo);
int icmPrintFreeHashUser(icmPubInfo  *ltPubInfo);

/*icmArpIpList.c*/
int icmInitArpIpListHash(icmPubInfo  *ltPubInfo);
icmArpIp * icmHashInsertArpIp(icmPubInfo  *ltPubInfo,unsigned long k, icmArpIp *item);
icmArpIp *icmHashLookUpArpIp(icmPubInfo  *ltPubInfo,unsigned long lip);
int icmDeleteArpIp(icmPubInfo  *ltPubInfo,unsigned long lip);
int icmPrintHashArpIp(icmPubInfo  *ltPubInfo);
int icmPrintFreeHashArpIp(icmPubInfo  *ltPubInfo);

/*icmIpListInfo.c*/
int icmInitIpListInfo(icmPubInfo  *ltPubInfo);


/*srv-admin.c*/
int icmShowAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmAdminLogout(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmShowTopview(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);


/*srv-now.c*/
int icmShowXTZY(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmZaixianInfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmShowUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmReadArpIp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmShowArpIpList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmReadArpIpText(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-group.c*/
int icmlooparea(stralloc *strTemp,int level,int pid);
int icmGroupTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmGroupAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmShowClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-manager.c*/
int icmAdminUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmAdminAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmAdmuserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmCheckOutAdmin(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmAdmlog(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmSysReset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmSysShutdown(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmCleanAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmStatusAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int ltnetareaaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int ltnetarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int ltlanarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int ltlanaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int ltdhcpaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int ltdhcparealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int icmVLanListAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmAddVlanAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmDeleteAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
/*srv-users*/
int icmPolicyTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmShowAddClient(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmTerminfoAdd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmTermInfoView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
/*srv-data*/
int icmSystemListSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmCreateReport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int icmArpListSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead); 
#endif


