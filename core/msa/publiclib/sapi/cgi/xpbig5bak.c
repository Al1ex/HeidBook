#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include "libxml/tree.h"
#include "libxml/parser.h"
#include "ltpublic.h" 
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
    char fileTemp[256];
    char confPath[256];

    /*�õ�����·��*/
    pathInfo1 = getenv("PATH_TRANSLATED");
    pathInfo=getenv("PATH_INFO");
    

    if(pathInfo1 == NULL || pathInfo == NULL) {
	printf("Content-type:text/html\n\n");
        printf("Internal server error: ҳ�治���� \n");
        exit(0);
    }
    pathInfoLen=strlen(pathInfo);
    pathInfoLen1=strlen(pathInfo1);
    
    memset(curPath,0,sizeof(curPath));
    p=strrchr(pathInfo1,'/');
    if(p==NULL){
	    printf("Content-type:text/html\n\n");
	    printf("Internal server error: ҳ��·������ \n");
            exit(0);
   }
   if( (p-pathInfo1+1)>sizeof(curPath) ){
	    printf("Content-type:text/html\n\n");
	    printf("Internal server error: ҳ��·��̫�� \n");
            exit(0);
   }
   strncpy(curPath,pathInfo1,p-pathInfo1+1);
   memset(rootPath,0,sizeof(rootPath));
   strncpy(rootPath,pathInfo1,pathInfoLen1-pathInfoLen+1);
   memset(parPath,0,sizeof(parPath));
   p=strchr(pathInfo+1,'/');
   if(p==NULL){
	 strncpy(parPath,rootPath,strlen(rootPath));
   }else{
	strncpy(parPath,pathInfo1, pathInfoLen1- (pathInfoLen-(p -pathInfo)) +1);
   }

   /*�õ�������Ϣ*/
   
    /* �������ļ�  */
    memset((char *)&sCgiConf,0,sizeof(ltCgiConfInfo));
    sprintf(sCgiConf.upLoadPath,"/tmp/upload/");
    sCgiConf.lTimeOut = 60;
    sCgiConf.lMaxMsgPkg = 0;
    

    memset(fileTemp,0,sizeof(fileTemp));
    memset(confPath,0,sizeof(confPath));
    /*��ǰ·����������Ϣ*/
    sprintf(fileTemp,"%s%s",curPath,"xp.ini");

    if( ltFileIsExist(fileTemp)==1){
	if( ltCnfGetValue_s(fileTemp,"xpcnf",confPath)==1){
		//��ȡxml������Ϣ
		if( ltGetCgiValue(confPath,&sCgiConf)!=0){
			printf("Content-type:text/html\n\n");
	    		printf("Internal server error: ���������ô��������ļ�����\n");
            		exit(0);
		}
        }
	else{
	    printf("Content-type:text/html\n\n");
	    printf("Internal server error: ���������ô����Ҳ���������Ϣ\n");
            exit(0);
	}
    }else{
            printf("Content-type:text/html\n\n");
	    printf("Internal server error: ���������ô����Ҳ���������Ϣ\n");
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
    
    ltCgiDoMuti(&sCgiConf,pathInfo1);

    exit(0);
}   
    
    
    
   
