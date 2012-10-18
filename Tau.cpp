/*
 * Tau.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: rghunter
 */

#include "Tau.h"

namespace OpticalQuad {

Tau::Tau() : tau(0), location(0,0,0)
{

}
Tau::Tau(float tau, cv::KeyPoint location) : tau(tau), location(location)
{
	// TODO Auto-generated constructor stub

}

Tau::~Tau() {
	// TODO Auto-generated destructor stub
}

} /* namespace OpticalQuad */
