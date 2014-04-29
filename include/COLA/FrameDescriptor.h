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
/**
 * \brief	An object to contain all the relevant feature information for a frame.
 *
 * The FrameDescriptor class is a data structure that represents everything we know about a given frame. It contains
 * the ROI information, source frame, Key Points (where interesting features are located) and the descriptors for those
 * features. It also contains timestamp information useful for determining the elapsed time between frames. This is
 * represented by both timestamp (for real-time implementations) and frame number (for offline video processing).
 * See COLA::Time for more information on how time is tracked by the program. The ROI rectangle is used to create an ROI
 * image from the original frame. This way, we can track the normalization of keypoints with respect to the original image.
 */
class FrameDescriptor {
private:
	int numberOfFeatures;
	bool has_been_normalized;

public:
	cv::Rect roi_rect;
	bool roi_set;
	cv::Point2f roi_offset;

	std::vector<cv::KeyPoint> featurePoints;
	cv::Mat descriptors;

	int frame_number;
	struct timespec timestamp;

	cv::Mat refFrame; //The original image this descriptor set belongs too.
	cv::Mat *process_frame; //this is the frame we run the algorithim on.
	/**
	 * @brief	Constructor
	 *
	 * We initialize a frame descriptor with number of feature and an optional ROI rectangle.
	 *
	 * @param numberOfFeatures int the maximum number of features to track
	 * @param roi_rect cv::Rect the ROI rectangle we will use to generate the sub-image for processing.
	 */
	FrameDescriptor(int numberOfFeatures=1000, cv::Rect roi_rect=cv::Rect());
	/**
	 * @brief	Transforms the KeyPoint vector to real-space
	 *
	 * This function allows us to transform the keypoints from ROI space to the full image space. This
	 * function will only carry out the transform once to prevent accidentally breaking the keypoint vector.
	 */
	void normalizeKeypoints(void);
	/**
	 * @brief	prepare for a new reference frame without deallocating memory
	 *
	 * This resets the state of the frame descriptor by clearing the keypoint vector
	 */
	void reset(void); //Clear (but do not de-allocate) where nesssary.
};

} /* namespace COLA */

#endif /* FRAMEDESCRIPTOR_H_ */
