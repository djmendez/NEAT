#pragma once

//#include <windows.h>
//
//#include <BWAPI.h>
#include <math.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include "IMParams.h"
//#include <tchar.h>

#define BUF_SIZE 256

class GA
{
public:
	static GA* getInstance();
	GA();
	~GA();

	double positiveFitnessFactor;
	int writeFitness2SharedMemory(int score);
	IMParams getRedParams();
	IMParams getBlueParams();
	IMParams getBaselineParams();
	IMParams getBaselineParams2();

	void setParams(IMParams params1, IMParams params2){
		this->redParams  = params1;
		this->blueParams = params2;
	};

	void setScore(int score);
	int getScore(){return this->score;}
	void log2file(int score);
protected:
	int score;
	IMParams redParams;
	IMParams blueParams;

	bool readfile();
	int evaluateCount;
private:
	int decode(int param[], int length);

	int initSharedMemory();
	void testInit(int buffer[]);
	void readParameterFromSharedMemory();

	void parseParameterInput(int buffer[], int length);

//	HANDLE hMapFile;
	int* pBuf;

	int chromoLen;
	int signalLen;
//    static const TCHAR szName[];
	static const char datafile[];
	static const char logfile[];
	
	bool initialized;

	static GA* instance;
};
