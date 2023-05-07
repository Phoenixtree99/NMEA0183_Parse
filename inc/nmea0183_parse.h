#ifndef __NMEA0183_PARSE_H
#define __NMEA0183_PARSE_H

typedef struct{
	unsigned char SAT_ID;								//���Ǳ��(01~32)[%d]
	unsigned char SAT_Elevation;					//��������(0~90��)[%d]
	unsigned short SAT_Azimuth;					//���Ƿ�λ��(0~360��)[%d]
	unsigned char SAT_CNo;								//�����(0~99dB)[%d]
}GSV;

typedef struct{
	unsigned char BDGSV_Number;					//�ܵ�GSV��䱨����[%d]
	unsigned char BD_SAT_Number;					//����BD��������(0~12)[%d]
	GSV BD_SAT_Status[12];		//������12�ű������ǵ�״̬[%d]
}BDGSV;

typedef struct{
	unsigned char GPGSV_Number;					//�ܵ�GSV��䱨����[%d]
	unsigned char Current_GPGSV_Number;	//��ǰ��GSV����[%d]
	unsigned char GPS_SAT_Number;				//����GPS��������(0~12)[%d]
	GSV GPS_SAT_Status[12];		//������12��GPS���ǵ�״̬[%d]
}GPGSV;

typedef struct{
	unsigned short Year;									//UTC��[%d]
	unsigned char Month;									//UTC��[%d]
	unsigned char Day;										//UTC��[%d]
	unsigned char Hour;									//UTCʱ[%d]
	unsigned char Min;										//UTC��[%d]
	unsigned char Sec;										//UTC��[%d]
}UTC;

typedef struct{
	UTC Time;							//UTCʱ��(GGA���Ĳ���������)
	double Lati;					//γ��[%f]
	unsigned char NorS;							//�ϱ������־:N=������,S=�ϰ���,[%c]
	double Longi;					//����[%f]
	unsigned char EorW;							//���������־:E=������,W=������[%c]
	unsigned char PosiQual;					//��λ����ָʾ:0=��λ��Ч,1=��λ��Ч[%d]
	unsigned char SATNum;						//����ʹ�ý���λ�õ���������(0~12)[%d]
	float HDOP;						//ˮƽ��ȷ��[%.1f]
	float AntAlti;				//�����뺣ƽ��ĸ߶�[%.2f]
	unsigned char AntAltiUnit;				//�����뺣ƽ��߶ȵ�λ:M��[%c]
	float GeoAlti;				//�����Բ����Ժ�ƽ��ĸ߶�[%.2f]
	unsigned char GeoAltiUnit;				//�����Բ����Ժ�ƽ��߶ȵ�λ:M��[%c]
}GNGGA;

typedef struct{
	unsigned char MODE2;							//ģʽ2:M=�ֶ�,A=�Զ�[%c]
	unsigned char MODE1;							//ģʽ1:1=δ��λ,2=��ά��λ,3=��ά��λ[%d]
	unsigned char SAT_PRN[12];			//12���ŵ�������PRN��[%d]
	float PDOP;						//�ۺ�λ�þ�������[%.1f]
	float HDOP;						//ˮƽ��ȷ������[%.1f]
	float VDOP;						//��ֱ��ȷ������[%.1f]
}BDGSA;

typedef struct{
	unsigned char MODE2;							//ģʽ2:M=�ֶ�,A=�Զ�
	unsigned char MODE1;							//ģʽ1:1=δ��λ,2=��ά��λ,3=��ά��λ
	unsigned short SAT_PRN[12];			//12���ŵ�������PRN��
	float PDOP;						//�ۺ�λ�þ�������
	float HDOP;						//ˮƽ��ȷ������
	float VDOP;						//��ֱ��ȷ������
}GPGSA;

typedef struct{
	UTC Time;							//UTCʱ��(RMC���İ�������)
	unsigned char PosStatus;					//��λ״̬:A=��Ч��λ,V=��Ч��λ[%c]
	double Lati;						//γ��[%f]
	unsigned char NorS;							//�ϱ������־:N=������,S=�ϰ���[%c]
	double Longi;					//����[%f]
	unsigned char EorW;							//���������־:E=������,W=������[%c]
	float SpeedKont;			//��������(000.0~999.9��)[%.2f]
	float Course;					//���溽��(000.0~359.9��,���汱Ϊ�ο���׼)[%.2f]
	float MagVari;				//��ƫ��(000.0~180.0��)[%.1f]
	unsigned char Decl;							//��ƫ�Ƿ���:E=��/W=��[%c]
	unsigned char ModeIndi;					//ģʽָʾ:A=������λ,D=���,E=����,N=������Ч[%c]
}GNRMC;

typedef struct{
	float TrueNCour;			//���溽��(000.0~359.9��,���汱Ϊ�ο���׼)[%.2f]
	float MagNCour;				//���溽��(000.0~359.9��,�Դű�Ϊ�ο���׼)[%.2f]
	float SpeedKont;			//��������(000.0~999.9��)[%.2f]
	float Speedkmps;			//��������(000.0~1851.8����/Сʱ)[%.2f]
	unsigned char ModeIndi;					//ģʽָʾ:A=������λ,D=���,E=����,N=������Ч[%c]
}GNVTG;

	unsigned char NMEA0183_CommaAddrOffset(unsigned char *buf,unsigned char CommaNumber);
	int GetIntBetweenTwoCommas(unsigned char *headAddr);
	float GetFloatBetweenTwoCommas(unsigned char *headAddr);
	double GetDoubleBetweenTwoCommas(unsigned char *headAddr);
	unsigned char GetUTCTime(unsigned char mode,unsigned char *headAddr);
	unsigned char GetUTCDate(unsigned char mode,unsigned char *headAddr);
	void NMEA0183_BDGSV_Analysis(BDGSV *BDGSV_Info,char *buf);
	void NMEA0183_GPGSV_Analysis(GPGSV *GPGSV_Info,char *buf);
	void NMEA0183_GNGGA_Analusis(GNGGA *GNGGA_Info,char *buf);
	void NMEA0183_BDGSA_Analusis(BDGSA *BDGSA_Info,char *buf);
	void NMEA0183_GPGSA_Analusis(GPGSA *GPGSA_Info,char *buf);
	void NMEA0183_GNRMC_Analusis(GNRMC *GNRMC_Info,char *buf);
	void NMEA0183_GNVTG_Analusis(GNVTG *GNVTG_Info,char *buf);

#endif

