#ifndef MICROPARAMS_H_
#define MICROPARAMS_H_

namespace FastEcslent{

	class MicroParam
	{
	public:
		float freeze;        //freeze time after firing
		int kitingRange;   //Start to kiting in this range
		int targetNearby;
		float kitingDist;
		float hpfocus;
		float hpkiting;
	};


}


#endif
