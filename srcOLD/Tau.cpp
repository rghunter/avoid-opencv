/*
 * Tau.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: rghunter
 */

#include "Tau.h"

namespace COLA {

Tau::Tau() : tau(0), location(0,0,0)
{

}
Tau::Tau(float tau, cv::KeyPoint location) : tau(tau), location(location)
{

}

Tau::~Tau() {
}

} /* namespace COLA */
