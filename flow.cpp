//SurfFeatureDetector detector(2000,4);
	GridAdaptedFeatureDetector detector(new SurfFeatureDetector(2000,4), 100, 4,4);
	FREAK extractor;
	Mat raw_frame(input);
	BruteForceMatcher <Hamming> matcher;
	vector<KeyPoint> keypoint;
	Mat descriptor;
	Mat output_frame;
	vector<DMatch> matches;

	if(first)
	{
		fixed_image = raw_frame.clone();
		detector.detect(raw_frame,keypoint_fixed);
		extractor.compute(raw_frame,keypoint_fixed,fixed_descriptor);
		first = false;
		return;
	}

	detector.detect(raw_frame,keypoint);
	extractor.compute(raw_frame,keypoint,descriptor);
	matcher.match(fixed_descriptor,descriptor,matches);
	//drawMatches(raw_frame,keypoint_fixed,raw_frame,keypoint,matches,output_frame);
	//imshow("output",output_frame);
	std::cout << "Num Points: " << keypoint.size() << "Num Matches: " << matches.size() << std::endl;
	drawKeypoints(raw_frame,keypoint,raw_frame,Scalar(255,0,0));
	drawKeypoints(raw_frame,keypoint_fixed,raw_frame,Scalar(0,255,0),DrawMatchesFlags::DRAW_OVER_OUTIMG);

	*output = raw_frame;

	//memcpy(output->imageData,input->imageData,output->imageSize);
	return;
