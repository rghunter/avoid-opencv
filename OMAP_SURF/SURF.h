/*
 * SURF.h
 *
 *  Created on: Oct 7, 2012
 *      Author: rghunter
 */

#ifndef SURF_H_
#define SURF_H_

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <stdlib.h>
#include <fstream>
#include <sys/time.h>
#include <unistd.h>

#include "responselayer.h"

using namespace std;

namespace OpticalQuad {

static const int OCTAVES = 5;
static const int INTERVALS = 4;

static const int filter_map [OCTAVES][INTERVALS] = {{0,1,2,3}, {1,3,4,5}, {3,5,6,7}, {5,7,8,9}, {7,9,10,11}};


class SURF {
public:
	SURF(const int octaves, const int intervals, const int init_sample,
            const float thresh);
	virtual ~SURF();
	bool ExtractPoints(cv::Mat &input, vector<cv::KeyPoint> &keypoints_train);

private:

	cv::Mat integral_image;
	vector<ResponseLayer *> responseMap;
	const int octaves, init_sample, intervals;
	const float thresh;

	ofstream data_log3;


	bool IntegralImage(cv::Mat &input_image, cv::Mat &integral_image_in);
	float BoxIntegral(cv::Mat &img, int row, int col, int rows, int cols);
	void BuildResponseMap(cv::Mat &int_image);
	void BuildResponseLayer(ResponseLayer *rl, cv::Mat &int_image);
	void FastHessian();
	int isExtremum(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b);
	void interpolateExtremum(int r, int c, ResponseLayer *t, ResponseLayer *m,
			ResponseLayer *b, vector<cv::KeyPoint> &keypoints_train);
	void interpolateStep(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b,
	                                  double* xi, double* xr, double* xc );
	cv::Mat* hessian3D(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b);
	cv::Mat* deriv3D(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b);

	float time_elapsed(timespec start, timespec end);


};

} /* namespace OpticalQuad */
#endif /* SURF_H_ */
