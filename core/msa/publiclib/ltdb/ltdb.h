#ifndef  _LT_DB_
#define _LT_DB_

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifdef  _LT_DB_MYSQL_ 

#include "mysql.h"
typedef MYSQL ltDbConn;
typedef MYSQL_ROW  LT_DBROW;
typedef MYSQL_RES  DB_CURSOR;
typedef char**	   LT_DBFIELD;
typedef struct{
	DB_CURSOR *db_cursor;
	LT_DBROW db_row;
	unsigned int num_field;
	LT_DBFIELD db_field;
} ltDbCursor;

#endif

#ifdef  _LT_DB_POSTGRESQL_ 

#include "libpq-fe.h"
typedef PGconn ltDbConn;
typedef char **LT_DBROW;
typedef char**	   LT_DBFIELD;
typedef struct{
  ltDbConn *db_conn;
	PGresult *db_cursor;
	LT_DBROW db_row;
	unsigned int num_field;
	LT_DBFIELD   db_field;
	char     cursorname[48];
	unsigned int nowline;
	unsigned int maxline;
} ltDbCursor;

#endif

#ifdef  _LT_DB_ORACLE_ 

#include "ltoci.h"
typedef DB_CONNECT ltDbConn;
typedef char **LT_DBROW;
//typedef MYSQL_RES  DB_CURSOR;
typedef char**	   LT_DBFIELD;
typedef struct{
	DB_CURSOR *db_cursor;
	LT_DBROW db_row;
	unsigned int num_field;
	LT_DBFIELD db_field;
} ltDbCursor;

#endif


/*���ݿ�����*/
ltDbConn *ltDbConnect(char *pUser,char *pPassword,char *pService);
/* �ر����ݿ����ӣ����ͷ��й���Դ  */
void  ltDbClose(ltDbConn *psConn);



/* ���α�, ʹ�ô���ʽ����  */
//ltDbCursor *ltDbOpenCursor(ltDbConn *pConn,  char *pSmt,...);
ltDbCursor *ltDbOpenCursorS(ltDbConn *pConn,  char *pSmt);
ltDbCursor *ltDbOpenCursor(ltDbConn *pConn,  char *pSmt);
void ltDbCloseCursor(ltDbCursor *ltCursor);



/*����fetch����*/
/*����fetch����*/
unsigned  int ltNumField(ltDbCursor *ltCursor);
/*�õ��ֶ���*/
LT_DBFIELD  ltDbFetchField(ltDbCursor *ltCursor);
/*fetch���õ��ֶ�ֵ*/
LT_DBROW  ltDbFetchRow(ltDbCursor *ltCursor);
/*type, len, arg*/
//int ltDbFetchInto(ltDbCursor *ltCursor,...);


/*������¼����*/
//LT_DBROW ltDbOneRow(ltDbConn *pConn,int *fieldnum,char *caSmt);
//void ltDbFreeRow(LT_DBROW dbRow);
///*type, len, arg*/
//int ltDbOneRowTo(ltDbConn *pConn,char *pSmt,...);


/*���²���*/
int ltDbExecSql(ltDbConn *pConn,  char *pSmt);
//int ltDbExecSqlS(ltDbConn *pConn,  char *pSmt);
//unsigned int ltDbAffectRows(ltDbConn *pConn);

///*�������*/
//int ltDbCommit(ltDbConn *pConn);
//int ltDbRollback(ltDbConn *pConn);
//
//
//
///*������*/
//unsigned int  ltDbErrorCode(ltDbConn *pConn);
//char *ltDbError(ltDbConn *pConn);

#endif
