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
	bool has_been_normalized;

public:
	int frame_number;
	cv::Rect roi_rect;
	bool roi_set;
	cv::Point2f roi_offset;
	std::vector<cv::KeyPoint> featurePoints;
	cv::Mat descriptors;
	struct timespec timestamp;
	cv::Mat refFrame; //The original image this descriptor set belongs too.
	cv::Mat *process_frame; //this is the frame we run the algorithim on.

	FrameDescriptor(int numberOfFeatures=1000, cv::Rect roi_rect=cv::Rect());

	int MaxFeatures(void);
	void normalizeKeypoints(void);
	void clear(void); //Clear (but do not de-allocate) where nesssary.
};

} /* namespace COLA */

#endif /* FRAMEDESCRIPTOR_H_ */
