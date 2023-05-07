#include "gps.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "USART1.h"

/**
  * @brief 获得buf中第CommaNumber个逗号的地址与buf首地址的偏移量
  * @param *buf:需要检索的字符串指针
	* @param CommaNumber:需要检索第几个逗号的偏移量
  * @retval 0~0xfe,所检索逗号的地址与buf首地址的偏移量;0xff不存在所要检索的逗号
	*/
u8 NMEA0183_CommaAddrOffset(u8 *buf,u8 CommaNumber){	 		    
	u8 *p = buf;
	while(CommaNumber){		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0Xff;
		if(*buf==',')CommaNumber--;
		buf++;
	}
	return buf-p;
}

/**
  * @brief 获得相邻两个逗号之间的字符串并转换成整型
	* @param *headAddr:字符串首地址
  * @retval 两逗号间的整型数据
	*/
int GetIntBetweenTwoCommas(u8 *headAddr){
	u8 Integer[10]="";																//存放整数的字符串
	u8 i=0;
	while(*headAddr != ',' && *headAddr != '*'){	//没有到下一个','或'*'
		Integer[i++] = *headAddr;										//获取字符形成字符串
		headAddr++;																	//地址偏移
	}
	return atoi((const char *)Integer);						//字符串转整型
}

/**
  * @brief 获得相邻两个逗号之间的字符串并转换成浮点型
	* @param *headAddr:字符串首地址
  * @retval 两逗号间的浮点型数据
	*/
float GetFloatBetweenTwoCommas(u8 *headAddr){
	u8 Decimal[15]="";																//存放整数的字符串
	u8 i=0;
	while(*headAddr != ',' && *headAddr != '*'){	//没有到下一个','或'*'
		Decimal[i++] = *headAddr;										//获取字符形成字符串
		headAddr++;																	//地址偏移
	}
	return atof((const char *)Decimal);						//字符串转浮点型
}

/**
  * @brief 获得相邻两个逗号之间的字符串并转换成双精度浮点型
	* @param *headAddr:字符串首地址
  * @retval 两逗号间的双精度浮点型数据
	*/
double GetDoubleBetweenTwoCommas(u8 *headAddr){
	u8 Decimal[15]="";																//存放整数的字符串
	u8 i=0;
	while(*headAddr != ',' && *headAddr != '*'){	//没有到下一个','或'*'
		Decimal[i++] = *headAddr;										//获取字符形成字符串
		headAddr++;																	//地址偏移
	}
	return atof((const char *)Decimal);						//字符串转浮点型
}

/**
  * @brief 整理NMEA0183格式的UTC字符串
	* @param mode:模式选择,1=返回小时,2=返回分钟,3=返回秒钟
	* @param *headAddr:字符串首地址
  * @retval 根据mode返回相应的数据
	*/
u8 GetUTCTime(u8 mode,u8 *headAddr){
	u8 UTCHourStr[2]="";
	u8 UTCMinStr[2]="";
	u8 UTCSecStr[2]="";
	u8 i=0,j=0,k=0;
	while(*headAddr != '.'){
		if(i<2){
			UTCHourStr[i] = *headAddr;
			headAddr++;
		}else if(i>=2 && i<4){
			UTCMinStr[j++] = *headAddr;
			headAddr++;
		}else if(i>=4 && i<6){
			UTCSecStr[k++] = *headAddr;
			headAddr++;
		}
		i++;
	}
	switch(mode){
		case 1:return (u8)atoi((const char *)UTCHourStr);
		case 2:return (u8)atoi((const char *)UTCMinStr);
		case 3:return (u8)atoi((const char *)UTCSecStr);
		default:return 0;
	}
}

/**
  * @brief 整理NMEA0183格式的UTC字符串
	* @param mode:模式选择,1=返回年份,2=返回月份,3=返回日期
	* @param *headAddr:字符串首地址
  * @retval 根据mode返回相应的数据
	*/
u8 GetUTCDate(u8 mode,u8 *headAddr){
	u8 UTCYearStr[2]="";
	u8 UTCMouthStr[2]="";
	u8 UTCDayStr[2]="";
	u8 i=0,j=0,k=0;
	while(*headAddr != ','){
		if(i<2){
			UTCDayStr[i] = *headAddr;
			headAddr++;
		}else if(i>=2 && i<4){
			UTCMouthStr[j++] = *headAddr;
			headAddr++;
		}else if(i>=4 && i<6){
			UTCYearStr[k++] = *headAddr;
			headAddr++;
		}
		i++;
	}
	switch(mode){
		case 1:return (u8)atoi((const char *)UTCYearStr);
		case 2:return (u8)atoi((const char *)UTCMouthStr);
		case 3:return (u8)atoi((const char *)UTCDayStr);
		default:return 0;
	}
}

/**
  * @brief 解析BDGSV报文
	* @param *BDGSV_Info:存储BDGSV数据的结构体指针
	* @param *buf:存放NMEA0183报文的缓存区指针
  * @retval None
	*/
void NMEA0183_BDGSV_Analysis(BDGSV *BDGSV_Info,char *buf){
	u8 *p;
	u8 offset=0;
	u8 i = 0,j = 0;
	p = (u8 *)strstr((const char *)buf,"$BDGSV");	//在NMEA0183报文里寻找"$BDGSV"并返回其地址
	BDGSV_Info->BDGSV_Number = p[7] - '0';				//获得总的BDGSV报文数
	offset = NMEA0183_CommaAddrOffset(p,3);
	if(offset != 0xff) BDGSV_Info->BD_SAT_Number = GetIntBetweenTwoCommas(p+offset);
	for(i=0;i<BDGSV_Info->BDGSV_Number;i++){
		for(j=0;j<4;j++){		//每条BDGSV报文最多包含四颗卫星的数据(卫星编号、仰角、方位角、信噪比)
			offset = NMEA0183_CommaAddrOffset(p,4+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_ID = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,5+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_Elevation = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,6+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_Azimuth = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,7+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_CNo = GetIntBetweenTwoCommas(p+offset);
		}
		p = (u8 *)strstr((const char *)p,"\n")+1;		//将p指向下一个"$BDGSV"的首地址
	}
}

/**
  * @brief 解析GPGSV报文
	* @param *GPGSV_Info:存储GPGSV数据的结构体指针
	* @param *buf:存放NMEA0183报文的缓存区指针
  * @retval None
	*/
void NMEA0183_GPGSV_Analysis(GPGSV *GPGSV_Info,char *buf){
	u8 *p;
	u8 offset=0;
	u8 i = 0,j = 0;
	p = (u8 *)strstr((const char *)buf,"$GPGSV");	//在NMEA0183报文里寻找"$GPGSV"并返回其地址
	GPGSV_Info->GPGSV_Number = p[7] - '0';				//获得总的GPGSV报文数
	offset = NMEA0183_CommaAddrOffset(p,3);
	if(offset != 0xff) GPGSV_Info->GPS_SAT_Number = GetIntBetweenTwoCommas(p+offset);
	for(i=0;i<GPGSV_Info->GPGSV_Number;i++){
		for(j=0;j<4;j++){		//每条GPGSV报文最多包含四颗卫星的数据(卫星编号、仰角、方位角、信噪比)
			offset = NMEA0183_CommaAddrOffset(p,4+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_ID = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,5+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_Elevation = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,6+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_Azimuth = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,7+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_CNo = GetIntBetweenTwoCommas(p+offset);
		}
		p = (u8 *)strstr((const char *)p,"\n")+1;		//将p指向下一个"$GPGSV"的首地址
	}
}

/**
  * @brief 解析GNGGA报文
	* @param *GNGGA_Info:存储GNGGA数据的结构体指针
	* @param *buf:存放NMEA0183报文的缓存区指针
  * @retval None
	*/
void NMEA0183_GNGGA_Analusis(GNGGA *GNGGA_Info,char *buf){
	u8 *p;
	u8 offset=0;
	p = (u8 *)strstr((const char *)buf,"$GNGGA");	//在NMEA0183报文里寻找"$GNGGA"并返回其地址
	offset = NMEA0183_CommaAddrOffset(p,1);
	if(offset != 0xff){
		GNGGA_Info->Time.Hour = GetUTCTime(1,p+offset);
		GNGGA_Info->Time.Min = GetUTCTime(2,p+offset);
		GNGGA_Info->Time.Sec = GetUTCTime(3,p+offset);
	}
	offset = NMEA0183_CommaAddrOffset(p,2);
	if(offset != 0xff) GNGGA_Info->Lati = GetDoubleBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,3);
	if(offset != 0xff) GNGGA_Info->NorS = *(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,4);
	if(offset != 0xff) GNGGA_Info->Longi = GetDoubleBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,5);
	if(offset != 0xff) GNGGA_Info->EorW = *(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,6);
	if(offset != 0xff) GNGGA_Info->PosiQual = GetIntBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,7);
	if(offset != 0xff) GNGGA_Info->SATNum = GetIntBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,8);
	if(offset != 0xff) GNGGA_Info->HDOP = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,9);
	if(offset != 0xff) GNGGA_Info->AntAlti = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,10);
	if(offset != 0xff) GNGGA_Info->AntAltiUnit = *(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,11);
	if(offset != 0xff) GNGGA_Info->GeoAlti = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,12);
	if(offset != 0xff) GNGGA_Info->GeoAltiUnit = *(p+offset);
	//UART1_Put_String((unsigned char *)p+offset);
}

/**
  * @brief 解析BDGSA报文
	* @param *BDGSA_Info:存储BDGSA数据的结构体指针
	* @param *buf:存放NMEA0183报文的缓存区指针
  * @retval None
	*/
void NMEA0183_BDGSA_Analusis(BDGSA *BDGSA_Info,char *buf){
	u8 *p;
	u8 offset=0;
	u8 i;
	p = (u8 *)strstr((const char *)buf,"$BDGSA");	//在NMEA0183报文里寻找"$BDGSA"并返回其地址
	offset = NMEA0183_CommaAddrOffset(p,1);
	if(offset != 0xff) BDGSA_Info->MODE2 = *(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,2);
	if(offset != 0xff) BDGSA_Info->MODE1 = GetIntBetweenTwoCommas(p+offset);
	for(i=0;i<12;i++){
		offset = NMEA0183_CommaAddrOffset(p,3+i);
		if(offset != 0xff) BDGSA_Info->SAT_PRN[i] = GetIntBetweenTwoCommas(p+offset);
	}
	offset = NMEA0183_CommaAddrOffset(p,15);
	if(offset != 0xff) BDGSA_Info->PDOP = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,16);
	if(offset != 0xff) BDGSA_Info->HDOP = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,17);
	if(offset != 0xff) BDGSA_Info->VDOP = GetFloatBetweenTwoCommas(p+offset);
}

/**
  * @brief 解析GPGSA报文
	* @param *GPGSA_Info:存储GPGSA数据的结构体指针
	* @param *buf:存放NMEA0183报文的缓存区指针
  * @retval None
	*/
void NMEA0183_GPGSA_Analusis(GPGSA *GPGSA_Info,char *buf){
	u8 *p;
	u8 offset=0;
	u8 i;
	p = (u8 *)strstr((const char *)buf,"$GPGSA");	//在NMEA0183报文里寻找"$BDGSA"并返回其地址
	offset = NMEA0183_CommaAddrOffset(p,1);
	if(offset != 0xff) GPGSA_Info->MODE2 = *(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,2);
	if(offset != 0xff) GPGSA_Info->MODE1 = GetIntBetweenTwoCommas(p+offset);
	for(i=0;i<12;i++){
		offset = NMEA0183_CommaAddrOffset(p,3+i);
		if(offset != 0xff) GPGSA_Info->SAT_PRN[i] = GetIntBetweenTwoCommas(p+offset);
	}
	offset = NMEA0183_CommaAddrOffset(p,15);
	if(offset != 0xff) GPGSA_Info->PDOP = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,16);
	if(offset != 0xff) GPGSA_Info->HDOP = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,17);
	if(offset != 0xff) GPGSA_Info->VDOP = GetFloatBetweenTwoCommas(p+offset);
}

/**
  * @brief 解析GNRMC报文
	* @param *GNRMC_Info:存储GNRMC数据的结构体指针
	* @param *buf:存放NMEA0183报文的缓存区指针
  * @retval None
	*/
void NMEA0183_GNRMC_Analusis(GNRMC *GNRMC_Info,char *buf){
	u8 *p;
	u8 offset=0;
	p = (u8 *)strstr((const char *)buf,"$GNRMC");	//在NMEA0183报文里寻找"$GNRMC"并返回其地址
	offset = NMEA0183_CommaAddrOffset(p,1);
	if(offset != 0xff){
		GNRMC_Info->Time.Hour = GetUTCTime(1,p+offset);
		GNRMC_Info->Time.Min = GetUTCTime(2,p+offset);
		GNRMC_Info->Time.Sec = GetUTCTime(3,p+offset);
	}
	offset = NMEA0183_CommaAddrOffset(p,2);
	if(offset != 0xff) GNRMC_Info->PosStatus = *(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,3);
	if(offset != 0xff) GNRMC_Info->Lati = GetDoubleBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,4);
	if(offset != 0xff) GNRMC_Info->NorS = *(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,5);
	if(offset != 0xff) GNRMC_Info->Longi = GetDoubleBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,6);
	if(offset != 0xff) GNRMC_Info->EorW = *(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,7);
	if(offset != 0xff) GNRMC_Info->SpeedKont = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,8);
	if(offset != 0xff) GNRMC_Info->Course = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,9);
	if(offset != 0xff){
		GNRMC_Info->Time.Year = GetUTCDate(1,p+offset);
		GNRMC_Info->Time.Month = GetUTCDate(2,p+offset);
		GNRMC_Info->Time.Day = GetUTCDate(3,p+offset);
	}
	offset = NMEA0183_CommaAddrOffset(p,10);
	if(offset != 0xff) GNRMC_Info->MagVari = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,11);
	if(offset != 0xff) GNRMC_Info->Decl = *(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,12);
	if(offset != 0xff) GNRMC_Info->ModeIndi = *(p+offset);
}

/**
  * @brief 解析GNVTG报文
	* @param *GNVTG_Info:存储GNVTG数据的结构体指针
	* @param *buf:存放NMEA0183报文的缓存区指针
  * @retval None
	*/
void NMEA0183_GNVTG_Analusis(GNVTG *GNVTG_Info,char *buf){
	u8 *p;
	u8 offset=0;
	p = (u8 *)strstr((const char *)buf,"$GNVTG");	//在NMEA0183报文里寻找"$GNRMC"并返回其地址
	offset = NMEA0183_CommaAddrOffset(p,1);
	if(offset != 0xff) GNVTG_Info->TrueNCour = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,3);
	if(offset != 0xff) GNVTG_Info->MagNCour = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,5);
	if(offset != 0xff) GNVTG_Info->SpeedKont = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,7);
	if(offset != 0xff) GNVTG_Info->Speedkmps = GetFloatBetweenTwoCommas(p+offset);
	offset = NMEA0183_CommaAddrOffset(p,9);
	if(offset != 0xff) GNVTG_Info->ModeIndi = *(p+offset);
}

