#ifndef  __BSP_GPS_H__
#define  __BSP_GPS_H__

//#include <includes.h>

/* GPS NMEA-0183Э����Ҫ�����ṹ�嶨�� */
/* ������Ϣ */
typedef struct NMEA_SLMSG_STRU
{										    
 	u8 num;								/* ���Ǳ�� */
	u8 eledeg;						/* �������� */
	u16 azideg;						/* ���Ƿ�λ�� */
	u8 sn;								/* ����� */		   
}NMEA_SLMSG;  

/* UTCʱ����Ϣ */
typedef struct NMEA_UTC_TIME_STRU
{										    
 	u16 year;							/* ��� */
	u8 month;							/* �·� */
	u8 date;							/* ���� */
	u8 hour; 							/* Сʱ */
	u8 min; 							/* ���� */
	u8 sec; 							/* ���� */
}NMEA_UTC_TIME;   

/* GPS������Ϣ */
typedef struct NMEA_MSG_STRU 
{										    
 	u8 svnum;					         /* �ɼ������� */
	NMEA_SLMSG slmsg[12];			 /* ���12������ */
	NMEA_UTC_TIME utc;			   /* UTCʱ�� */
	u32 latitude;				       /* γ�� ������100000��,ʵ��Ҫ����100000 */
	u8 nshemi;								 /* ��γ/��γ,N:��γ;S:��γ */				  
	u32 longitude;			    	 /* ���� ������100000��,ʵ��Ҫ����100000 */
	u8 ewhemi;								 /* ����/����,E:����;W:���� */
	u8 gpssta;								 /* GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ��� */				  
 	u8 posslnum;							 /* ���ڶ�λ��������,0~12 */
 	u8 possl[12];							 /* ���ڶ�λ�����Ǳ�� */
	u8 fixmode;								 /* ��λ����:1,û�ж�λ;2,2D��λ;3,3D��λ */
	u16 pdop;									 /* λ�þ������� 0~500,��Ӧʵ��ֵ0~50.0 */
	u16 hdop;									 /* ˮƽ�������� 0~500,��Ӧʵ��ֵ0~50.0 */
	u16 vdop;									 /* ��ֱ�������� 0~500,��Ӧʵ��ֵ0~50.0 */
	int altitude;			 			   /* ���θ߶�,�Ŵ���10��,ʵ�ʳ���10.��λ:0.1m */
	u16 speed;								 /* ��������,�Ŵ���1000��,ʵ�ʳ���10.��λ:0.001����/Сʱ */ 
}NMEA_MSG;

void GPS_Analysis(NMEA_MSG *gpsx,u8 *buf);
	

#endif
