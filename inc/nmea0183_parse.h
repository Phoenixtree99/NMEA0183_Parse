#ifndef __GPS_H
#define __GPS_H
#include "stm32f4xx.h"

typedef struct{
	u8 SAT_ID;								//���Ǳ��(01~32)[%d]
	u8 SAT_Elevation;					//��������(0~90��)[%d]
	u16 SAT_Azimuth;					//���Ƿ�λ��(0~360��)[%d]
	u8 SAT_CNo;								//�����(0~99dB)[%d]
}GSV;

typedef struct{
	u8 BDGSV_Number;					//�ܵ�GSV��䱨����[%d]
	u8 BD_SAT_Number;					//����BD��������(0~12)[%d]
	GSV BD_SAT_Status[12];		//������12�ű������ǵ�״̬[%d]
}BDGSV;

typedef struct{
	u8 GPGSV_Number;					//�ܵ�GSV��䱨����[%d]
	u8 Current_GPGSV_Number;	//��ǰ��GSV����[%d]
	u8 GPS_SAT_Number;				//����GPS��������(0~12)[%d]
	GSV GPS_SAT_Status[12];		//������12��GPS���ǵ�״̬[%d]
}GPGSV;

typedef struct{
	u16 Year;									//UTC��[%d]
	u8 Month;									//UTC��[%d]
	u8 Day;										//UTC��[%d]
	u8 Hour;									//UTCʱ[%d]
	u8 Min;										//UTC��[%d]
	u8 Sec;										//UTC��[%d]
}UTC;

typedef struct{
	UTC Time;							//UTCʱ��(GGA���Ĳ���������)
	double Lati;					//γ��[%f]
	u8 NorS;							//�ϱ������־:N=������,S=�ϰ���,[%c]
	double Longi;					//����[%f]
	u8 EorW;							//���������־:E=������,W=������[%c]
	u8 PosiQual;					//��λ����ָʾ:0=��λ��Ч,1=��λ��Ч[%d]
	u8 SATNum;						//����ʹ�ý���λ�õ���������(0~12)[%d]
	float HDOP;						//ˮƽ��ȷ��[%.1f]
	float AntAlti;				//�����뺣ƽ��ĸ߶�[%.2f]
	u8 AntAltiUnit;				//�����뺣ƽ��߶ȵ�λ:M��[%c]
	float GeoAlti;				//�����Բ����Ժ�ƽ��ĸ߶�[%.2f]
	u8 GeoAltiUnit;				//�����Բ����Ժ�ƽ��߶ȵ�λ:M��[%c]
}GNGGA;

typedef struct{
	u8 MODE2;							//ģʽ2:M=�ֶ�,A=�Զ�[%c]
	u8 MODE1;							//ģʽ1:1=δ��λ,2=��ά��λ,3=��ά��λ[%d]
	u16 SAT_PRN[12];			//12���ŵ�������PRN��[%d]
	float PDOP;						//�ۺ�λ�þ�������[%.1f]
	float HDOP;						//ˮƽ��ȷ������[%.1f]
	float VDOP;						//��ֱ��ȷ������[%.1f]
}BDGSA;

typedef struct{
	u8 MODE2;							//ģʽ2:M=�ֶ�,A=�Զ�
	u8 MODE1;							//ģʽ1:1=δ��λ,2=��ά��λ,3=��ά��λ
	u16 SAT_PRN[12];			//12���ŵ�������PRN��
	float PDOP;						//�ۺ�λ�þ�������
	float HDOP;						//ˮƽ��ȷ������
	float VDOP;						//��ֱ��ȷ������
}GPGSA;

typedef struct{
	UTC Time;							//UTCʱ��(RMC���İ�������)
	u8 PosStatus;					//��λ״̬:A=��Ч��λ,V=��Ч��λ[%c]
	double Lati;						//γ��[%f]
	u8 NorS;							//�ϱ������־:N=������,S=�ϰ���[%c]
	double Longi;					//����[%f]
	u8 EorW;							//���������־:E=������,W=������[%c]
	float SpeedKont;			//��������(000.0~999.9��)[%.2f]
	float Course;					//���溽��(000.0~359.9��,���汱Ϊ�ο���׼)[%.2f]
	float MagVari;				//��ƫ��(000.0~180.0��)[%.1f]
	u8 Decl;							//��ƫ�Ƿ���:E=��/W=��[%c]
	u8 ModeIndi;					//ģʽָʾ:A=������λ,D=���,E=����,N=������Ч[%c]
}GNRMC;

typedef struct{
	float TrueNCour;			//���溽��(000.0~359.9��,���汱Ϊ�ο���׼)[%.2f]
	float MagNCour;				//���溽��(000.0~359.9��,�Դű�Ϊ�ο���׼)[%.2f]
	float SpeedKont;			//��������(000.0~999.9��)[%.2f]
	float Speedkmps;			//��������(000.0~1851.8����/Сʱ)[%.2f]
	u8 ModeIndi;					//ģʽָʾ:A=������λ,D=���,E=����,N=������Ч[%c]
}GNVTG;

	u8 NMEA0183_CommaAddrOffset(u8 *buf,u8 CommaNumber);
	int GetIntBetweenTwoCommas(u8 *headAddr);
	float GetFloatBetweenTwoCommas(u8 *headAddr);
	double GetDoubleBetweenTwoCommas(u8 *headAddr);
	u8 GetUTCTime(u8 mode,u8 *headAddr);
	u8 GetUTCDate(u8 mode,u8 *headAddr);
	void NMEA0183_BDGSV_Analysis(BDGSV *BDGSV_Info,char *buf);
	void NMEA0183_GPGSV_Analysis(GPGSV *GPGSV_Info,char *buf);
	void NMEA0183_GNGGA_Analusis(GNGGA *GNGGA_Info,char *buf);
	void NMEA0183_BDGSA_Analusis(BDGSA *BDGSA_Info,char *buf);
	void NMEA0183_GPGSA_Analusis(GPGSA *GPGSA_Info,char *buf);
	void NMEA0183_GNRMC_Analusis(GNRMC *GNRMC_Info,char *buf);
	void NMEA0183_GNVTG_Analusis(GNVTG *GNVTG_Info,char *buf);

#endif

