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
	cv::KeyPoint root;
	cv::KeyPoint magnitude;
	float delta_t;

	FlowPoint(cv::KeyPoint root, cv::KeyPoint magnitude, float delta_t);
};

class FlowField {

private:
	float timeDelta;
	vector<FlowPoint> flowField;

public:
	FlowField(float delta_ms);
	FlowField(float delta_ms, int size);
	virtual ~FlowField();

	void addFlowPoint(const FlowPoint *flow_point);
	const FlowPoint* getFlowPoint(int index);

	int getLength(void);
	float getTimeDelta(void);

};

} /* namespace COLA */

#endif /* FLOWFIELD_H_ */
