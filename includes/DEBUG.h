/* 
 * File:   DEBUG.h
 * Author: chris
 *
 * Created on September 6, 2013, 11:21 AM
 */

#ifndef DEBUG_H
#define	DEBUG_H

#ifdef DEBUG_OUTPUT
#include <iostream>
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif


#endif	/* DEBUG_H */

