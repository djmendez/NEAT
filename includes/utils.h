/*
 * utils.h
 *
 *  Created on: Nov 21, 2011
 *      Author: sushil
 */

#ifndef FE_UTILS_H_
#define FE_UTILS_H_

#include <stdlib.h>
#include <OgreMatrix3.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

const float pi = 3.14159;
const float twopi = 2.0 * pi;
const double epsilon = 0.0001;

inline bool closeTo(double v1, double v2){
	return ( fabs(v1-v2) < epsilon);
}

inline float clamp(float value, float lower, float upper){
	if (value > upper)
		return upper;
	if(value < lower)
		return lower;
	return value;
}

inline float makeAnglePosNeg(float angle)
{
    //printf("makeAnglePosNeg:%f ", angle);
    while (angle > pi)
        angle -= twopi;
    while (angle < -pi)
        angle += twopi;
    //printf("->%f ", angle);
    return angle;
}

inline float differenceBetweenAngles(float angle1, float angle2){
    return makeAnglePosNeg(angle1 - angle2);
}

inline float feet(float x){
	return 0.3048 * x;
}
inline float meters(float x){
	return x;
}

inline float knots(float x) {
	return 0.514444444 * x;
}
inline float feetPerSecondSquared(float x) {
	return 0.3048 * x;
}

inline float degreesPerSecond(float x) {
	return 0.0174532925 * x;
}

inline float tons(float x){
	return x * 907.18474;
}
inline float pounds(float x){
	return x * 0.45359237;
}

inline int randInt(int low, int high) {
	return low + random() % (high - low);
}

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

inline ptime getCurrentTime(){
	return boost::posix_time::microsec_clock::local_time();
}

inline Ogre::Radian degrees(float d){
    return Ogre::Radian(d * pi / 180.0);
}

inline float toDegrees(float r){
    return r * 180/pi;
}

inline Ogre::Quaternion pitchYawRoll(float p, float y, float r){
    Ogre::Matrix3 m;
    m.FromEulerAnglesXYZ(degrees(p), degrees(y), degrees(r));
    Ogre::Quaternion q;
    q.FromRotationMatrix(m);
    return q;
}

inline std::string trim(std::string s){
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

inline std::string int_to_string(const int& port) {
    std::stringstream ss;
    ss << port;
    return ss.str();
}

inline std::string ip_int_to_string(const long& ipi){
	struct in_addr addr;
	addr.s_addr = ipi;
	char *ips = inet_ntoa(addr);
	return std::string(ips);
}

inline unsigned int ip_string_to_int(const std::string& ips){
	unsigned int ipi = inet_addr(ips.c_str());
	return ipi;
}
#endif /* FE_UTILS_H_ */
