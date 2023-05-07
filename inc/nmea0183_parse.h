#ifndef __GPS_H
#define __GPS_H
#include "stm32f4xx.h"

typedef struct{
	u8 SAT_ID;								//卫星编号(01~32)[%d]
	u8 SAT_Elevation;					//卫星仰角(0~90°)[%d]
	u16 SAT_Azimuth;					//卫星方位角(0~360°)[%d]
	u8 SAT_CNo;								//信噪比(0~99dB)[%d]
}GSV;

typedef struct{
	u8 BDGSV_Number;					//总的GSV语句报文数[%d]
	u8 BD_SAT_Number;					//可视BD卫星总数(0~12)[%d]
	GSV BD_SAT_Status[12];		//存放最多12颗北斗卫星的状态[%d]
}BDGSV;

typedef struct{
	u8 GPGSV_Number;					//总的GSV语句报文数[%d]
	u8 Current_GPGSV_Number;	//当前的GSV语句号[%d]
	u8 GPS_SAT_Number;				//可视GPS卫星总数(0~12)[%d]
	GSV GPS_SAT_Status[12];		//存放最多12颗GPS卫星的状态[%d]
}GPGSV;

typedef struct{
	u16 Year;									//UTC年[%d]
	u8 Month;									//UTC月[%d]
	u8 Day;										//UTC日[%d]
	u8 Hour;									//UTC时[%d]
	u8 Min;										//UTC分[%d]
	u8 Sec;										//UTC秒[%d]
}UTC;

typedef struct{
	UTC Time;							//UTC时间(GGA报文不包含日期)
	double Lati;					//纬度[%f]
	u8 NorS;							//南北半球标志:N=北半球,S=南半球,[%c]
	double Longi;					//经度[%f]
	u8 EorW;							//东西半球标志:E=东半球,W=西半球[%c]
	u8 PosiQual;					//定位质量指示:0=定位无效,1=定位有效[%d]
	u8 SATNum;						//正在使用解算位置的卫星数量(0~12)[%d]
	float HDOP;						//水平精确度[%.1f]
	float AntAlti;				//天线离海平面的高度[%.2f]
	u8 AntAltiUnit;				//天线离海平面高度单位:M米[%c]
	float GeoAlti;				//大地椭圆面相对海平面的高度[%.2f]
	u8 GeoAltiUnit;				//大地椭圆面相对海平面高度单位:M米[%c]
}GNGGA;

typedef struct{
	u8 MODE2;							//模式2:M=手动,A=自动[%c]
	u8 MODE1;							//模式1:1=未定位,2=二维定位,3=三维定位[%d]
	u16 SAT_PRN[12];			//12个信道的卫星PRN码[%d]
	float PDOP;						//综合位置精度因子[%.1f]
	float HDOP;						//水平精确度因子[%.1f]
	float VDOP;						//垂直精确度因子[%.1f]
}BDGSA;

typedef struct{
	u8 MODE2;							//模式2:M=手动,A=自动
	u8 MODE1;							//模式1:1=未定位,2=二维定位,3=三维定位
	u16 SAT_PRN[12];			//12个信道的卫星PRN码
	float PDOP;						//综合位置精度因子
	float HDOP;						//水平精确度因子
	float VDOP;						//垂直精确度因子
}GPGSA;

typedef struct{
	UTC Time;							//UTC时间(RMC报文包含日期)
	u8 PosStatus;					//定位状态:A=有效定位,V=无效定位[%c]
	double Lati;						//纬度[%f]
	u8 NorS;							//南北半球标志:N=北半球,S=南半球[%c]
	double Longi;					//经度[%f]
	u8 EorW;							//东西半球标志:E=东半球,W=西半球[%c]
	float SpeedKont;			//地面速率(000.0~999.9节)[%.2f]
	float Course;					//地面航向(000.0~359.9°,以真北为参考基准)[%.2f]
	float MagVari;				//磁偏角(000.0~180.0°)[%.1f]
	u8 Decl;							//磁偏角方向:E=东/W=西[%c]
	u8 ModeIndi;					//模式指示:A=自主定位,D=差分,E=估算,N=数据无效[%c]
}GNRMC;

typedef struct{
	float TrueNCour;			//地面航向(000.0~359.9°,以真北为参考基准)[%.2f]
	float MagNCour;				//地面航向(000.0~359.9°,以磁北为参考基准)[%.2f]
	float SpeedKont;			//地面速率(000.0~999.9节)[%.2f]
	float Speedkmps;			//地面速率(000.0~1851.8公里/小时)[%.2f]
	u8 ModeIndi;					//模式指示:A=自主定位,D=差分,E=估算,N=数据无效[%c]
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

