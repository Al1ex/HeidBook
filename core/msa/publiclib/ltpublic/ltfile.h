#ifndef LTFILE_H
#define LTFILE_H

/*�õ�һ���ļ��Ĵ�С*/
long ltFileSize(char *pFile);
/*�ж�һ���ļ��Ƿ���� ���ڷ���1 ���淵��-1*/
int ltFileIsExist(char *pFile);
/*�ļ�����*/
int  ltFileCopy(char *pSfile,char *pDfile);

/* ɾ��ָ���ļ��е�����  */
int ltDeleteDir(char *pDir,
                 unsigned long lStartTime,
                 unsigned long lStopTime);

/* ����ǰĿ¼�µ��ļ�ȫ��ɾ���󣬽���ǰĿ¼Ҳɾ��  */
int ltDeleteDir_p(char *pDir,
                 unsigned long lStartTime,
                 unsigned long lStopTime);


/* ɾ��ָ���ļ��е�����  */
int ltRmDir(char *pDir);
/* �����豸��ȡʣ����̿ռ�  ����*/
int ltFsGetDiskSpaceD(char *pDev,long *lTotal,long *lUsed,long *lFree);

/*���*/
int ltFsGetDiskSpaceF(char *pMount,long *lTotal,long *lUsed,long *lFree);


/* ���ļ������һ�� tar ��ʽ  
   ����: pTarFile ---- ���ɵ��ļ�, ������ .tar��β
         pFlst    ---- �ļ��б�, ����Ϊ����ļ����ļ���֮���Կո�ָ�
   ����ֵ:
         0 --- ��ȷ
         ����---����
*/
int ltTarTxtFile(char *pTarFile,char *pFlst);
/* �� Dos���ı��ļ���ʽת��ΪUNIX���ı��ļ���ʽ  */
int ltTxtDos2Unix(char *pDosFile,char *pUnixFile);

/* ��UNIX��ʽ���ı��ļ�ת��ΪDos��ʽ���ı��ļ� */
int ltTxtUnix2Dos(char *pUnixFile,char *pDosFile);
/* ���ļ������һ�� tar ��ʽ  
   ����: pTarFile ---- ���ɵ��ļ�, ������ .tar��β
         pFlst    ---- �ļ��б�, ����Ϊ����ļ����ļ���֮���Կո�ָ�
   ����ֵ:
         0 --- ��ȷ
         ����---����
*/
int ltTarDirFile(char *pTarFile,char *pPath,char *pFlst);

/* �����ϴ����ļ���  */
int ltStrGetUploadName(char *pIn,char *pPath,char *pFile);
#endif


