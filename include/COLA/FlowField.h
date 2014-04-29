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

class FlowPoint {

public:
	cv::Point2f location;
	cv::Vec2f magnitude;

	FlowPoint(cv::Point2f root, cv::Vec2f magnitude);
	FlowPoint(cv::Point2f start, cv::Point2f end, float time_delta);
	bool operator== (const FlowPoint &other) const;

};

class FlowField : public vector<FlowPoint> {

private:

public:
	vector<vector<cv::DMatch> > matches;
	FlowField(int size);
	void reset(void);

};

} /* namespace COLA */

#endif /* FLOWFIELD_H_ */
