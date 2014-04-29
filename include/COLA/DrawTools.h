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
	 * @param output a cv::Mat containing a reference to the output frame we will write to.
	 * @param input a cv::Mat reference to the output buffer
	 * @param flow_vector a COLA::FlowPoint reference that will be plotted on the output buffer.
	 */
	void DrawFlowPoint(cv::Mat &output, cv::Mat &input_frame, FlowPoint &flow_vector);
	/**
	 * \brief	DrawMatches is used to plot matches from a COLA::FlowField onto an output buffer
	 *
	 * \param output a cv::Mat containing a reference to the output frame we will write to.
	 * \param train a COLA::FrameDescriptor used source the train portion of the match set from
	 * \param query a COLA::FrameDescriptor used source the query portion of the match set from
	 * \param field a COLA::FlowField reference that will be used to source the matches for the given frame set.
	 */
	void DrawMatches(cv::Mat &output, FrameDescriptor &train, FrameDescriptor &query, FlowField &field);
	/**
	 * \brief	DrawFlowPoint is used to plot a Tau point on an image
	 *
	 * \param output a cv::Mat containing a reference to the output frame we will write to.
	 * \param input a cv::Mat reference to the output buffer
	 * \param flow_vector a COLA::FlowPoint reference that will be plotted on the output buffer.
	 */
	void DrawTau(cv::Mat &output, COLA::TauMat &tau_field);
};

} /* namespace COLA */

#endif /* DRAWTOOLS_H_ */
