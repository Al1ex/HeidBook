#ifndef LTDATE_H
#define LTDATE_H


struct datetime {
  int hour;
  int min;
  int sec;
  int wday;
  int mday;
  int yday;
  int mon;
  int year;
} ;

typedef long datetime_sec;

extern void datetime_tai(struct datetime *dt,datetime_sec t);
extern datetime_sec datetime_untai(struct datetime *dt);

extern unsigned int date822fmt(char *s,struct datetime *dt);
#define DATE822FMT 60

/* Conver the long time to format date 
     %y   ---- 2 bytes years
     %Y   ---- 4 bytes years
     %m   ----- month
     %d   ----- day
     %H   ------ Hours
     %M   ------ mintus
     %S   ------- Second
 */
char *ltTimeFormat(char *pFormat,long lTime);
long ltTime();

/*��һ����ʽ��ʱ��ת��Ϊ�������� */
long ltTimStrToLong(char *fmt,char *instr);
/* ���ַ�����ʱ��yyyy-mm-dd ת��Ϊ yyymmdd  */
int ltTimStr10To8(char *pInTime,char *pFormat);
/*ʱ���ʽyyyymmdd*/
int ltTimeNextDay(char *caInDate,char *caOutDate);
int ltTimePrevDay(char *caInDate,char *caOutDate);
/* ������ڵĺϷ���  
    ��ķ�Χ�� 1900--2100֮��
    >0  -- �Ϸ�
    -1 -- �겻�Ϸ�
    -2 -- �²��Ϸ�
    -3 -- �ղ��Ϸ�
*/
int ltTimIsValidDate(int iYear,int iMonth,int iDay);

#endif


