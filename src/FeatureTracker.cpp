/*
 * ObstacleDetector.cpp
 *
 *  Created on: Sep 12, 2012
 *      Author: rghunter
 */

#include "FeatureTracker.h"

#define REFRESH_AFTER			0
#define ACC_HAMMING				60

namespace COLA {

FeatureTracker::FeatureTracker(int height, int width)
{
	FAST_Detector = new cv::FastFeatureDetector(40,true); //60
	extractor = new cv::FREAK(false,true,70,4);

	frame_count = 0;

	roi_rec = new cv::Rect(0,height/3,width,height/3);

	nodal_pt = cv::KeyPoint (width/2.0f,height/2.0f,0);

	data_log2.open("./data/performance_data.csv");
	tau_log.open("./data/tau_value.csv");

}

FeatureTracker::~FeatureTracker() {
	cout << "Closing Obstacle Detector." << endl;
	data_log2.close();
	tau_log.close();
	delete(FAST_Detector);
	delete(extractor);
}

void FeatureTracker::matchFrame(cv::Mat inputFrame)
{
	timespec begin, end;

	memcpy(&frame_last,&frame_current,sizeof(timespec));

	clock_gettime(CLOCK_MONOTONIC,&frame_current);

	keypoint_query.clear();
	keypoint_query_roi.clear();
	matches.clear();

	cv::Mat greyFrame;
	cvtColor(inputFrame,greyFrame,CV_BGR2GRAY,1); //We dont care about the color, greyscale is fine.


	greyFrame.copyTo(undistorted_frame); //this is being used for drawing

	FAST_Detector->detect(greyFrame,keypoint_query);

	if(keypoint_query.size() == 0) //confirm we detected something.
		return;

	extractor->compute(greyFrame,keypoint_query,descriptor_query); //The freak extractor creates descriptors for the given keypoints

	if(keypoint_train.size() == 0 || frame_count > REFRESH_AFTER)
	{
		this->initialFrame(greyFrame,keypoint_query,descriptor_query);
		return;
	}

	frame_count++;

	matcher.match(descriptor_train,descriptor_query,matchesUnfiltered); //note: query and train are swapped on purpose.
																		//All query points should match to a training point but not
																		//nessesarily the other way around (the training point my no
																		//longer exist as it is off camera)

	for(unsigned int i=0;i<matchesUnfiltered.size();i++) //Now remove "Weak matches" (Matches with a hamming distance above the acceptable threshold)
		if(matchesUnfiltered[i].distance < ACC_HAMMING)
			matches.push_back(matchesUnfiltered[i]);

	tauVals.clear();

	for(unsigned int i=0;i<matches.size();i++)
	{
		 float origin_pt = euclid_distance(keypoint_train[matches[i].queryIdx].pt,nodal_pt.pt);
		 float current_pt = euclid_distance(keypoint_query[matches[i].trainIdx].pt,nodal_pt.pt);

		 float traverse_time=1.0;// time_elapsed(frame_last,frame_current);

		 float tauV = (current_pt - origin_pt) / traverse_time;

		 cv::KeyPoint curr_loc =  keypoint_query[matches[i].trainIdx];
		 COLA::Tau tau(tauV,curr_loc);
		 tauVals.push_back(tau);

		 tau_log << tauV << ", ";

	}

	tau_log << endl;
}

void FeatureTracker::initialFrame(cv::Mat input_image, vector<cv::KeyPoint> detected_keypoints, cv::Mat descriptors)
{
	frame_count = 0;
	input_image.copyTo(initial_frame);

	descriptors.copyTo(descriptor_train);
	keypoint_train.clear();
	for(unsigned int i=0;i<detected_keypoints.size();i++)
		keypoint_train.push_back(detected_keypoints[i]);
}

float FeatureTracker::time_elapsed(timespec start, timespec end)
{
	return (end.tv_sec + end.tv_nsec/1000000000.0) - (start.tv_sec + start.tv_nsec/1000000000.0);
}

float FeatureTracker::euclid_distance(cv::Point2f pt1, cv::Point2f pt2)
{
	return (float)sqrt(pow((double)(pt1.x - pt2.x),2)+pow((double)(pt1.y - pt2.y),2));
}

}


