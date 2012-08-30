#ifndef STR_H
#define STR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct {
    long lRand;
    long lNumber;
} ltStrRandNumber;


/*str.c*/
extern unsigned int str_copy(char *,char *);
extern int str_diff(char *,char *);
extern int str_diffn(char *,char *,unsigned int);
extern unsigned int str_len(char *);
extern unsigned int str_chr(char *,int);
extern unsigned int str_rchr(char *,int);
extern int str_start(char *,char *);

#define str_equal(s,t) (!str_diff((s),(t)))
#define ltStrBitIs(x,y) ( (x>>(y-1))&1 )


/*str01.c*/
/*ɾ���ո�*/
char *ltStrDelSpaces(char *pInstr);
/*��д ���س���*/
int ltStrToUpper(char *pInstr);
int ltStrToLower(char *pInstr);
/*��д ������255�ֽ�*/
char *ltStrUpper(char *pInstr);

char *ltStrGetWord(char *p,char *pOut,
       short nMaxLen,char *pTerm);
int ltStrCharIsIn(char *pTerm,char c);
int ltStrIsSpaces(char *pIn);
char *ltStrSkipSpaces(char *pIn);
int ltStrIsSpaces1(char *pIn);
char *ltStrSkipSpaces1(char *pIn);
char *ltStrGetWord1(char *p,char *pOut,
       short nMaxLen,char *pTerm);

int ltStrHanZi(char *pInstr,int iPos);
int ltStrRand(long iNumber,ltStrRandNumber *psStrRand);
int ltStrCompare_001(const void *p1,const void *p2);
char *ltStrAddOneStr(char *pOutStr,char *pInStr,int iLen);
char *ltStrncpy(char *pOutstr,char *pInstr,int iLen);
long ltStrGetId();




/* �ж��ַ����Ƿ�Ϊ��Ч������  �ɴ�ӡ�ĺ��֣�a--z A--Z _ 0-9 . ����ת��Ϊ_*/
int ltStrCvtValidName(unsigned char *caName);
/* �ж��ַ����Ƿ�Ϊ��Ч������  �ɴ�ӡ�ĺ��֣�a--z A--Z _ 0-9 */
int ltStrIsValidName(unsigned char *caName);




/* ȡ��¼���е�����
   iRow --- �ӵ�ǰλ�ÿ�ʼ�ļ�¼���, ��1��ʼ
 */
char *ltDbRecordValue(char *caRecord,int iRow,int iSumField,...);

unsigned long ltFileSkipUntilStr(FILE *fp,char *pTerm,int lTerm);
/* ���ļ��ж�һ����, �� caVar��ͷ, �� pTerm����, ��󳤶�Ϊ iMaxLen

 */
int ltFileReadWord(FILE *fp,
                char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
/* ���ļ��ж�һ����, �� caVar��ͷ, �� pTerm����, ��󳤶�Ϊ iMaxLen

 */
int ltFileReadWordTermByChar(FILE *fp,
                char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm);
char *ltFileReadAllFile(char *pFile,unsigned long *lSize);
int ltStrGetThisWordTermByStr(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,char iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
/* ȡָ�����ַ�������������ָ�� */
char *ltStrGetThisWord(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,int  iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
int ltStrGetThisWordTermByChar(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm);




#endif
