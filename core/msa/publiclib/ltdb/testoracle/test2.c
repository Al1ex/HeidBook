#include <stdio.h>
#include <stdlib.h>
#include "lt.h"
#include "dboracle.h"
#include "ltdb.h"

/*���ݿ�����*/
//ltDbConn *ltDbConnect(char *pUser,char *pPassword,char *pService);
/* �ر����ݿ����ӣ����ͷ��й���Դ  */
//void  ltDbClose(ltDbConn *psConn);
/*������¼����*/
//LT_DBROW ltDbOneRow(ltDbConn *pConn,int *fieldnum,char *pSmt,...);
//void ltDbFreeRow(LT_DBROW dbRow,int fieldnum);
//ltDbCursor *ltDbOpenCursor(ltDbConn *pConn,  char *pSmt,...)
//void ltDbCloseCursor(ltDbCursor *ltCursor);
main(){
	ltDbConn *tempCon;
	LT_DBROW tempRow;
	int i,j;
	int fieldnum;
	ltDbCursor *tempCursor;
	tempCon=ltDbConnect("nc","nc",NULL);
	if(tempCon!=NULL){
		printf("ok\n");
	}else{
		printf("connect error\n");
	}
	tempCursor=ltDbOpenCursor(tempCon, "select * from NCADMUSER");
	if(tempCursor==NULL){
		printf("opn cursor eror\n");
	}
	/*fetch���õ��ֶ�ֵ*/
	tempRow= ltDbFetchRow(tempCursor);
	fieldnum=ltNumField(tempCursor);
	printf("fieldnum:%d\n",fieldnum);
        j=0;
	while( tempRow!=NULL ){
		
		j++;
		printf("j:%d\n",j);
		for (i=0; i<fieldnum;i++){
			printf("%s\n",tempRow[i]);
		}
		if(j>10){
			break;
		}
		tempRow= ltDbFetchRow(tempCursor);
	}
	ltDbCloseCursor(tempCursor);	
	
	//int ltDbExecSql(ltDbConn *pConn,  char *pSmt,...);
	for(i=1;i<10000;i++){
		ltDbExecSql(tempCon,"insert into ncadmuser (name ,password) values ('xxxxxx%d','xxx')",i);
		//ltDbExecSql(tempCon,"delete ncadmuser where name ='xxx%d'",2900+i);
		ltDbCommit(tempCon);
	}
	
	ltDbClose(tempCon);

}

