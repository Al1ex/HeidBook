#ifndef _LT_PLT_
#define _LT_PLT_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "libxml/tree.h"
#include "libxml/parser.h"
#include "ltmsg.h"

struct ltDb{
		xmlDocPtr doc;	
        xmlNodePtr bodyPtr;
		char       *head;
};

typedef struct ltDb* ltDbHeadPtr;   
typedef xmlNodePtr ltRecordPtr;
typedef xmlNodePtr ltTablePtr;

/*��ʼ������ģ��ָ��*/
ltDbHeadPtr lt_dbinit();

/*����head����*/
int lt_dbput_head(ltDbHeadPtr dbPtr,char *value);

/*�����ӽڵ����*/
int lt_dbput_rootvar(ltDbHeadPtr dbPtr,char *name,char *value);

/*һ�����Ӷ���ӽڵ����*/
int lt_dbput_rootvars(ltDbHeadPtr dbPtr,int varnum,...);

/*��ӡ����dbhead�ṹ*/
char *lt_db_print(ltDbHeadPtr dbPtr);

/*�õ�����dbheadָ��*/
char *lt_db_mem(ltDbHeadPtr dbPtr);

/*�ظ�dbhead����ģ��*/
int lt_db_response(int iSock,ltMsgHead *psMsgHead0,ltDbHeadPtr dbPtr);

/*�õ�dbheadͷ*/
char *lt_dbget_head(xmlDocPtr doc);
/*�õ�charsetͷ*/
char *lt_dbget_charset(xmlDocPtr doc);
/*�õ�dbhead������ֵ*/
char *lt_dbget_Var(xmlNodePtr nodePtr,char *varName);

/*�õ�ѭ������ֵ*/
char *lt_dbget_LoopVar(xmlNodePtr node,char *tablename,char *varName,int index);

/*����cookie����*/
int lt_db_setcookie(ltDbHeadPtr dbPtr,const char *name, const char *value);
/*����cookie����1*/
int lt_db_setcookieA(ltDbHeadPtr dbPtr,const char *name, const char *value,
        char *pPath,char *pDomain,char *pExpire);
/*ɾ��cookie����*/
int lt_db_delcookie(ltDbHeadPtr dbPtr,const char *name);

/*����webҳ�޻���*/
int lt_db_nocache(ltDbHeadPtr dbPtr);
int lt_web_nocachemsie(ltDbHeadPtr dbPtr);

/*�ڸ��ϼ�һ��table*/ 
ltTablePtr lt_dbput_table(ltDbHeadPtr dbPtr,char *tablename);

/*��table�ϼ�һ��������recordֵ*/
int lt_dbput_recordvars(ltTablePtr tablePtr,int varnum,...);

/*��table�ϼ�һ��table*/ 
ltTablePtr lt_dbput_subtable(ltTablePtr tablePtr,char *tablename);

/*��table�¼�record*/ 
ltRecordPtr lt_dbput_record(ltTablePtr tablePtr);

/*��record�¼�ֵ*/
int lt_dbput_recordvar(ltRecordPtr rsPtr,char *name,char *value);

xmlNodePtr lt_dbget_body(xmlDocPtr doc);
xmlNodePtr lt_dbget_table(xmlNodePtr tableParent,char  *tableName);
int lt_dbget_LoopNum(xmlNodePtr parNode,char *tablename);

/*pFile ģ��ָ�� ltDbBuf xmlģ������*/
char *ltPltFileParse(char *pFile,xmlDocPtr ltDocPtr,int chSet);
/* ��ģ��ת��Ϊ HTML�ַ���
       pIn      ---- ģ�����ݵ��ַ���
       ltDbBuf ---- ģ������xml�ṹ
   ����:
       ���ɵ� HTML�ַ���

 */
char *ltPltMemParse(char *pIn,xmlDocPtr ltDocPtr);

int lt_db_htmlpage(ltDbHeadPtr dbPtr,char *charset);

int lt_html_response(int iSock,ltMsgHead *psMsgHead0,char *htmlHead,char *htmlContent);
int lt_download(int iSock,ltMsgHead *psMsgHead0,char *fileurl,char *filename,char *contype);
int lt_downloadA(int iSock,ltMsgHead *psMsgHead0,char *con,int consize,char *filename,char *contype);
int lt_db_redirect(int iSock,ltMsgHead *psMsgHead0,const char *buf);
int lt_dbput_html(int iSock,ltMsgHead *msgPk,xmlDocPtr xDoc,char *pltPageUrl,char *head,int chSet);
void lt_dbfree(ltDbHeadPtr dbPtr);

#endif


