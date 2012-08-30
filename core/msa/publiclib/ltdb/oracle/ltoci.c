#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ltoci.h"

void _dbFreeDefineBuf(DB_CURSOR* pCursor);
int _dbAppendDefineBuf(DB_CURSOR* pCursor, DB_DESCR* pDescr);

/*///////////////////////////////////////////////////////////////
/////////////////////////Oracle8 APIs////////////////////////////
///////////////////////////////////////////////////////////////*/

void checkerr(OCIError *errhp, sword status);
void _dbsize2dsize(DB_DESCR* pDescr);
int _dbStr2Lower(char* str);

/*---------------------------------------------------------------
  �������ƣ�dbConnect()
  ����˵�����������ݿ�
  ����˵����[in]uid     -- ���ݿ��û���
            [in]passwd  -- ���ݿ�����
            [in]service -- ���ݿ������
  ����ֵ��  ���ɹ�  -- NULL
            �ɹ�    -- DB_CONNECT�ṹָ��
---------------------------------------------------------------*/
DB_CONNECT* dbConnect(char* uid, char* passwd, char* host)
{
    sword status;
    DB_CONNECT* pConn;

    if((pConn = malloc(sizeof(DB_CONNECT))) == NULL)
        return NULL;

    OCIInitialize((ub4) OCI_DEFAULT, (dvoid *)0,
        (dvoid* (*)(dvoid*, size_t))0,
        (dvoid* (*)(dvoid*, dvoid*, size_t))0,
        (void (*)(dvoid*, dvoid*))0 );

    OCIEnvInit((OCIEnv **)&pConn->pEnv, OCI_DEFAULT, (size_t)0, (dvoid**)0);

    OCIHandleAlloc((dvoid*)pConn->pEnv, (dvoid**)&pConn->pErr, OCI_HTYPE_ERROR, 
        (size_t)0, (dvoid**)0);  

    OCIHandleAlloc((dvoid*)pConn->pEnv, (dvoid**)&pConn->pSrv, OCI_HTYPE_SERVER,
        (size_t)0, (dvoid**)0);

    OCIHandleAlloc((dvoid*)pConn->pEnv, (dvoid**)&pConn->pSvc, OCI_HTYPE_SVCCTX,
        (size_t)0, (dvoid**)0);

    if(host == NULL)
        OCIServerAttach(pConn->pSrv, pConn->pErr, (text*)host, 0, 0);
    else
        OCIServerAttach(pConn->pSrv, pConn->pErr, (text*)host, strlen(host), 0);

    OCIAttrSet((dvoid*)pConn->pSvc, OCI_HTYPE_SVCCTX, (dvoid*)pConn->pSrv, 
        (ub4)0, OCI_ATTR_SERVER, (OCIError*)pConn->pErr);

    OCIHandleAlloc((dvoid*)pConn->pEnv, (dvoid**)&pConn->pSession,
        (ub4)OCI_HTYPE_SESSION, (size_t)0, (dvoid**)0);

    OCIAttrSet((dvoid*)pConn->pSession, (ub4)OCI_HTYPE_SESSION, (dvoid*)uid, 
        (ub4)strlen((char*)uid), (ub4)OCI_ATTR_USERNAME, pConn->pErr);

    OCIAttrSet((dvoid*)pConn->pSession, (ub4)OCI_HTYPE_SESSION, (dvoid*)passwd,
        (ub4)strlen((char*)passwd), (ub4)OCI_ATTR_PASSWORD, pConn->pErr);
    status = OCISessionBegin(pConn->pSvc, pConn->pErr, pConn->pSession, 
        OCI_CRED_RDBMS, (ub4)OCI_DEFAULT);
    if(status != OCI_SUCCESS)
    {
        if(pConn->pErr)
        {
            OCIServerDetach(pConn->pSrv, pConn->pErr, OCI_DEFAULT);
            OCIHandleFree((dvoid*)pConn->pErr, OCI_HTYPE_ERROR);
        }
        if(pConn->pSrv)
            OCIHandleFree((dvoid*)pConn->pSrv, OCI_HTYPE_SERVER);
        if(pConn->pSvc)
            OCIHandleFree((dvoid*)pConn->pSvc, OCI_HTYPE_SVCCTX);
        if(pConn->pSession)
            OCIHandleFree((dvoid*)pConn->pSession, OCI_HTYPE_SESSION);
        if(pConn->pEnv)
            OCIHandleFree((dvoid*)pConn->pEnv, OCI_HTYPE_ENV);
        
        free(pConn);
        
        return NULL;
    }

    OCIAttrSet((dvoid*)pConn->pSvc, (ub4)OCI_HTYPE_SVCCTX,
        (dvoid*)pConn->pSession, (ub4)0,
        (ub4)OCI_ATTR_SESSION, pConn->pErr);

    return(pConn);
}

/*---------------------------------------------------------------
  �������ƣ�dbDisconnect()
  ����˵�����Ͽ����ݿ����ӣ��ͷ�pConnָ���buffer
  ����˵����[in]pConn   -- DB_CONNECT�ṹָ��
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbDisconnect(DB_CONNECT* pConn)
{
    if(pConn == NULL)
        return 0;
    
    OCISessionEnd(pConn->pSvc, pConn->pErr, pConn->pSession, OCI_DEFAULT);
    
    if(pConn->pErr)
    {
        OCIServerDetach(pConn->pSrv, pConn->pErr, OCI_DEFAULT);
        OCIHandleFree((dvoid*)pConn->pErr, OCI_HTYPE_ERROR);
    }
    if(pConn->pSrv)
        OCIHandleFree((dvoid*)pConn->pSrv, OCI_HTYPE_SERVER);
    if(pConn->pSvc)
        OCIHandleFree((dvoid*)pConn->pSvc, OCI_HTYPE_SVCCTX);
    if(pConn->pSession)
        OCIHandleFree((dvoid*)pConn->pSession, OCI_HTYPE_SESSION);
    if(pConn->pEnv)
        OCIHandleFree((dvoid*)pConn->pEnv, OCI_HTYPE_ENV);
    
    free(pConn);
    
    return 1;
}

/*---------------------------------------------------------------
  �������ƣ�dbOpen()
  ����˵���������ݿ��α�
  ����˵����[in]pConn   -- DB_CONNECT�ṹָ��
  ����ֵ��  ���ɹ�  -- NULL
            �ɹ�    -- DB_CURSOR�ṹָ��
---------------------------------------------------------------*/
DB_CURSOR* dbOpen(DB_CONNECT* pConn)
{    
    DB_CURSOR* pCursor;
    int status;
    if(pConn == NULL || (pCursor = (DB_CURSOR*)malloc(sizeof(DB_CURSOR))) == NULL)
        return NULL;
    
    status = OCIHandleAlloc((dvoid*)pConn->pEnv, (dvoid**)&pCursor->pStmt,
        OCI_HTYPE_STMT, (size_t)0, (dvoid**)0);
    if(status == OCI_SUCCESS) {
        pCursor->pConn = pConn;
        pCursor->nDef = 0;
        pCursor->pDef = NULL;
        pCursor->pSql = NULL;
        pCursor->bDescribed = 0;
        return(pCursor);
    }
    else {
        free(pCursor);
        return NULL;
    }
}

/*---------------------------------------------------------------
  �������ƣ�dbClose()
  ����˵�����ر����ݿ��α�
  ����˵����[in]pCursor -- DB_CURSOR�ṹָ��
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbClose(DB_CURSOR* pCursor)
{
    if(pCursor == NULL)
        return 0;
        
    _dbFreeDefineBuf(pCursor);
    
    if(pCursor->pSql)
        free(pCursor->pSql);
    
    if(pCursor->pStmt)
        OCIHandleFree((dvoid*)pCursor->pStmt, OCI_HTYPE_STMT);
    free(pCursor);
    
    return 1;
}

/*---------------------------------------------------------------
  �������ƣ�dbParse()
  ����˵��������SQL���
  ����˵����[in]pCursor -- DB_CURSOR�ṹָ��
            [in]sqlstmt -- SQL����ַ�������'\0'��β
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbParse(DB_CURSOR* pCursor, char* sqlstmt)
{
    sword status;
    
    if(pCursor == NULL || sqlstmt == NULL)
        return 0;
    
    status = OCIStmtPrepare(pCursor->pStmt, pCursor->pConn->pErr, sqlstmt,
        (ub4)strlen(sqlstmt),
        (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT);
    if(status != OCI_SUCCESS) {
        return 0;
    }
    if(pCursor->pSql != NULL)
        free(pCursor->pSql);
    pCursor->pSql = calloc(strlen(sqlstmt)+1, 1);
    if(pCursor->pSql == NULL)
    {
        printf("insufficient memory\n");
        return 0;
    }
    memcpy(pCursor->pSql, sqlstmt, strlen(sqlstmt)+1);    
    pCursor->bDescribed = 0;
    return((status == OCI_SUCCESS) ? 1 : 0);
}

/*---------------------------------------------------------------
  �������ƣ�dbBind()
  ����˵������SQL����еı�����Ӧ�ó����еı���
  ����˵����[in]pCursor -- DB_CURSOR�ṹָ��
            [in]sqlvar  -- SQL����еı���������'\0'��β
            [in]progv   -- �����еı���
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbBind(DB_CURSOR* pCursor, char* sqlvar, char* progv)
{    
    sword status;
    OCIBind* pBind = NULL;
    
    status = OCIBindByName(pCursor->pStmt, (OCIBind**)&pBind, pCursor->pConn->pErr, 
        (text*)sqlvar, -1, (dvoid*)progv,
             strlen(progv)+1, SQLT_STR, (dvoid*)0,
             (ub2*)0, (ub2*)0, (ub4)0, (ub4*)0, OCI_DEFAULT);
    return((status == OCI_SUCCESS) ? 1 : 0);
}

/*---------------------------------------------------------------
  �������ƣ�dbDescribe()
  ����˵����ȡ��SQL�����select-list������
  ����˵����[in]pCursor -- DB_CURSOR�ṹָ��
            [in]pos     -- select-list����ţ���1��ʼ����
            [in/out]pDescr  -- DB_DESCR�ṹָ��
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbDescribe(DB_CURSOR* pCursor, int pos, DB_DESCR* pDescr)
{
    sword status;
    OCIParam *pPara;
           
    /*Get OCIParam*/
    if(dbSqlCommandType(pCursor->pSql) == SQL_CMD_SELECT && pCursor->bDescribed != 1)
    {
        status = OCIStmtExecute(pCursor->pConn->pSvc, pCursor->pStmt, pCursor->pConn->pErr,
            (ub4)0, (ub4)0, NULL, NULL, OCI_DESCRIBE_ONLY);    
        pCursor->bDescribed = 1;
        if(status != OCI_SUCCESS) {
            return 0;
        }
    }
    
    status = OCIParamGet(pCursor->pStmt, OCI_HTYPE_STMT, pCursor->pConn->pErr, 
        (dvoid**)&pPara, pos);
    
    if(status != OCI_SUCCESS || pPara == NULL)
        return 0;
          
    /*Get Select-list name*/    
    status = OCIAttrGet((dvoid*)pPara, (ub4)OCI_DTYPE_PARAM, (dvoid**)&pDescr->cbuf, 
        &pDescr->cbufl, OCI_ATTR_NAME, pCursor->pConn->pErr);

    /*Get Data type of the select-list*/
    status = OCIAttrGet(pPara, OCI_DTYPE_PARAM, &pDescr->dbtype, NULL, OCI_ATTR_DATA_TYPE,
        pCursor->pConn->pErr);

    /*Get Maximum size of the select-list*/
    status = OCIAttrGet(pPara, OCI_DTYPE_PARAM, &pDescr->dbsize, NULL, OCI_ATTR_DATA_SIZE,
        pCursor->pConn->pErr);
    _dbsize2dsize(pDescr);

    return((status == OCI_SUCCESS) ? 1 : 0);
}

/*---------------------------------------------------------------
  �������ƣ�dbExec()
  ����˵����exec sql statement and define select-list��
  ����˵����[in][out]pCursor  -- DB_CURSOR�ṹָ��
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbExec(DB_CURSOR* pCursor)
{
    sword i, status;
    DB_DESCR d;
    DB_DEFINE *pDef;
    OCIDefine* pd;
    ub4 nExecCount = 1;
        
    _dbFreeDefineBuf(pCursor);

    for(i = 1; ; i++)
    {
        if(!dbDescribe(pCursor, i, &d))
            break;

        if(!_dbAppendDefineBuf(pCursor, &d))
        {
            printf("error in parse sql\n");
            return 0;
        }
        pDef = pCursor->pDef + pCursor->nDef - 1;
                    
        status = OCIDefineByPos(pCursor->pStmt, &pd, pCursor->pConn->pErr, i,
            pDef->buf, pDef->bufl, SQLT_STR, &pDef->ind, &pDef->rlen, &pDef->rcode,
            OCI_DEFAULT);    
        if(status != OCI_SUCCESS) {
            return 0;
        }
        nExecCount = 0;
    }        
        
    status = OCIStmtExecute(pCursor->pConn->pSvc, pCursor->pStmt, pCursor->pConn->pErr,
        nExecCount, (ub4)0, (OCISnapshot*)0, (OCISnapshot*)0, OCI_DEFAULT);
    return((status == OCI_SUCCESS) ? 1 : 0);
}

/*---------------------------------------------------------------
  �������ƣ�dbFetch()
  ����˵����ȡ���α�����λ�õļ�¼�������α��ƶ�����һ����¼
  ����˵����[in/out]pCursor -- DB_CURSOR�ṹָ��            
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbFetch(DB_CURSOR* pCursor)
{
    int status;
    
    status = OCIStmtFetch(pCursor->pStmt, pCursor->pConn->pErr, 1, OCI_FETCH_NEXT, OCI_DEFAULT);
    return((status == OCI_SUCCESS) ? 1 : 0);
}

/*---------------------------------------------------------------
  �������ƣ�dbFieldValue()
  ����˵����ȡ��select-list��ֵ
  ����˵����[in]pCursor -- DB_CURSOR�ṹָ��
            [in]iPos    -- select-list����ţ���1��ʼ����
            [in/out]pVal -- ָ����select-listֵ��buffer
            [in/out]pLen -- ָ��buffer���ȣ��������غ�ָ��ʵ�ʳ���
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbFieldValue(DB_CURSOR* pCursor, int iPos, char* pVal, int* pLen)
{
    DB_DEFINE* pDef;
    
    if(iPos > pCursor->nDef || iPos < 1)
    {        
        return 0;
    }
    
    memset(pVal, 0, *pLen);
    pDef = pCursor->pDef + iPos - 1;
    *pLen = min(*pLen, pDef->rlen);
    memcpy(pVal, pDef->buf, *pLen);
    
    return 1;
}

/*---------------------------------------------------------------
  �������ƣ�dbFieldName()
  ����˵����ȡ��select-list������
  ����˵����[in]pCursor -- DB_CURSOR�ṹָ��
            [in]iPos    -- select-list����ţ���1��ʼ����
            [in/out]pName -- ָ����select-listֵ��buffer
            [in/out]pLen -- ָ��buffer���ȣ��������غ�ָ��ʵ�ʳ���
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbFieldName(DB_CURSOR* pCursor, int iPos, char* pName, int* pLen)
{
    int i;
    DB_DEFINE* pDef;
    
    if(iPos > pCursor->nDef || iPos < 1)
    {        
        return 0;
    }
    
    memset(pName, 0, *pLen);
    pDef = pCursor->pDef + iPos - 1;
    *pLen = min(*pLen, pDef->namel);
    memcpy(pName, pDef->name, *pLen);
    for(i = 0; i < *pLen; i++)
        pName[i] = tolower(pName[i]);
    
    return 1;
}

/*---------------------------------------------------------------
  �������ƣ�dbCommit()
  ����˵�����ύ�����ݿ�ĸ��¡�
  ����˵����[in]pConn   -- DB_CONNECT�ṹָ��            
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbCommit(DB_CONNECT* pConn)
{
    sword status;
    
    status = OCITransCommit(pConn->pSvc, pConn->pErr, (ub4)0);
    
    return((status == OCI_SUCCESS) ? 1 : 0);
}    

/*---------------------------------------------------------------
  �������ƣ�dbRollback()
  ����˵����ȡ�����ϴ�commit֮������ݿ�ĸ��¡�
  ����˵����[in]pConn   -- DB_CONNECT�ṹָ��            
  ����ֵ��  ���ɹ�  -- 0
            �ɹ�    -- 1
---------------------------------------------------------------*/
int dbRollback(DB_CONNECT* pConn)
{
    sword status;
    
    status = OCITransRollback(pConn->pSvc, pConn->pErr, (ub4)0);
    
    return((status == OCI_SUCCESS) ? 1 : 0);
}    

/*---------------------------------------------------------------
  �������ƣ�dbPrintError()
  ����˵������ʾ���ݿ����������Ϣ��
  ����˵����[in]pCursor  -- DB_CURSOR�ṹָ��
  ����ֵ��  �޷���
---------------------------------------------------------------*/       
void dbPrintError(DB_CONNECT* pConn)
{
    text errbuf[512];
    ub4 errcode;
    
    OCIErrorGet(pConn->pErr, (ub4) 1, (text *) NULL, &errcode,
                    errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    printf("Error - %s\n", errbuf);
        
    return;
}

/*---------------------------------------------------------------
  �������ƣ�dbGetLastError()
  ����˵����ȡ�����ݿ��������š�
  ����˵����[in]pCursor  -- DB_CURSOR�ṹָ��
  ����ֵ��  �����
---------------------------------------------------------------*/
int dbGetLastError(DB_CONNECT* pConn)
{
    text errbuf[512];
    ub4 errcode;
    
    OCIErrorGet(pConn->pErr, (ub4) 1, (text *) NULL, &errcode,
                    errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    
    return errcode;
}



/*---------------------------------------------------------------
  �������ƣ�dbSqlCommandType()
  ����˵����ȡ��Sql�������͡�
  ����˵����[in]sqlstmt  -- Sql���ָ��
  ����ֵ��  
            SQL_CMD_UNKNOWN -- δ֪
            SQL_CMD_SELECT -- select
            SQL_CMD_INSERT -- insert
            SQL_CMD_UPDATE -- update
            SQL_CMD_DELETE -- delete
---------------------------------------------------------------*/
int dbSqlCommandType(char* sqlstmt)
{
    char *pSql;
    int i, len;
    char ppCmd[4][7] = {"select", "insert", "update", "delete"};
    int pRtn[4] = {SQL_CMD_SELECT, SQL_CMD_INSERT, SQL_CMD_UPDATE, SQL_CMD_DELETE};
    
    len = strlen(sqlstmt);
    pSql = calloc(len, 1);
    memcpy(pSql, sqlstmt, len);
    if(pSql == NULL)
    {
        printf("insufficient memory\n");
        return 0;
    }
    
    for(i = 0; i < len; i++)
    {
        pSql[i] = tolower(pSql[i]);
    }
    
    for(i = 0; i < 4; i++)
    {
        if(strstr(pSql, &ppCmd[i][0]))
        {
            free(pSql);
            return(pRtn[i]);
        }
    }
    
    free(pSql);
    return(SQL_CMD_UNKNOWN);
}

void checkerr(OCIError *errhp, sword status)
{
  text errbuf[512];  
  ub4 errcode;

  switch (status)
  {
  case OCI_SUCCESS:
    break;
  case OCI_SUCCESS_WITH_INFO:
    printf("Error - OCI_SUCCESS_WITH_INFO\n");
       OCIErrorGet (errhp, (ub4) 1, (text *) NULL, &errcode,
                    errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    printf("Success with info - %s\n", errbuf);
    break;
  case OCI_NEED_DATA:
    printf("Error - OCI_NEED_DATA\n");
    break;
  case OCI_NO_DATA:
    /*printf("OCI_NODATA\n");*/
    break;
  case OCI_ERROR:
    OCIErrorGet (errhp, (ub4) 1, (text *) NULL, &errcode,
                    errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    printf("Error - %s\n", errbuf);
    break;
  case OCI_INVALID_HANDLE:
    printf("Error - OCI_INVALID_HANDLE\n");
    break;
  case OCI_STILL_EXECUTING:
    printf("Error - OCI_STILL_EXECUTE\n");
    break;
  default:
    break;
  }

}

void _dbsize2dsize(DB_DESCR* pDescr)
{
    switch (pDescr->dbtype)
    {
        case  1 : /* CHAR datatype: no change in length
                     needed, except possibly for TO_CHAR
                     conversions (not handled here). */
            pDescr->dsize = pDescr->dbsize + 1;
            break;
        case  2 : /* NUMBER datatype: use sqlprc() to
                     extract precision and scale. */
            /*sqlprc (&(select_dp->L[i]), &precision, &scale[i]);*/
                  /* Allow for maximum size of NUMBER. */
            /*if (precision == 0) precision = 40;*/
                  /* Also allow for decimal point and
                     possible sign. */
            /* convert NUMBER datatype to FLOAT if scale[i] > 0,
               INT otherwise. */
            /*if (scale[i] > 0)
                select_dp->L[i] = sizeof(float);
            else*/
                pDescr->dsize = 50;
            break;

        case  8 : /* LONG datatype */
            pDescr->dsize = 240;
            break;

        case 11 : /* ROWID datatype */
            pDescr->dsize = 18;
            break;

        case 12 : /* DATE datatype */
            pDescr->dsize = 50;
            break;

        case 23 : /* RAW datatype */
            pDescr->dsize = 0;
            break;

        case 24 : /* LONG RAW datatype */
            pDescr->dsize = 240;
            break;
        default:
            pDescr->dsize = 50;
            break;
    }
}

int _dbStr2Lower(char* str)
{
    int i, len;
    
    len = strlen(str);

    for(i = 0; i < len; i++)
        str[i] = tolower(str[i]);
    
    return 0;
}

void _dbFreeDefineBuf(DB_CURSOR* pCursor)
{
    int i;
    DB_DEFINE* pDef;
    
    if(pCursor->pDef != NULL && pCursor->nDef != 0)
    {
        for(i = 0; i < pCursor->nDef; i++)
        {
            pDef = pCursor->pDef + i;
            free(pDef->buf);
            free(pDef->name);
        }
        free(pCursor->pDef);
        pCursor->pDef = NULL;
        pCursor->nDef = 0;
    }
    
    return;
}

int _dbAppendDefineBuf(DB_CURSOR* pCursor, DB_DESCR* pDescr)
{
    DB_DEFINE* pDef;
    
    if(pCursor->pDef == NULL)
    {
        pCursor->pDef = (DB_DEFINE*)calloc(sizeof(DB_DEFINE), COL_MAX);
        if(pCursor->pDef == NULL)
        {
            printf("insufficient memory in append define buf\n");
            return 0;
        }
        pCursor->nDef = 0;
    }
        
    pCursor->nDef = pCursor->nDef + 1;
    pDef = pCursor->pDef + pCursor->nDef - 1;
    if((pDef->buf = (ub1*)calloc(pDescr->dsize+1, 1)) == NULL)
    {
        printf("insufficient memory in append define buf -- calloc\n");        
        return 0;
    }
    pDef->bufl = pDescr->dsize + 1;
    /*          Add by Liyunming 2002/3/28 */
    pDef->ftype = pDescr->dbtype;
    /*          Add by Liyunming 2002/3/28 */
    if((pDef->name = (ub1*)calloc(pDescr->cbufl, 1)) == NULL)
    {
        printf("insufficient memory in append define buf -- calloc\n");
        return 0;
    }
    pDef->namel = pDescr->cbufl;
    memcpy(pDef->name, pDescr->cbuf, pDescr->cbufl);
    
    return 1;
}
