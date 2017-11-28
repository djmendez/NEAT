/*
 * timer.h
 *
 *  Created on: Jan 24, 2012
 *      Author: sushil
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;
#include <utils.h>

namespace FastEcslent {


	class MilliSecondTimer {
	private:

		ptime start;
		time_duration diff;
		float         interval;
		ptime current;

	public:
		MilliSecondTimer(float inter) {
			start = getCurrentTime();
			interval = inter;
		}

		bool hasFired(){
			current = getCurrentTime();
			diff = current - start;
			if (diff.total_milliseconds() > interval) {
				start = current;
				return true;
			}
			return false;
		}

	};



}


#endif /* TIMER_H_ */
