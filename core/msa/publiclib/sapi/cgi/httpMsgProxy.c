#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include "libxml/tree.h"
#include "libxml/parser.h"
#include "publiclib.h" 
#include "ltmsg.h"
#include "ltcgi.h"



int ltGetCgiValue(char *filename,ltCgiConfInfo *psCgiConf){
	xmlDocPtr doc;
	xmlNodePtr  node;
	xmlNodePtr  childnode;
  xmlNodePtr  tempRootNode;
	char *nodeValue;
	char *nodeName;
	char *nodeType;
	char *tempValue;
	doc=xmlParseFile(filename); 
	if(doc==NULL){
		return -1;
	}
	tempRootNode=xmlDocGetRootElement(doc);
	for(node = tempRootNode->children; node != NULL; node = node->next) {
		if(node->type==1){	
			
			if(!case_diffs(node->name,"server")){
				for(childnode=node->children;childnode!=NULL;childnode=childnode->next){
				 if(childnode->type==1){
         if(!case_diffs(childnode->name,"parameter")){
					nodeName=xmlGetProp(childnode,"name");
					nodeValue=xmlGetProp(childnode,"value");
					if(!case_diffs(nodeName,"ipaddr")){
						snprintf(psCgiConf->ipAddr,sizeof(psCgiConf->ipAddr),"%s",	nodeValue);
					}else if(!case_diffs(nodeName,"port")){
						psCgiConf->nPort=atol(nodeValue);
					}else if(!case_diffs(nodeName,"timeout")){
						psCgiConf->lTimeOut=atol(nodeValue);	
				        }else if(!case_diffs(nodeName,"uploadpath")){
						sprintf(psCgiConf->upLoadPath,"%s",	nodeValue);
					}else if(!case_diffs(nodeName,"msgcode")){
						psCgiConf->lMsgCode=atol(nodeValue);
					}else if(!case_diffs(nodeName,"maxmsg")){
						psCgiConf->lMaxMsgPkg=atol(nodeValue);
					}else{
						xmlFreeDoc(doc);
						return -1;
					}
				}
				}
				}
			}else{
				xmlFreeDoc(doc);
				return -1;
			}
		break;
		}
  }
	xmlFreeDoc(doc);
	return 0;
} 



main(int argc,char *argv[])
{
    struct stat buf;
    char *pTemp;
    char *pathInfo;
    int  pathInfoLen;
    char *pathInfo1;
    int  pathInfoLen1;
    /*ϵͳ��·��*/
    char rootPath[256];
    /*ϵͳ��ǰ·��*/
    char curPath[256];
    /*ϵͳ�ĸ�·��*/
    char parPath[256];

    char *p;
    int iReturn;
    int iRead,l;
    int lenTemp;
    ltCgiConfInfo   sCgiConf;               /* ������Ϣ   */
  

   /*�õ�������Ϣ*/
   
    /* �������ļ�  */
    memset((char *)&sCgiConf,0,sizeof(ltCgiConfInfo));
    sprintf(sCgiConf.upLoadPath,"/tmp/upload/");
    sCgiConf.lTimeOut = 60;
    sCgiConf.lMaxMsgPkg = 0;
    
					//��ȡxml������Ϣ
		if( ltGetCgiValue("/app/ns/app/xp.conf",&sCgiConf)!=0){
						    printf("Content-type:text/html\n\n");
				    		printf("Internal server error: ���������ô��������ļ�����\n");
			          exit(0);
		}
			  
			  
  
/*  ��ʾ������Ϣ  */

   /*
   printf("Content-type:text/plain\n\n");
   printf("curPath:%s\n",curPath);
   printf("rootPath:%s\n",rootPath);
   printf("pathinfo:%s\n",pathInfo);
   printf("parPath:%s\n",parPath);
   printf("msgcode:%d\n",sCgiConf.lMsgCode);
   printf("SERVERIP:%s\n",sCgiConf.ipAddr);
 */
    //printf("Content-type:text/plain\n\n");
    //printf("pathinfo:%s\n",pathInfo1);
    

    lt_SetMaxTcpBuf(sCgiConf.lMaxMsgPkg);
    
    ltCgiDoHttpFun(&sCgiConf,"/");

    exit(0);
}   
    
    
    
   
