#include "nmea0183_parse.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

unsigned char NMEA0183_CommaAddrOffset(unsigned char *buf,unsigned char CommaNumber){	 		    
	unsigned char *p = buf;
	while(CommaNumber){		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0Xff;
		if(*buf==',')CommaNumber--;
		buf++;
	}
	return buf-p;
}

int GetIntBetweenTwoCommas(unsigned char *headAddr){
	unsigned char Integer[10]="";																//����������ַ���
	unsigned char i=0;
	while(*headAddr != ',' && *headAddr != '*'){
		Integer[i++] = *headAddr;
		headAddr++;
	}
	return atoi((const char *)Integer);
}

float GetFloatBetweenTwoCommas(unsigned char *headAddr){
	unsigned char Decimal[15]="";																//����������ַ���
	unsigned char i=0;
	while(*headAddr != ',' && *headAddr != '*'){
		Decimal[i++] = *headAddr;
		headAddr++;
	}
	return atof((const char *)Decimal);
}

double GetDoubleBetweenTwoCommas(unsigned char *headAddr){
	unsigned char Decimal[15]="";																//����������ַ���
	unsigned char i=0;
	while(*headAddr != ',' && *headAddr != '*'){
		Decimal[i++] = *headAddr;
		headAddr++;
	}
	return atof((const char *)Decimal);
}

unsigned char GetUTCTime(unsigned char mode,unsigned char *headAddr){
	unsigned char UTCHourStr[2]="";
	unsigned char UTCMinStr[2]="";
	unsigned char UTCSecStr[2]="";
	unsigned char i=0,j=0,k=0;
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
		case 1:return (unsigned char)atoi((const char *)UTCHourStr);
		case 2:return (unsigned char)atoi((const char *)UTCMinStr);
		case 3:return (unsigned char)atoi((const char *)UTCSecStr);
		default:return 0;
	}
}

unsigned char GetUTCDate(unsigned char mode,unsigned char *headAddr){
	unsigned char UTCYearStr[2]="";
	unsigned char UTCMouthStr[2]="";
	unsigned char UTCDayStr[2]="";
	unsigned char i=0,j=0,k=0;
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
		case 1:return (unsigned char)atoi((const char *)UTCYearStr);
		case 2:return (unsigned char)atoi((const char *)UTCMouthStr);
		case 3:return (unsigned char)atoi((const char *)UTCDayStr);
		default:return 0;
	}
}

void NMEA0183_BDGSV_Analysis(BDGSV *BDGSV_Info,char *buf){
	unsigned char *p;
	unsigned char offset=0;
	unsigned char i = 0,j = 0;
	p = (unsigned char *)strstr((const char *)buf,"$BDGSV");
	BDGSV_Info->BDGSV_Number = p[7] - '0';
	offset = NMEA0183_CommaAddrOffset(p,3);
	if(offset != 0xff) BDGSV_Info->BD_SAT_Number = GetIntBetweenTwoCommas(p+offset);
	for(i=0;i<BDGSV_Info->BDGSV_Number;i++){
		for(j=0;j<4;j++){
			offset = NMEA0183_CommaAddrOffset(p,4+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_ID = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,5+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_Elevation = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,6+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_Azimuth = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,7+j*4);
			if(offset != 0xff) BDGSV_Info->BD_SAT_Status[j+i*4].SAT_CNo = GetIntBetweenTwoCommas(p+offset);
		}
		p = (unsigned char *)strstr((const char *)p,"\n")+1;
	}
}

void NMEA0183_GPGSV_Analysis(GPGSV *GPGSV_Info,char *buf){
	unsigned char *p;
	unsigned char offset=0;
	unsigned char i = 0,j = 0;
	p = (unsigned char *)strstr((const char *)buf,"$GPGSV");
	GPGSV_Info->GPGSV_Number = p[7] - '0';
	offset = NMEA0183_CommaAddrOffset(p,3);
	if(offset != 0xff) GPGSV_Info->GPS_SAT_Number = GetIntBetweenTwoCommas(p+offset);
	for(i=0;i<GPGSV_Info->GPGSV_Number;i++){
		for(j=0;j<4;j++){
			offset = NMEA0183_CommaAddrOffset(p,4+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_ID = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,5+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_Elevation = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,6+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_Azimuth = GetIntBetweenTwoCommas(p+offset);
			offset = NMEA0183_CommaAddrOffset(p,7+j*4);
			if(offset != 0xff) GPGSV_Info->GPS_SAT_Status[j+i*4].SAT_CNo = GetIntBetweenTwoCommas(p+offset);
		}
		p = (unsigned char *)strstr((const char *)p,"\n")+1;
	}
}

void NMEA0183_GNGGA_Analusis(GNGGA *GNGGA_Info,char *buf){
	unsigned char *p;
	unsigned char offset=0;
	p = (unsigned char *)strstr((const char *)buf,"$GNGGA");
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
}

void NMEA0183_BDGSA_Analusis(BDGSA *BDGSA_Info,char *buf){
	unsigned char *p;
	unsigned char offset=0;
	unsigned char i;
	p = (unsigned char *)strstr((const char *)buf,"$BDGSA");
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

void NMEA0183_GPGSA_Analusis(GPGSA *GPGSA_Info,char *buf){
	unsigned char *p;
	unsigned char offset=0;
	unsigned char i;
	p = (unsigned char *)strstr((const char *)buf,"$GPGSA");
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

void NMEA0183_GNRMC_Analusis(GNRMC *GNRMC_Info,char *buf){
	unsigned char *p;
	unsigned char offset=0;
	p = (unsigned char *)strstr((const char *)buf,"$GNRMC");
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

void NMEA0183_GNVTG_Analusis(GNVTG *GNVTG_Info,char *buf){
	unsigned char *p;
	unsigned char offset=0;
	p = (unsigned char *)strstr((const char *)buf,"$GNVTG");
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

