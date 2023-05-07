#include "gps.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "USART1.h"

/**
  * @brief ���buf�е�CommaNumber�����ŵĵ�ַ��buf�׵�ַ��ƫ����
  * @param *buf:��Ҫ�������ַ���ָ��
	* @param CommaNumber:��Ҫ�����ڼ������ŵ�ƫ����
  * @retval 0~0xfe,���������ŵĵ�ַ��buf�׵�ַ��ƫ����;0xff��������Ҫ�����Ķ���
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
  * @brief ���������������֮����ַ�����ת��������
	* @param *headAddr:�ַ����׵�ַ
  * @retval �����ż����������
	*/
int GetIntBetweenTwoCommas(u8 *headAddr){
	u8 Integer[10]="";																//����������ַ���
	u8 i=0;
	while(*headAddr != ',' && *headAddr != '*'){	//û�е���һ��','��'*'
		Integer[i++] = *headAddr;										//��ȡ�ַ��γ��ַ���
		headAddr++;																	//��ַƫ��
	}
	return atoi((const char *)Integer);						//�ַ���ת����
}

/**
  * @brief ���������������֮����ַ�����ת���ɸ�����
	* @param *headAddr:�ַ����׵�ַ
  * @retval �����ż�ĸ���������
	*/
float GetFloatBetweenTwoCommas(u8 *headAddr){
	u8 Decimal[15]="";																//����������ַ���
	u8 i=0;
	while(*headAddr != ',' && *headAddr != '*'){	//û�е���һ��','��'*'
		Decimal[i++] = *headAddr;										//��ȡ�ַ��γ��ַ���
		headAddr++;																	//��ַƫ��
	}
	return atof((const char *)Decimal);						//�ַ���ת������
}

/**
  * @brief ���������������֮����ַ�����ת����˫���ȸ�����
	* @param *headAddr:�ַ����׵�ַ
  * @retval �����ż��˫���ȸ���������
	*/
double GetDoubleBetweenTwoCommas(u8 *headAddr){
	u8 Decimal[15]="";																//����������ַ���
	u8 i=0;
	while(*headAddr != ',' && *headAddr != '*'){	//û�е���һ��','��'*'
		Decimal[i++] = *headAddr;										//��ȡ�ַ��γ��ַ���
		headAddr++;																	//��ַƫ��
	}
	return atof((const char *)Decimal);						//�ַ���ת������
}

/**
  * @brief ����NMEA0183��ʽ��UTC�ַ���
	* @param mode:ģʽѡ��,1=����Сʱ,2=���ط���,3=��������
	* @param *headAddr:�ַ����׵�ַ
  * @retval ����mode������Ӧ������
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
  * @brief ����NMEA0183��ʽ��UTC�ַ���
	* @param mode:ģʽѡ��,1=�������,2=�����·�,3=��������
	* @param *headAddr:�ַ����׵�ַ
  * @retval ����mode������Ӧ������
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
  * @brief ����BDGSV����
	* @param *BDGSV_Info:�洢BDGSV���ݵĽṹ��ָ��
	* @param *buf:���NMEA0183���ĵĻ�����ָ��
  * @retval None
	*/
void NMEA0183_BDGSV_Analysis(BDGSV *BDGSV_Info,char *buf){
	u8 *p;
	u8 offset=0;
	u8 i = 0,j = 0;
	p = (u8 *)strstr((const char *)buf,"$BDGSV");	//��NMEA0183������Ѱ��"$BDGSV"���������ַ
	BDGSV_Info->BDGSV_Number = p[7] - '0';				//����ܵ�BDGSV������
	offset = NMEA0183_CommaAddrOffset(p,3);
	if(offset != 0xff) BDGSV_Info->BD_SAT_Number = GetIntBetweenTwoCommas(p+offset);
	for(i=0;i<BDGSV_Info->BDGSV_Number;i++){
		for(j=0;j<4;j++){		//ÿ��BDGSV�����������Ŀ����ǵ�����(���Ǳ�š����ǡ���λ�ǡ������)
			offset = NMEA0183_CommaAddrOffset(p,4+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_ID = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,5+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_Elevation = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,6+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_Azimuth = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,7+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_CNo = GetIntBetweenTwoCommas(p+offset);
		}
		p = (u8 *)strstr((const char *)p,"\n")+1;		//��pָ����һ��"$BDGSV"���׵�ַ
	}
}

/**
  * @brief ����GPGSV����
	* @param *GPGSV_Info:�洢GPGSV���ݵĽṹ��ָ��
	* @param *buf:���NMEA0183���ĵĻ�����ָ��
  * @retval None
	*/
void NMEA0183_GPGSV_Analysis(GPGSV *GPGSV_Info,char *buf){
	u8 *p;
	u8 offset=0;
	u8 i = 0,j = 0;
	p = (u8 *)strstr((const char *)buf,"$GPGSV");	//��NMEA0183������Ѱ��"$GPGSV"���������ַ
	GPGSV_Info->GPGSV_Number = p[7] - '0';				//����ܵ�GPGSV������
	offset = NMEA0183_CommaAddrOffset(p,3);
	if(offset != 0xff) GPGSV_Info->GPS_SAT_Number = GetIntBetweenTwoCommas(p+offset);
	for(i=0;i<GPGSV_Info->GPGSV_Number;i++){
		for(j=0;j<4;j++){		//ÿ��GPGSV�����������Ŀ����ǵ�����(���Ǳ�š����ǡ���λ�ǡ������)
			offset = NMEA0183_CommaAddrOffset(p,4+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_ID = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,5+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_Elevation = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,6+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_Azimuth = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,7+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_CNo = GetIntBetweenTwoCommas(p+offset);
		}
		p = (u8 *)strstr((const char *)p,"\n")+1;		//��pָ����һ��"$GPGSV"���׵�ַ
	}
}

/**
  * @brief ����GNGGA����
	* @param *GNGGA_Info:�洢GNGGA���ݵĽṹ��ָ��
	* @param *buf:���NMEA0183���ĵĻ�����ָ��
  * @retval None
	*/
void NMEA0183_GNGGA_Analusis(GNGGA *GNGGA_Info,char *buf){
	u8 *p;
	u8 offset=0;
	p = (u8 *)strstr((const char *)buf,"$GNGGA");	//��NMEA0183������Ѱ��"$GNGGA"���������ַ
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
  * @brief ����BDGSA����
	* @param *BDGSA_Info:�洢BDGSA���ݵĽṹ��ָ��
	* @param *buf:���NMEA0183���ĵĻ�����ָ��
  * @retval None
	*/
void NMEA0183_BDGSA_Analusis(BDGSA *BDGSA_Info,char *buf){
	u8 *p;
	u8 offset=0;
	u8 i;
	p = (u8 *)strstr((const char *)buf,"$BDGSA");	//��NMEA0183������Ѱ��"$BDGSA"���������ַ
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
  * @brief ����GPGSA����
	* @param *GPGSA_Info:�洢GPGSA���ݵĽṹ��ָ��
	* @param *buf:���NMEA0183���ĵĻ�����ָ��
  * @retval None
	*/
void NMEA0183_GPGSA_Analusis(GPGSA *GPGSA_Info,char *buf){
	u8 *p;
	u8 offset=0;
	u8 i;
	p = (u8 *)strstr((const char *)buf,"$GPGSA");	//��NMEA0183������Ѱ��"$BDGSA"���������ַ
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
  * @brief ����GNRMC����
	* @param *GNRMC_Info:�洢GNRMC���ݵĽṹ��ָ��
	* @param *buf:���NMEA0183���ĵĻ�����ָ��
  * @retval None
	*/
void NMEA0183_GNRMC_Analusis(GNRMC *GNRMC_Info,char *buf){
	u8 *p;
	u8 offset=0;
	p = (u8 *)strstr((const char *)buf,"$GNRMC");	//��NMEA0183������Ѱ��"$GNRMC"���������ַ
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
  * @brief ����GNVTG����
	* @param *GNVTG_Info:�洢GNVTG���ݵĽṹ��ָ��
	* @param *buf:���NMEA0183���ĵĻ�����ָ��
  * @retval None
	*/
void NMEA0183_GNVTG_Analusis(GNVTG *GNVTG_Info,char *buf){
	u8 *p;
	u8 offset=0;
	p = (u8 *)strstr((const char *)buf,"$GNVTG");	//��NMEA0183������Ѱ��"$GNRMC"���������ַ
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

