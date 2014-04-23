/*
 * FrameDescriptor.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#ifndef FRAMEDESCRIPTOR_H_
#define FRAMEDESCRIPTOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace COLA {

class FrameDescriptor {
private:
	int numberOfFeatures;
	cv::Rect roi_rect;

public:
	std::vector<cv::KeyPoint> featurePoints;
	cv::Mat descriptors;
	struct timespec timestamp;
	cv::Mat refFrame; //The original image this descriptor set belongs too.

	FrameDescriptor(int numberOfFeatures=1000, cv::Rect roi_rect=cv::Rect());
	virtual ~FrameDescriptor();

	int MaxFeatures(void);
	void clear(void); //Clear (but do not de-allocate) where nesssary.
};

} /* namespace COLA */

#endif /* FRAMEDESCRIPTOR_H_ */
