/*
 * FlowField.h
 *
 *  Created on: Apr 21, 2014
 *      Author: rhunter
 */

#ifndef FLOWFIELD_H_
#define FLOWFIELD_H_

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;

namespace COLA {

/**
 * \brief FlowPoint is an object representation of an optical flow vector
 *
 * A FlowPoint object represent the location and magnitude of an optical flow point.
 */
class FlowPoint {

public:
	cv::Point2f location; /**< The location of the flow point (currently) */

	cv::Vec2f magnitude; /**< The magnitude of the flow */

	/**
	 * @brief	FlowPoint Constructor (Pre-Calculated flow vector)
	 *
	 * This constructor is used if we have a flow point we already calculated the magnitude for.
	 *
	 * @param root cv::Point2f the location the flow vector represent the flow for. This is the current location of the feature.
	 * @param magnitude cv::Vec2f floating point representation of the x and y portions of the optical flow vector magnitude
	 */
	FlowPoint(cv::Point2f root, cv::Vec2f magnitude);

	/**
	 * @brief	FlowPoint Constructor (Point Delta form)
	 *
	 * @param start cv::Point2f the initial feature location
	 * @param end cv::Point2f the current (final) location of the feature
	 * @param time_delta float the elapsed time (in seconds) between start and end
	 */
	FlowPoint(cv::Point2f start, cv::Point2f end, float time_delta);

	/**
	 * @brief	FlowPoint equivalence operator
	 * This operator allows us to compare COLA::FlowPoints
	 * @param &other COLA::FlowPoint a reference to the FlowPoint we wish to compare this too.
	 */
	bool operator== (const FlowPoint &other) const;

};

/**
 * \brief	A FlowField represents a collection of FlowPoints from the same frame set
 *
 * This is a vector grouping of FlowPoints for a given frame set. This is also used to carry the match vector used by the matcher for
 * determining feature matches from the descriptors
 */
class FlowField : public vector<FlowPoint> {

private:

public:
	vector<vector<cv::DMatch> > matches;  /**< the vector of matches (for use by the matching algorithim) */

	/**
	 * @brief	Constructor
	 *
	 * @param size int the expected size of the flow field (used to pre-allocate the vector)
	 */
	FlowField(int size);

	/**
	 * @brief	Reset the FlowField
	 * This allows us to reset the flow field without releasing the memory allocated to the vector. This is useful when we have an
	 * upper bound on the number of possible flow points and do not want to have to reallocate memory in between matchings.
	 */
	void reset(void);

};

} /* namespace COLA */

#endif /* FLOWFIELD_H_ */
