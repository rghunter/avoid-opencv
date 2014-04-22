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
	cv::Point2f root;
	cv::Point2f magnitude;

	FlowPoint(cv::Point2f root, cv::Point2f magnitude);
};

class FlowField {

private:
	vector<FlowPoint> flowField;

public:
	vector<cv::DMatch> matches;
	float timeDelta_sec;
	FlowField(int size);
	virtual ~FlowField();

	void addFlowPoint(FlowPoint &flow_point);
	void addFlowPoint(const FlowPoint flow_point);
	const FlowPoint* getFlowPoint(int index);

	void clear(void);
	int getSize(void);

};

} /* namespace COLA */

#endif /* FLOWFIELD_H_ */
