#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <gdfontmb.h>
#include "msas.h"

/*
���ߣ�heidsoft.liu
����޸����ڣ� 2010-9-21
��Ҫ���ܣ����������˺������˺���ͨ��sql�������ݿ⣬�����������ݷ��ء�
���������int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead
����������
*/
int msasTestApp1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char *name;
    char *password;
    char *rePass;
  //char myre[1024];
    char caStr[2048];
    char sqlBuf[2048];
  int  maxuid;

  ltDbCursor *tempCursor;//ע���
  LT_DBROW   tempRow;

  name=ltMsgGetVar_s(ltMsgPk,"txtName");
    if(!name){
         name="";
   }
     password=ltMsgGetVar_s(ltMsgPk,"txtPass");
    if(!password){
         password="";
   }
   rePass=ltMsgGetVar_s(ltMsgPk,"txtRePass");
    if(!rePass){
         rePass="";
   }
   if(strcmp(password,rePass)!=0){
       ltMsgPk->msgpktype=1;
       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
                "lthtml",LT_TYPE_STRING,"error!");   
      ltMsgFree(ltMsgPk);
      return 0; 
   }
   
   maxuid=1;
   sprintf(sqlBuf,"select max(uid)+1 from msasusertest" );
   
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);   
   if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
              maxuid=atol(tempRow[0]);  
          }
     ltDbCloseCursor(tempCursor);
   }

   sprintf(sqlBuf,"insert into msasusertest values (%d,'%s','%s')",maxuid,name,password);
   ltDbExecSql(G_DbCon,sqlBuf); 
   
   memset(caStr,0,sizeof(caStr));
   sprintf(sqlBuf,"select * from msasusertest" );
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);   
   if(tempCursor!=NULL){ 
          tempRow= ltDbFetchRow(tempCursor);
          while(tempRow!=NULL){
             sprintf(caStr,"%s%s %s %s\r\n<br>",caStr,tempRow[0],tempRow[1],tempRow[2]);
             tempRow= ltDbFetchRow(tempCursor);
          }
        ltDbCloseCursor(tempCursor);
   }

    ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
                "lthtml",LT_TYPE_STRING,caStr);   
    ltMsgFree(ltMsgPk);
    return 0;
}
//********************************************************************************************************************

/*
���ߣ�heidsoft.liu
����޸����ڣ� 2010-9-21
��Ҫ���ܣ������ύ���ݣ������ύ���ݷ��ص�ҳ�档
���������int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead
����������
*/

/*
int msasTestApp2(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  char *name;
    char *password;
    char *rePass;
  //char myre[1024];
    char caStr[2048];
    char sqlBuf[2048];
    name=ltMsgGetVar_s(ltMsgPk,"txtName");
    if(!name){
         name="";
   }
    password=ltMsgGetVar_s(ltMsgPk,"txtPass");
    if(!password){
         password="";
   }
   rePass=ltMsgGetVar_s(ltMsgPk,"txtRePass");
    if(!rePass){
         rePass="";
   }
   if(strcmp(password,rePass)!=0){
       ltMsgPk->msgpktype=1;
       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\r\n",
                "lthtml",LT_TYPE_STRING,"error!");   
      ltMsgFree(ltMsgPk);
      return 0; 
   }

   sprintf(caStr,"insert into msasusertest values (3,'%s','%s')",name,password);
   ltDbExecSql(G_DbCon,sqlBuf); 

   sprintf(caStr,"%s is:%s password:%s",_ltPubInfo->pubMsgStr[0].conStr,name,password);
     ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\r\n",
                "lthtml",LT_TYPE_STRING,caStr);   
    ltMsgFree(ltMsgPk);
    return 0;
}
*/

//********************************************************************************************************

/*
���ߣ�heidsoft.liu
����޸����ڣ� 2010-9-21
��Ҫ���ܣ����ô�ӡ��ʽ����ҳ������
���������int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead
����������
*/
int msasTestApp3(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char *name;
  char *password;
  char *rePass;
  char caStr[2048];
   //char sqlBuf[2048];
  
  name=ltMsgGetVar_s(ltMsgPk,"txtName");
  if(!name){
         name="";
  }
    password=ltMsgGetVar_s(ltMsgPk,"txtPass");
  if(!password){
         password="";
  }
  rePass=ltMsgGetVar_s(ltMsgPk,"txtRePass");
  if(!rePass){
         rePass="";
  }
  if(strcmp(password,rePass)!=0){
       ltMsgPk->msgpktype=1;
       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
                "lthtml",LT_TYPE_STRING,"error!");   
      ltMsgFree(ltMsgPk);
      return 0; 
  }
  
  sprintf(caStr,"%s",
  "<html><head><meta http-equiv='Content-Type' content='text/html; charset=utf-8'/><title>ע��</title></head><body><form><table><tr><td>�û�����</td><td><input type='text' id='username' value='+txtPass+' /></td></tr><tr><td>��&nbsp;&nbsp;�룺</td><td><input type='text' id='password' value='+txtRePass+'/></td></tr></table></form></body></html>");
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
                "lthtml",LT_TYPE_STRING,caStr);   
   ltMsgFree(ltMsgPk);
   return 0;    
}


/*
���ߣ�heidsoft.liu
����޸����ڣ� 2010-9-21
��Ҫ���ܣ�����ģ�巵����������
���������int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead
����������
*/


  int msasTestApp4(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  char useracoount[32];
  char sqlBuf[1024];
  char caLabel[256];
  char caTempDir[256];

  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  ltDbHeadPtr dbPtr;
  ltTablePtr  tablePtr;

    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    sprintf(useracoount,"%s","0");
    sprintf(sqlBuf,"%s","select count(*) from AlertList ");
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    
    if(tempCursor!=NULL){
      tempRow= ltDbFetchRow(tempCursor);
      if(tempRow!=NULL){
        sprintf(useracoount,"%s",tempRow[0]);
      }
      ltDbCloseCursor(tempCursor);
    }
    
    lt_dbput_rootvars(dbPtr,1,"alluser",useracoount);
    tablePtr=lt_dbput_table(dbPtr,"userlist");
    sprintf(sqlBuf,"%s","select * from AlertList ");
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
                tempRow= ltDbFetchRow(tempCursor);
                while(tempRow!=NULL){
                 lt_dbput_recordvars(tablePtr,2,
                 "uname",LT_TYPE_STRING,tempRow[1],
                 "group",LT_TYPE_STRING,tempRow[2]);
               tempRow=ltDbFetchRow(tempCursor);
                }
         ltDbCloseCursor(tempCursor);
    }
      
  ltMsgPk->msgpktype=1;
  //lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msashtml/test1/model.htm",dbPtr->head,0);
  char  *lt_page_content;
  int   iFd;
   lt_page_content=ltPltFileParse("/app/msa/msashtml/test1/moldboard/dailyreportpart1.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","�ޱ�������,�޷����ɱ���",caTempDir);
            system(caLabel);/*����Ŀ¼�����ڴ����־*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  }else{
        lt_page_content=strstr(lt_page_content,"<html");
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
      /*����html��ʽ����*/
  }
  
  ltMsgFree(ltMsgPk);

  return 0;
}
