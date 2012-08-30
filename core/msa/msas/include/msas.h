#ifndef __MSASDEF__
#define __MSASDEF__


#include <netinet/ether.h>
#include "pubfun.h"

/*���Ų�μ���*/
#define MSAS_MAX_GROUPLEVEL   5
/*ȫ����������*/
#define NAS_MAX_ALLGRPNUM     1000
/*ϵͳ��������*/
#define NAS_MAX_COREFUNNUM    500
#define NAS_MAX_FUNNUM        500
#define NAS_DEFAULT_FUNTIME   900

#define COMM_MAX_COREFUNNUM   50

/*����Ա����*/
#define MAX_ADMIN_SESSION            300
#define MAX_ADMIN_TIMEOUT            7200
#define MAX_ADMIN_TRYTIMEOUT         120
#define MAX_ADMIN_MAXRIGHT           100

#define MSAS_RIGHT_SYSADMIN          5

#define MSAS_DEFAULT_ONLINEHOSTSTR   "500"

/*ϵͳ����һ��������*/
#define NAS_MAX_SYSSRVNUM        50

/*�Զ����ַ���*/
#define MAX_SYSTEM_MSG               1000
/*�256*/
#define MAX_LANG_LIST_CONSIZE        256
#define CTYPEINDEXSTART         15
#define CTYPEINDEXEND           37
#define CUNTRYINDEXSTART        38
#define CUNTRYINDEXEND          230
#define NETIDINDEXSTART         260
#define NETIDINDEXEND           369
#define ALERTTYPENAME           232
#define ALERTTYPENAMEEND        242
#define ALERTTYPENAME0          243

/*��ʱ�����ı�����Ŀ*/
#define MAXALERTNOW           200

#define pubconfdir   "/etc/msa/msas"
#define datacenterdir   "/datacenter/msas"
#define proccessdir   "/datacenter/proccess"
#define _TMPDIR   "/datacenter/tmp"

#define SERV_PORT 8888

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
  char            allgroup[NAS_MAX_ALLGRPNUM];   /* ������   */
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
typedef struct _shm_hash_link_onlinehosthead {
    unsigned int       bufptr;  /*��ͷ��ʼ��ƫ����*/
    unsigned int       bufsize;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       conbufptr;  /*��ͷ��ʼ��ƫ����*/ 
    unsigned int       bufnum;
    int                freecontenthead;
    int                freecontenttail;
    int                locksemid;
    int                nownum;
} shm_hash_link_onlinehosthead;

/*hash index ����index�ֲ�*/
typedef struct _shm_hash_link_onlinehostindex {
    int           next; /*-1�൱��NULL*/
} shm_hash_link_onlinehostindex;

typedef struct msasOnlineHostS_s {   
	  uint32   starttime;
	  uint32   lasttime;  
	  uint32   ipadd;                /* ͨ�ŵ�ip��ַ*/
    char     caName[128];          /* ����      */
    char     caKey[32];            /* ���к�    */
    uint32   sysversion;           /* ϵͳ�汾  */
    uint32   cpunum;               /* cpu������ */
    uint32   memnum;           	   /* mem������ */
    uint32   sysdisknum;           /* ϵͳӲ�������� */
    uint32   appdisknum;           /* Ӧ��Ӳ�������� */
    uint32   usernum;              /* �����û� */
    uint32   p2pnum;               /* p2p�û� */
    uint32   livernum;             /* live�û� */
    uint32   gamenum;              /* game�û� */
    uint32   soketnum;             /* ��Ʊ�û� */
    uint32   tcpnum;               /*tcp������*/
    uint32   udpnum;               /*udp������*/
    uint32   httpnum;              /* http������ */   
    uint32   urlnum;               /* ���ַ��*/
    uint64   bytenum;              /*��������*/ 
    uint64   bytenumu;             /*��������*/ 
    uint64   bytenumd;             /*��������*/ 
    uint16   levelgroup[MSAS_MAX_GROUPLEVEL]; 
} msasOnlineHostS;

typedef struct msasOnlineHostHour_s { 
	  char     chours; 
	  uint16   hourtimes;
    uint32   cpunum;               /* cpu������ */
    uint32   memnum;           	   /* mem������ */
    uint32   sysdisknum;           /* ϵͳӲ�������� */
    uint32   appdisknum;           /* Ӧ��Ӳ�������� */
    uint32   usernum;              /* �����û� */
    uint32   p2pnum;               /* p2p�û� */
    uint32   livernum;             /* live�û� */
    uint32   gamenum;              /* game�û� */
    uint32   soketnum;             /* ��Ʊ�û� */
    uint32   tcpnum;               /*tcp������*/
    uint32   udpnum;               /*udp������*/
    uint32   httpnum;              /* http������ */   
    uint32   urlnum;               /* ���ַ��*/
    uint64   bytenum;              /*��������*/ 
    uint64   bytenumu;             /*��������*/ 
    uint64   bytenumd;             /*��������*/
    unsigned long long srvflow[NAS_MAX_SYSSRVNUM];
} msasOnlineHostHour;

typedef struct msasOnlineHost_s { 
	  uint32   starttime;
	  uint32   lasttime;  
	  uint32   ipadd;                /* ͨ�ŵ�ip��ַ*/
    char     caName[128];          /* ����      */
    char     caKey[32];            /* ���к�    */
    uint32   sysversion;           /* ϵͳ�汾  */
    uint32   cpunum;               /* cpu������ */
    uint32   memnum;           	   /* mem������ */
    uint32   sysdisknum;           /* ϵͳӲ�������� */
    uint32   appdisknum;           /* Ӧ��Ӳ�������� */
    uint32   usernum;              /* �����û� */
    uint32   p2pnum;               /* p2p�û� */
    uint32   livernum;             /* live�û� */
    uint32   gamenum;              /* game�û� */
    uint32   soketnum;             /* ��Ʊ�û� */
    uint32   tcpnum;               /*tcp������*/
    uint32   udpnum;               /*udp������*/
    uint32   httpnum;              /* http������ */   
    uint32   urlnum;               /* ���ַ��*/
    uint64   bytenum;              /*��������*/ 
    uint64   bytenumu;             /*��������*/ 
    uint64   bytenumd;             /*��������*/ 
    uint16   levelgroup[MSAS_MAX_GROUPLEVEL];        /*5�������*/
    unsigned long long srvflow[NAS_MAX_SYSSRVNUM];
    uint8    cmdupdateflag;
    msasOnlineHostHour hourinfo;
    uint16   intervalfig;
    uint8    alarmlog;    /*�ϱ������Լ�������־*/
    uint8    trlog;       /*�ϱ��켣��¼*/
    uint8    onlinelog;   /*�ϱ���������ʼ�¼*/
    uint8    identitylog; /*�ϱ���ݼ�¼*/
    uint8    conlog;      /*�ϱ����ݼ�¼*/
    uint8    statlog;      /*�ϱ�ͳ�Ƽ�¼*/       
    uint16   uploadtimeval;       /*�ϴ�ʱ����*/
    uint8    vpnflag; 
    uint8    havecmd;       
} msasOnlineHost;

/*hash���ݶ���,���ݿ������*/
typedef struct _shm_hash_link_onlinehost {
    int           next;
    msasOnlineHost   item;
} shm_hash_link_onlinehost;

typedef struct jiffy_counts_t {
	unsigned long long usr,nic,sys,idle,iowait,irq,softirq,steal;
	unsigned long long total;
	unsigned long long busy;
} jiffy_counts_t;

typedef struct nasTopSrv_s {
	int           srvid;
	char          srvname[48];
}nasTopSrv;

typedef struct AlertList_s {
	int           Matching_id;
	char          gnmkey[32];
	char          srvname[128];
	char          Rule_id[32];
  char          anjian_id[32];
  char          Rule_name[64];
  char          Matching_time[32];
  char          Net_ending_ip[32];
  char          Net_ending_code[32];
  char          Net_ending_mac[16];
  char          Destination_ip[32];
  char          Service_type[16];
  char          KeyWord1[64];
  char          KeyWord2[32];
  char          KeyWord3[24];
  char          Customer_name[32];
  char          Certificate_type[12];
  char          Certificate_code[24];
  int           Rule_type;
  char          contenturl[128];
}AlertList;



typedef struct msasPubInfo_s {
	  unsigned int  lRunning;
	  char          _dbname[128];
	  char          _dbuser[128];
	  char          _dbpass[128];
	  int           sysActive;
	  int           maxfuntonnum;
	  int           msasGeneralHour;
	  nasAdmSession      admSession[MAX_ADMIN_SESSION];
	  conListType        pubMsgStr[MAX_SYSTEM_MSG];
	  int                defaultLan;    /*Ĭ������*/  
	  shm_hash_link_onlinehosthead  _onlinehosthead;   
	  unsigned long long syssrvflow[NAS_MAX_SYSSRVNUM];
	  nasTopSrv          topSrvName[NAS_MAX_SYSSRVNUM]; 
	  AlertList          _AlertList[MAXALERTNOW];
	  unsigned  int      AlertListIndex;
}msasPubInfo;

msasPubInfo  *_ltPubInfo;

ltDbConn     *G_DbCon; /*������������̵����ݿ�����*/

typedef struct msasDir_s {
    char   caDirName[128];
    char   caDirPath[256];
    int    dirTime;    
} msasDir;


lt_shmHead *msascreateShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
lt_shmHead *msasopenShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
int msascloseShmMem(lt_shmHead *lt_MMHead);

/*nasPubUtil.c*/
int msasInitPubInfo(lt_shmHead *lt_MMHead);
int ltWebMsgErr(char *errorMsg,int confd,ltMsgHead *ltMsgPk);
int msasInitPubVar(lt_shmHead *lt_MMHead);
int logWrite(ltMsgHead *ltMsgPk,char *msg,int msgtype);
int nasTimeFormat(char *pFormat,long lTime);
int checkRight(int confd,ltMsgHead *ltMsgPk,int right,lt_shmHead *lt_MMHead);
int strgb2utf8(const char *cpInput,char *cpOut,int iInputLen);
int msasGetName(char *caKey,int start,int end,char *caGetName);
int dirtimecmp(msasDir* a,msasDir* b);
msasDir* mydirlist(char* dir,int* len);
int msasDataCenetrDownLoad(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int nasTimeGetDate(char *cDate,long lTime);
unsigned long nasCvtLongTime(char *caSdate,char *caTime);
int msasReportList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int nasTimeLongFormat(char *pFormat,long lTime);
int nasCvtStime(unsigned long lTime,char *caDate,char *caTime);
int msasTestApp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-admin.c*/
int msasShowAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdminLogout(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaShowTopview(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int msasSysReset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSysShutdown(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int msasPrintHashHost(msasPubInfo  *ltPubInfo);
int msasDeleteHost(msasPubInfo  *ltPubInfo,unsigned long lip);
msasOnlineHost *msasHashLookUpHost(msasPubInfo  *ltPubInfo,unsigned long lip);
msasOnlineHost * msasHashInsertHost(msasPubInfo  *ltPubInfo,unsigned long k, msasOnlineHost *item);
int msasInitHostListHash(msasPubInfo  *ltPubInfo);
int msasPrintFreeHashHost(msasPubInfo  *ltPubInfo);

/*srv-group.c*/
int msasGroupTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasGroupAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAddClient(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTerminfoAdd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTermInfoUpdate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTermInfoView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdminUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdminAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasCheckOutAdmin(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdmuserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdmlog(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int vpnonlineuser(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int tovpnsetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int vpnsetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-now.c*/
int msasShowXTZY(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasZaixianInfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasHostList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSrvInfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowHostList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasGetAlertNow(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowClientSrv(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-data.c*/
int msasRealIdList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasNetIdList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasNetIdTrackList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasClientOnOff(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSearchTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAccessSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasIpbackSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasNewSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAccesssCmdSet(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasYunSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowYunSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasYunSearchCMDList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasYunSearchDel(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSearchShowList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasGetInfoByKey(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSearchShowListt(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShKeySearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShKeyList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int msasShowjson(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowYunSearchPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-rule.c*/
int msasAnjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleListAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertRuleAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleTypeAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAnjianTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAnJian(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowanjianPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAddanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasUpdateanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAddAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowRulePage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertRuleLsit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-report*/
int msasreportproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasreportproc1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);



#endif


