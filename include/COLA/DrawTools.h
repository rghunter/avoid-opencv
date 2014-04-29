/*
 * DrawTools.h
 *
 *  Created on: Apr 23, 2014
 *      Author: rhunter
 */

#ifndef DRAWTOOLS_H_
#define DRAWTOOLS_H_

#include <opencv2/opencv.hpp>
#include <COLA/FlowField.h>
#include <COLA/FrameDescriptor.h>
#include <COLA/Tau.h>

namespace COLA {

/**
 * \brief	DrawTools is a class containing useful utilities for drawing outputs from COLA
 *
 * The Drawtools class provides functions for drawing matches, flow points, and tau fields
 */
class DrawTools {
public:

	/**
	 * @brief	DrawFlowPoint is used to plot a plot point on an image
	 *
	 * @param &output Contains a reference to the output frame we will write to.
	 * @param &input_frame Contains a reference to the output buffer
	 * @param &flow_vector Contains a flow vector reference that will be plotted on the output buffer.
	 */
	void DrawFlowPoint(cv::Mat &output, cv::Mat &input_frame, FlowPoint &flow_vector);

	/**
	 * \brief	DrawMatches is used to plot matches from a COLA::FlowField onto an output buffer
	 *
	 * \param output contains a reference to the output frame we will write to.
	 * \param train Source of the train portion of the match set from
	 * \param query Source of the query portion of the match set from
	 * \param field The field that will be used to source the matches for the given frame set.
	 */
	void DrawMatches(cv::Mat &output, FrameDescriptor &train, FrameDescriptor &query, FlowField &field);

	/**
	 * \brief	DrawTau is used to plot a Tau point on an image
	 *
	 * \param &output Contains a reference to the output frame we will write to.
	 * \param &tau_field A reference to the tau field to be plotted
	 */
	void DrawTau(cv::Mat &output, COLA::TauMat &tau_field);
};

} /* namespace COLA */

#endif /* DRAWTOOLS_H_ */
