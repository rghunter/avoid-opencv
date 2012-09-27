/*
 * Descriptor.cpp
 *
 *  Created on: Sep 13, 2012
 *      Author: rghunter
 */

#include "Descriptor.h"

Descriptor::Descriptor() {
	// TODO Auto-generated constructor stub

}

Descriptor::~Descriptor() {
	// TODO Auto-generated destructor stub
}

Mat * Descriptor::toMat(){
	if(isChanged)
		this->updateMat();
	else
		isChanged = true;
	return &descriptor_mat;
}
int Descriptor::size(){
	return descriptor_vec.size();
}

void Descriptor::updateMat()
{
	descriptor_mat.release();
	for(int i=0; i<descriptor_vec.size(); i++)
		descriptor_vec[i].copyTo(descriptor_mat.row(i));
	isChanged = false;
}
void Descriptor::updateVector()
{
	descriptor_mat

}

