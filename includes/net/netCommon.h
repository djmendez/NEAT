/*
 * netCommon.h
 *
 *  Created on: Sep 22, 2013
 *      Author: simingl
 */

#ifndef NETCOMMON_H_
#define NETCOMMON_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <messages.h>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace FastEcslent {

class NetCommon {

public:
	std::pair<boost::asio::ip::udp::endpoint, std::string> getMyIP();
	long getCurrentTimeLong();
	NetCommon();
	virtual ~NetCommon();

private:
	boost::posix_time::ptime  startTime;

};

} /* namespace FastEcslent */
#endif /* NETCOMMON_H_ */
