#ifndef IMPARAMS_H_
#define IMPARAMS_H_

class IMParams
{
public:
	//60 bits in total

	//IM - 18 bits
	int UnitValue;      //5 bits
	int UnitRadius;     //4 bits
	//int TerrainValue;   //5 bits
	//int TerrainRadius;  //4 bits
	//PF - 20 bits
	int A;              //6 bits
	int B;              //6 bits
	int m;              //4 bits
	int n;              //4 bits
	//Micro - 22 bits
	int m_freeze;       //4 bits 0-15 freeze time after firing
	int m_kitingRange;  //5 bits 0-31 cell Start to kiting in this range
	int m_targetNearby; //4 bits 0-15
	int m_kitingDist;   //3 bits 0-8
	int m_hpfocus;      //3 bits 0-1 split into 8
	int m_hpkiting;     //3 bits 0-1 split into 8
};

#endif /* IMPARAMS_H_ */
