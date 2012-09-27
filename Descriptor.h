/*
 * Descriptor.h
 *
 *  Created on: Sep 13, 2012
 *      Author: rghunter
 */

#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;


class Descriptor {
public:
	Descriptor();
	virtual ~Descriptor();
	Mat *toMat();
	int size();


private:
	void updateMat();
	void updateVector();

	bool isChanged;
	vector<Mat> descriptor_vec;
	Mat descriptor_mat;

};

#endif /* DESCRIPTOR_H_ */
