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

main(){
	ltDbConn *tempCon;
	LT_DBROW tempRow;
	int i;
	int fieldnum;
	tempCon=ltDbConnect("nc","nc",NULL);
	if(tempCon!=NULL){
		printf("ok\n");
	}else{
		printf("connect error\n");
	}
	tempRow=ltDbOneRow(tempCon,&fieldnum,"select * from NCADMUSER");
	printf("fieldnum:%d\n",fieldnum);
	if(tempRow==NULL){
		printf("get row error:\n");
	}
	for (i=0; i<fieldnum;i++){
		printf("%s\n",tempRow[i]);
	}
	ltDbFreeRow(tempRow,fieldnum);
	ltDbClose(tempCon);




}

