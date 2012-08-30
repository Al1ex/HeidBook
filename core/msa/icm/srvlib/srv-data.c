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
#include "icm.h"
///////////////////////////////////////////////////////////
//ϵͳ״̬�б��ѯ������
///////////////////////////////////////////////////////////
int icmSystemListSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 char strBuf[1024];   
   char caTmpp[512];
   long lRowNum,lStartRec,lSumRec,lCount,lNum;   
	 char *time1;//����ʱ��1
	 char *time2;//����ʱ��2
	 char *user;//�û����������
	 char caWhere[512];
	 //char *exportaction;
	 int k;
	 stralloc strTemp;
	 ltDbHeadPtr dbPtr;
	 ltDbCursor *tempCursor;
   LT_DBROW tempRow;  

   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   time1=ltMsgGetVar_s(ltMsgPk,"time1");  //��������Id
   time2=ltMsgGetVar_s(ltMsgPk,"time2");//����Id
   user=ltMsgGetVar_s(ltMsgPk,"user");//������������
	 memset(caWhere,0,sizeof(caWhere));
	 sprintf(caWhere,"%s","1=1 ");	
	 if(time1 && time1[0]){
		sprintf(caWhere,"%s and online_time > '%s'",caWhere,time1);
		}
	 
	 if(time2 && time2[0] ){
		sprintf(caWhere,"%s and online_time < '%s'",caWhere,time2);
		}
	 
	 if(user && user[0] ){
		sprintf(caWhere,"%s and b.username like '%c%s%c'",caWhere,'%',user,'%');
	  }
	
   lRowNum=20; /*ÿҳ������*/
   lSumRec=0;/*������*/
   lCount=0; 
   lStartRec=0;
   lNum=0; /*��ҳʣ�������*/
   if(ltMsgGetVar_s(ltMsgPk,"limit")){
	   lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	 }
   if(ltMsgGetVar_s(ltMsgPk,"start")){
	   lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
   sprintf(strBuf,"select count(*) from clientlist where %s ",caWhere);
	 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    sprintf(strBuf,"select a.logid,b.username,a.online_time,a.offline_time,a.net_ending_ip,a.net_ending_mac,a.net_ending_face from clientlist a,icmuserinfo b where a.userid=b.uid and %s limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{  
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	   	 {                
	       if(k==0)
				{
	        sprintf(caTmpp,"{\"logid\":\"%s\",\"username\":\"%s\",\"online_time\":\"%s\",\"offline_time\":\"%s\",\"net_ip\":\"%s\",\"net_mac\":\"%s\",\"net_face\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6]);
	        stralloc_cats(&strTemp,caTmpp);     
				}
				else
				{
			    sprintf(caTmpp,",{\"logid\":\"%s\",\"username\":\"%s\",\"online_time\":\"%s\",\"offline_time\":\"%s\",\"net_ip\":\"%s\",\"net_mac\":\"%s\",\"net_face\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6]);
	        stralloc_cats(&strTemp,caTmpp);     
				}                         
				tempRow=ltDbFetchRow(tempCursor);
				k++;
		   }
       ltDbCloseCursor(tempCursor);
	   }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
	}
////////////////////////////////////////////////////////////
//ARP�����б��ѯ������
////////////////////////////////////////////////////////////
int icmArpListSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 char strBuf[1024];   
   char caTmpp[512];
   long lRowNum,lStartRec,lSumRec,lCount,lNum;   
	 char *ip;//����ʱ��1
	 char *mac;//����ʱ��2
	 char caWhere[512];
	 //char *exportaction;
	 int k;
	 stralloc strTemp;
	 ltDbHeadPtr dbPtr;
	 ltDbCursor *tempCursor;
   LT_DBROW tempRow;  
   
	 
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");

   ip=ltMsgGetVar_s(ltMsgPk,"ip");  //arp ����IP
   printf("%s\n",ip);
   mac=ltMsgGetVar_s(ltMsgPk,"mac");//arp ����mac
   printf("%s\n",mac);
	 memset(caWhere,0,sizeof(caWhere));
	 sprintf(caWhere,"%s","1=1 ");
			
	 if(ip && ip[0]){
		sprintf(caWhere,"%s and arpip like '%c%s%c'",caWhere,'%',ip,'%');
		}
	 if(mac && mac[0] ){
		sprintf(caWhere,"%s and arpmac like '%c%s%c'",caWhere,'%',mac,'%');
		}
			
   lRowNum=20; /*ÿҳ������*/
   lSumRec=0;/*������*/
   lCount=0; 
   lStartRec=0;
   lNum=0; /*��ҳʣ�������*/
   if(ltMsgGetVar_s(ltMsgPk,"limit")){
	   lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	 }
   if(ltMsgGetVar_s(ltMsgPk,"start")){
	   lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
   sprintf(strBuf,"select count(*) from arpreport where %s ",caWhere);
	 tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    sprintf(strBuf,"select arpip,arpmac,requestnum,requestlasttime,flag from arpreport where %s limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	   	 {                
	       if(k==0)
				{
	        sprintf(caTmpp,"{\"ipadd\":\"%s\",\"mac\":\"%s\",\"requestnum\":\"%s\",\"lastrequesttime\":\"%s\",\"flag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4]);
	        stralloc_cats(&strTemp,caTmpp);     
				}
				else
				{
			    sprintf(caTmpp,",{\"ipadd\":\"%s\",\"mac\":\"%s\",\"requestnum\":\"%s\",\"lastrequesttime\":\"%s\",\"flag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4]);
	        stralloc_cats(&strTemp,caTmpp);     
				}                         
				tempRow= ltDbFetchRow(tempCursor);
				k++;
		   }
       ltDbCloseCursor(tempCursor);
	   }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
	}
/////////////////////////////////////////////////////////////
//����:ϵͳ״̬ͳ�Ʊ������ɴ�����
//      ��ARP����ͳ�Ʊ������ɴ�����
//��д:heidsoft
//ʱ�䣺2010-11-2
/////////////////////////////////////////////////////////////
int icmCreateReport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	/*��������*/
	char caLabel[256];//Ŀ¼��Ǳ���
	char caSdate[32];//����ҳ�濪ʼʱ�����
	char caEdate[32];//����ҳ�����ʱ�����
	char caSdate1[32];
	char caTime1[32];
	char email[32];//����ҳ���ʼ�����
	char *reportname;//���ɱ������Ʋ���
	char caTempDir[256];//��ʱĿ¼
	char caTempFile[256];//��ʱ�ļ�
	long lTime;
 	unsigned int time1,time2,Max,reporttype;
		
 	char sqlBuf[1024];//sql��ѯ����洢����
	int srcfd,srcfd1;
	char caItem[16];
	char caTemp[18192];
	char caTempDataDir[256];
	char caTempDataDir1[256];
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow=0;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	ltTablePtr  tablePtr1;
		
	/*��ȡ��������*/
	reporttype=atol(ltMsgGetVar_s(ltMsgPk,"reporttype"));
	printf("%d\n",reporttype);
	/*����ִ�д������򣺻�ȡִ��ҳ�棨���ֲ㣩��Ӧ�Ĳ��������洢����*/
	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			sprintf(caSdate,"%s","");
    }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			sprintf(caEdate,"%s","");
    }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
			sprintf(caEdate,"%s","");
    }else{
    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    }
			
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
    	reportname="********";
    }
    
    time1 = nasCvtLongTime(caSdate,"00:00:00");
	  time2 = nasCvtLongTime(caEdate,"23:59:59");
	  lTime = time(0);
	  nasCvtStime(lTime,caSdate1,caTime1);
	  caTime1[5]=0;	
	  
	   sprintf(caTempDir,"%s/report/%d/%lu/",_datacenterdir,reporttype,ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// ���ɸñ���Ŀ¼���� 
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:�������ɴ���,�޷���������Ŀ¼!<br><a href=\"javascript:history.back(1);\" >�����±���?</a></body></html>");		
						ltMsgFree(ltMsgPk);
            return 0;
        }
   }
       
    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*����Ŀ¼��־*/
    sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);
    system(caLabel);/*����Ŀ¼��־*/
    sprintf(caLabel,"/bin/echo 'sdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*����Ŀ¼��־*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);
    system(caLabel);/*����Ŀ¼��־*/
    sprintf(caLabel,"/bin/echo 'scon=%s--%s' >> %sinfo.ini",caEdate,caSdate,caTempDir);
    system(caLabel);/*����Ŀ¼��־*/

    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);/*����Ŀ¼���ڴ����־*/
	  sprintf(caLabel,"/bin/cp /app/msa/icm/htmlplt/reportplt/left-1.jpg  %s",caTempDir);
    system(caLabel);/*����Ŀ¼���ڴ����־*/
	  sprintf(caLabel,"/bin/cp /app/msa/icm/htmlplt/reportplt/left-2.jpg  %s",caTempDir);
    system(caLabel);/*����Ŀ¼���ڴ����־*/
	  sprintf(caLabel,"/bin/cp /app/msa/icm/htmlplt/reportplt/left-3.jpg  %s",caTempDir);
    system(caLabel);/*����Ŀ¼���ڴ����־*/
	  sprintf(caLabel,"/bin/cp /app/msa/icm/htmlplt/reportplt/left-4.jpg  %s",caTempDir);
    system(caLabel);/*����Ŀ¼���ڴ����־*/
	  sprintf(caLabel,"/bin/cp /app/msa/icm/htmlplt/reportplt/banner.jpg  %s",caTempDir);
    system(caLabel);/*����Ŀ¼���ڴ����־*/
	  sprintf(caLabel,"/bin/cp /app/msa/icm/htmlplt/reportplt/right-top-d.jpg  %s",caTempDir);
    system(caLabel);/*����Ŀ¼���ڴ����־*/	
  
  /*�жϱ������ͣ�ѡ�񱨸�sqlִ�����࣬�Լ�ͼƬ���ɵ�����*/	
	if(reporttype==1){
		sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","�����ļ�д����,�޷����ɱ���",caTempDir);
		      system(caLabel);/*����Ŀ¼�����ڴ����־*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}		
		   /*    ģ�忪ʼ     */ 
    dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		Max = 100;
    sprintf(sqlBuf,"select stime,cast(avg(cpunum) as dec(18,2)) as cpunum,cast(avg(memnum) as dec(18,2)) as memnum,cast(avg(sysdisknum) as dec(18,2)) as sysdisknum,cast(avg(appdisknum) as dec(18,2)) as appdisknum,cast(avg(arpalertrequest) as dec(18,0)) as arpalertrequest,cast(avg(arpalertresponse) as dec(18,0)) as arpalertresponse from icmsysinfolog where rtime >= %u and rtime <=%u group by stime order by stime asc",time1,time2);
    printf("%s\n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
                while(tempRow!=NULL){
								 sprintf(caItem,"%s:00",tempRow[0]);
								 printf("caItem=%s\n",caItem);
                 lt_dbput_recordvars(tablePtr,7,
                 "stime",LT_TYPE_STRING,tempRow[0],
                 "cpunum",LT_TYPE_STRING,tempRow[1],
								 "memnum",LT_TYPE_STRING,tempRow[2],
                 "sysdisknum",LT_TYPE_STRING,tempRow[3],
                 "appdisknum",LT_TYPE_STRING,tempRow[4],
                 "arpalertrequest",LT_TYPE_STRING,tempRow[5],
                 "arpalertresponse",LT_TYPE_STRING,tempRow[6]
                 );
				 sprintf(caTemp,"%s,%llu,%llu,%llu,\r\n",
			        	caItem,
						  	atoll(tempRow[1]),
								atoll(tempRow[2]),
								atoll(tempRow[3]));
			        write(srcfd, caTemp, strlen(caTemp));	
				 
               tempRow=ltDbFetchRow(tempCursor);
                }
         ltDbCloseCursor(tempCursor);
    }   
    sprintf(caTempFile,"%soutflow1.gif",caTempDir);
    sprintf(caTemp,"/app/msa/icm/htmlplt/Script/outflowpltline.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");
	  /* ģ��2*/
		sprintf(caTempDataDir1,"%sdata1",caTempDir);     
    srcfd1 = open(caTempDataDir1,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","�����ļ�д����,�޷����ɱ���",caTempDir);
		      system(caLabel);/*����Ŀ¼�����ڴ����־*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}	
    tablePtr1=lt_dbput_table(dbPtr,"list1");
    sprintf(sqlBuf,"select sdate,cast(avg(cpunum) as dec(18,2)) as cpunum,cast(avg(memnum) as dec(18,2)) as memnum,cast(avg(sysdisknum) as dec(18,2)) as sysdisknum,cast(avg(appdisknum) as dec(18,2)) as appdisknum,cast(avg(arpalertrequest) as dec(18,0)) as arpalertrequest,cast(avg(arpalertresponse) as dec(18,0)) as arpalertresponse from icmsysinfolog where rtime >=%u and rtime <=%u group by sdate order by sdate asc",time1,time2);
    printf("%s\n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){	
				tempRow= ltDbFetchRow(tempCursor);
                while(tempRow!=NULL){			
				sprintf(caItem,"%s",tempRow[0]);
                 lt_dbput_recordvars(tablePtr1,7,
                 "stime",LT_TYPE_STRING,tempRow[0],
                 "cpunum",LT_TYPE_STRING,tempRow[1],
				 				 "memnum",LT_TYPE_STRING,tempRow[2],
                 "sysdisknum",LT_TYPE_STRING,tempRow[3],
                 "appdisknum",LT_TYPE_STRING,tempRow[4],
                 "arpalertrequest",LT_TYPE_STRING,tempRow[5],
                 "arpalertresponse",LT_TYPE_STRING,tempRow[6]
                 );
            sprintf(caTemp,"%s,%llu,%llu,%llu,\r\n",
			      caItem,
						atoll(tempRow[1]),
						atoll(tempRow[2]),
						atoll(tempRow[3]));
				 
			        write(srcfd1, caTemp, strlen(caTemp));	

            tempRow=ltDbFetchRow(tempCursor);
           }
         ltDbCloseCursor(tempCursor);
    }
		
    lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);	
	  sprintf(caTempFile,"%soutflow2.gif",caTempDir);
    sprintf(caTemp,"/app/msa/icm/htmlplt/Script/outflowpltline.pl   %s %s %u ",caTempDataDir1,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file2","outflow2.gif");
    char  *lt_page_content;
    int   iFd;
    lt_page_content=ltPltFileParse("/app/msa/icm/htmlplt/reportplt/sysinforeport.htm",dbPtr->doc,0);
    if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","�ޱ�������,�޷����ɱ���",caTempDir);
            system(caLabel);/*����Ŀ¼�����ڴ����־*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
    }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");/*����html��ʽ����*/
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
    }
     ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >��������������,�뵽�����б��в쿴����״̬�������ر���<br><a href=\"/main/srvreportlist.htm\" >�쿴�±���?</a></body></html>");		
     //pdf��ʽ�������ɴ���
     sprintf(caLabel,"/app/msa/icm/htmlplt/Script/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
     printf("change to mht :: %s\n",caLabel);
     system(caLabel);
     chdir(caTempDir);
	   system("tar -cvzf report.tgz *");
	   sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	   system(caLabel);
	   sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	   system(caLabel);
	   ltMsgFree(ltMsgPk);
     return 0;
   }
	
	if(reporttype==2){
		sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","�����ļ�д����,�޷����ɱ���",caTempDir);
		      system(caLabel);/*����Ŀ¼�����ڴ����־*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}	
		 /*    ģ�忪ʼ     */ 
    dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		Max = 10;
    sprintf(sqlBuf,"select substring(requestlasttime,1,8) as date,count(*) as num from arpreport where substring(requestlasttime,1,8)>='%s' and substring(requestlasttime,1,8)<='%s' group by date",caEdate,caSdate);
    printf("%s\n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
				tempRow=ltDbFetchRow(tempCursor);
				if(Max<=(atol(tempRow[1]))){
		         Max = atol(tempRow[1]);
		     }
                while(tempRow!=NULL){				
								 sprintf(caItem,"%s",tempRow[0]);
								 printf("caItem=%s\n",caItem);
                 lt_dbput_recordvars(tablePtr,2,
                 "data",LT_TYPE_STRING,tempRow[0],
                 "num",LT_TYPE_STRING,tempRow[1]
				 				 );		 
			          sprintf(caTemp,"%s,%s\r\n", tempRow[0],tempRow[1]);
			          write(srcfd, caTemp, strlen(caTemp));				      				 
               tempRow=ltDbFetchRow(tempCursor);
                }
         ltDbCloseCursor(tempCursor);
    }
     lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);	
		sprintf(caTempFile,"%soutflow2.gif",caTempDir);
    sprintf(caTemp,"/app/msa/icm/htmlplt/Script/outflowplt.pl   %s %s %d ",caTempDataDir,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file1","outflow2.gif");
  char  *lt_page_content;
  int   iFd;
   lt_page_content=ltPltFileParse("/app/msa/icm/htmlplt/reportplt/arpreport.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","�ޱ�������,�޷����ɱ���",caTempDir);
            system(caLabel);/*����Ŀ¼�����ڴ����־*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
      /*����html��ʽ����*/
  }   
    ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >��������������,�뵽�����б��в쿴����״̬�������ر���<br><a href=\"/main/srvreportlist.htm\" >�쿴�±���?</a></body></html>");		

	   //pdf��ʽ�������ɴ���
     sprintf(caLabel,"/app/msa/icm/htmlplt/Script/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
     printf("change to mht :: %s\n",caLabel);
     system(caLabel);
     chdir(caTempDir);
  	 system("tar -cvzf report.tgz *");
	   sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	   system(caLabel);
	   sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	   system(caLabel);	
	   ltMsgFree(ltMsgPk);
     return 0;
  }
	return 0;
}
