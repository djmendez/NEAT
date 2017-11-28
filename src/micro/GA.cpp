#include "micro/GA.h"

GA * GA::instance = 0;  

GA* GA::getInstance()  
{  
    if (instance == 0) {  
        instance = new GA();  
    }  
 
    return instance;  
} 

GA::GA()
{
	chromoLen = 61;
	signalLen = 4;
	positiveFitnessFactor = 1000.0;
	this->initialized = false;

}

void GA::setScore(int score)
{
	this->score = score; 
}

IMParams GA::getRedParams()
{
	return this->redParams;
}

IMParams GA::getBlueParams()
{
	//return this->blueParams;
	return this->getBaselineParams();
}

IMParams GA::getBaselineParams()
{
	//12,16,20,20,2,2,0,13,12,13,3,2
	//old: 26, 16, 46, 4, 5, 2, 0, 8, 3, 30, 0, 4
	//9260: 14,10,60,13,10,3,2,22,14,4,7,1


	IMParams param;
	param.UnitValue      = 26;
	param.UnitRadius     = 16;
	//param.TerrainValue   = 3;
	//param.TerrainRadius  = 10;

	param.A = 46;
	param.B = 4;

	param.m = 5;
	param.n = 2;

	param.m_freeze      = 0;
	param.m_kitingRange = 8;
	param.m_targetNearby= 3;
	param.m_kitingDist  = 30;
	param.m_hpfocus     = 0;
	param.m_hpkiting    = 4;

/*
	IMParams param;
	param.UnitValue      = 14;
	param.UnitRadius     = 10;
	//param.TerrainValue   = 3;
	//param.TerrainRadius  = 10;

	param.A = 60;
	param.B = 13;

	param.m = 10;
	param.n = 3;

	param.m_freeze      = 2;
	param.m_kitingRange = 22;
	param.m_targetNearby= 14;
	param.m_kitingDist  = 4;
	param.m_hpfocus     = 7;
	param.m_hpkiting    = 1;
*/

	return param;
}

//30,15,3,11,57,48,1,14,0,30,6,1,7,1
IMParams GA::getBaselineParams2()
{
	IMParams param;
	param.UnitValue      = 30;
	param.UnitRadius     = 15;
	//param.TerrainValue   = 3;
	//param.TerrainRadius  = 11;

	param.A = 57;
	param.B = 48;

	param.m = 1;
	param.n = 14;

	param.m_freeze      = 0;
	param.m_kitingRange = 30;
	param.m_targetNearby= 6;
	param.m_kitingDist  = 1;
	param.m_hpfocus     = 7;
	param.m_hpkiting    = 1;

	return param;
}


void GA::parseParameterInput(int buffer[], int length)
{
//	int param1[]  = {0,0,0,0,0};
//	int param2[]  = {0,0,0,0};
//	int param3[]  = {0,0,0,0,0};
//	int param4[]  = {0,0,0,0};
//
//	int param5[]  = {0,0,0,0,0,0};
//	int param6[]  = {0,0,0,0,0,0};
//	int param7[]  = {0,0,0,0};
//	int param8[]  = {0,0,0,0};
//
//	int param9[]  = {0,0,0,0};
//	int param10[] = {0,0,0,0,0};
//	int param11[] = {0,0,0,0};
//	int param12[] = {0,0,0};
//	int param13[] = {0,0,0};
//	int param14[] = {0,0,0};
//
//	int offset = 4;
//	for(int i=offset;i<offset+5; i++) {	param1[i-offset] = buffer[i]; }
//	offset +=5;
//	for(int i=offset;i<offset+4; i++) {	param2[i-offset] = buffer[i]; }
//	offset +=4;
//	for(int i=offset;i<offset+5; i++) {	param3[i-offset] = buffer[i]; }
//	offset +=5;
//	for(int i=offset;i<offset+4; i++){  param4[i-offset] = buffer[i]; }
//	offset +=4;
//
//	for(int i=offset;i<offset+6; i++){	param5[i-offset] = buffer[i]; }
//	offset +=6;
//	for(int i=offset;i<offset+6; i++){	param6[i-offset] = buffer[i]; }
//	offset +=6;
//	for(int i=offset;i<offset+4; i++){	param7[i-offset] = buffer[i]; }
//	offset +=4;
//	for(int i=offset;i<offset+4; i++){	param8[i-offset] = buffer[i]; }
//	offset +=4;
//
//	for(int i=offset;i<offset+4; i++){	param9[i-offset] = buffer[i]; }
//	offset +=4;
//	for(int i=offset;i<offset+5; i++){	param10[i-offset] = buffer[i]; }
//	offset +=5;
//	for(int i=offset;i<offset+4; i++){	param11[i-offset] = buffer[i]; }
//	offset +=4;
//	for(int i=offset;i<offset+3; i++){	param12[i-offset] = buffer[i]; }
//	offset +=3;
//	for(int i=offset;i<offset+3; i++){	param13[i-offset] = buffer[i]; }
//	offset +=3;
//	for(int i=offset;i<offset+3; i++){	param14[i-offset] = buffer[i]; }
//
//	currentParams.UnitValue     = decode(param1, 5)+1;
//	currentParams.UnitRadius    = decode(param2, 4)+1;
//	currentParams.TerrainValue  = decode(param3, 5)+1;
//	currentParams.TerrainRadius = decode(param4, 4)+1;
//
//	currentParams.A = decode(param5, 6)+1;
//	currentParams.B = decode(param6, 6)+1;
//
//	currentParams.m = decode(param7, 4)+1;
//	currentParams.n = decode(param8, 4)+1;
//
//	currentParams.m_freeze      = decode(param9,  4);
//	currentParams.m_kitingRange = decode(param10, 5);
//	currentParams.m_targetNearby= decode(param11, 4);
//	currentParams.m_kitingDist  = decode(param12, 3);
//	currentParams.m_hpfocus     = decode(param13, 3);
//	currentParams.m_hpkiting    = decode(param14, 3);
}

int GA::decode(int param[], int length)
{
	int result = 0;
	for(int i=0;i<length;i++)
	{
		result += (int)(param[i])*pow((float)2, length-i-1);
	}
	return result;
}
