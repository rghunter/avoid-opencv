/*
 * SURF.cpp
 *
 *  Created on: Oct 7, 2012
 *      Author: rghunter
 */

#include "SURF.h"

namespace OpticalQuad {

SURF::SURF(const int octaves, const int intervals, const int init_sample,
        const float thresh) : octaves(octaves), intervals(intervals), init_sample(init_sample), thresh(thresh)
{
	data_log3.open("data/surf_perf.csv");

}
SURF::~SURF() {
	data_log3.close();
}
bool SURF::ExtractPoints(cv::Mat &input, vector<cv::KeyPoint> &keypoints_train)
{
	if(input.channels() > 1) //passed image must be grayscale
	{
		cout << "num channels: " << input.channels() << endl;
		return false;
	}

	timespec begin, end;

	cv::Mat rescaled_image;
	if(input.type() == CV_32FC1)
		input.copyTo(rescaled_image);
	else
		input.convertTo(rescaled_image,CV_32FC1,1.0/255.0,0);

	clock_gettime(CLOCK_MONOTONIC,&begin);

	IntegralImage(rescaled_image,integral_image);

	clock_gettime(CLOCK_MONOTONIC,&end);
	data_log3 << time_elapsed(begin,end) << ",";

	clock_gettime(CLOCK_MONOTONIC,&begin);
	BuildResponseMap(integral_image);

	clock_gettime(CLOCK_MONOTONIC,&end);
	data_log3 << time_elapsed(begin,end) << ",";

	keypoints_train.clear();

	clock_gettime(CLOCK_MONOTONIC,&begin);
	ResponseLayer *b, *m, *t;
	for (int o = 0; o < octaves; ++o) for (int i = 0; i <= 1; ++i)
	{
		b = responseMap.at(filter_map[o][i]);
		m = responseMap.at(filter_map[o][i+1]);
		t = responseMap.at(filter_map[o][i+2]);

		// loop over middle response layer at density of the most
		// sparse layer (always top), to find maxima across scale and space
		for (int r = 0; r < t->height; ++r)
			for (int c = 0; c < t->width; ++c)
				if (isExtremum(r, c, t, m, b))
					interpolateExtremum(r, c, t, m, b,keypoints_train);
	}

	clock_gettime(CLOCK_MONOTONIC,&end);
	data_log3 << time_elapsed(begin,end) << endl;

	return true;
}

void SURF::interpolateExtremum(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b,
		vector<cv::KeyPoint> &keypoints_train)
{
  // get the step distance between filters
  // check the middle filter is mid way between top and bottom
  int filterStep = (m->filter - b->filter);
  assert(filterStep > 0 && t->filter - m->filter == m->filter - b->filter);

  // Get the offsets to the actual location of the extremum
  double xi = 0, xr = 0, xc = 0;
  interpolateStep(r, c, t, m, b, &xi, &xr, &xc );

  // If point is sufficiently close to the actual extremum
  if( fabs( xi ) < 0.5f  &&  fabs( xr ) < 0.5f  &&  fabs( xc ) < 0.5f )
  {
    cv::KeyPoint ipt;
    ipt.pt.x = static_cast<float>((c + xc) * t->step);
    ipt.pt.y = static_cast<float>((r + xr) * t->step);
    ipt.size = static_cast<float>((0.1333f) * (m->filter + xi * filterStep));
    ipt.response = static_cast<int>(m->getLaplacian(r,c,t));
    keypoints_train.push_back(ipt);
  }
}

float SURF::time_elapsed(timespec start, timespec end)
{
	return (end.tv_sec + end.tv_nsec/1000000000.0) - (start.tv_sec + start.tv_nsec/1000000000.0);
}

void SURF::interpolateStep(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b,
                                  double* xi, double* xr, double* xc )
{
  cv::Mat dD, H, H_inv, X;
  double x[3] = { 0 };

  dD = *deriv3D( r, c, t, m, b );
  H = *hessian3D( r, c, t, m, b );
  H_inv = H.inv(cv::DECOMP_SVD);

  //cvInitMatHeader( &X, 3, 1, CV_64FC1, x, CV_AUTOSTEP );
  //cv::gemm( H_inv, dD, -1, NULL, 0, &X, 0 );
  X = H_inv * dD;

  *xi = x[2];
  *xr = x[1];
  *xc = x[0];
}

int SURF::isExtremum(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b)
{
  // bounds check
  int layerBorder = (t->filter + 1) / (2 * t->step);
  if (r <= layerBorder || r >= t->height - layerBorder || c <= layerBorder || c >= t->width - layerBorder)
    return 0;

  // check the candidate point in the middle layer is above thresh
  float candidate = m->getResponse(r, c, t);
  if (candidate < thresh)
    return 0;

  for (int rr = -1; rr <=1; ++rr)
  {
    for (int cc = -1; cc <=1; ++cc)
    {
      // if any response in 3x3x3 is greater candidate not maximum
      if (
        t->getResponse(r+rr, c+cc) >= candidate ||
        ((rr != 0 || cc != 0) && m->getResponse(r+rr, c+cc, t) >= candidate) ||
        b->getResponse(r+rr, c+cc, t) >= candidate
        )
        return 0;
    }
  }

  return 1;
}

//! Computes the partial derivatives in x, y, and scale of a pixel.
cv::Mat* SURF::deriv3D(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b)
{
  cv::Mat* dI;
  double dx, dy, ds;

  dx = (m->getResponse(r, c + 1, t) - m->getResponse(r, c - 1, t)) / 2.0;
  dy = (m->getResponse(r + 1, c, t) - m->getResponse(r - 1, c, t)) / 2.0;
  ds = (t->getResponse(r, c) - b->getResponse(r, c, t)) / 2.0;

  dI = new cv::Mat( 3, 1, CV_64FC1 );
  dI->at<double>(0,0) = dx;
  dI->at<double>(1,0) = dy;
  dI->at<double>(2,0) = ds;

  return dI;
}

//-------------------------------------------------------

//! Computes the 3D Hessian matrix for a pixel.
cv::Mat* SURF::hessian3D(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b)
{
  cv::Mat* H;
  double v, dxx, dyy, dss, dxy, dxs, dys;

  v = m->getResponse(r, c, t);
  dxx = m->getResponse(r, c + 1, t) + m->getResponse(r, c - 1, t) - 2 * v;
  dyy = m->getResponse(r + 1, c, t) + m->getResponse(r - 1, c, t) - 2 * v;
  dss = t->getResponse(r, c) + b->getResponse(r, c, t) - 2 * v;
  dxy = ( m->getResponse(r + 1, c + 1, t) - m->getResponse(r + 1, c - 1, t) -
          m->getResponse(r - 1, c + 1, t) + m->getResponse(r - 1, c - 1, t) ) / 4.0;
  dxs = ( t->getResponse(r, c + 1) - t->getResponse(r, c - 1) -
          b->getResponse(r, c + 1, t) + b->getResponse(r, c - 1, t) ) / 4.0;
  dys = ( t->getResponse(r + 1, c) - t->getResponse(r - 1, c) -
          b->getResponse(r + 1, c, t) + b->getResponse(r - 1, c, t) ) / 4.0;

  H = new cv::Mat( 3, 3, CV_64FC1 );
  H->at<double>(0,0) = dxx;
  H->at<double>(0,1) = dxy;
  H->at<double>(0,2) = dxs;
  H->at<double>(1,0) = dxy;
  H->at<double>(1,1) = dyy;
  H->at<double>(1,2) = dys;
  H->at<double>(2,0) = dxs;
  H->at<double>(2,1) = dys;
  H->at<double>(2,2) = dss;

  return H;
}


bool SURF::IntegralImage(cv::Mat &input_image, cv::Mat &integral_image)
{
	if(input_image.empty() || input_image.type() != CV_32FC1)
		return false;
	if(integral_image.empty() || !(integral_image.type() == CV_32FC1))
		integral_image.create(input_image.size(),CV_32FC1);

	float rs = 0.0f;
	for(int w = 0; w<input_image.cols;w++) //first row
	{
		rs += input_image.at<float>(0,w);
		integral_image.at<float>(0,w) = rs;
	}

	for(int r = 1;r<input_image.rows;r++) //rest of the image
	{
		rs = 0.0f;
		for(int w=0;w<input_image.cols;w++)
		{
			rs += input_image.at<float>(r,w);
			integral_image.at<float>(r,w) = integral_image.at<float>(r-1,w) + rs;
		}
	}
	return true;
}

float SURF::BoxIntegral(cv::Mat &img, int row, int col, int rows, int cols)
{
  float *data = (float *) img.data;
  int step = img.cols/sizeof(uchar);

  // The subtraction by one for row/col is because row/col is inclusive.
  int r1 = std::min(row,          img.rows) - 1;
  int c1 = std::min(col,          img.cols)  - 1;
  int r2 = std::min(row + rows,   img.rows) - 1;
  int c2 = std::min(col + cols,   img.cols)  - 1;

  float A(0.0f), B(0.0f), C(0.0f), D(0.0f);
  if (r1 >= 0 && c1 >= 0) A = data[r1 * step + c1];
  if (r1 >= 0 && c2 >= 0) B = data[r1 * step + c2];
  if (r2 >= 0 && c1 >= 0) C = data[r2 * step + c1];
  if (r2 >= 0 && c2 >= 0) D = data[r2 * step + c2];

  return std::max(0.f, A - B - C + D);
}

void SURF::BuildResponseMap(cv::Mat &int_image){

	 // Calculate responses for the first 4 octaves:
	  // Oct1: 9,  15, 21, 27
	  // Oct2: 15, 27, 39, 51
	  // Oct3: 27, 51, 75, 99
	  // Oct4: 51, 99, 147,195
	  // Oct5: 99, 195,291,387

	  // Deallocate memory and clear any existing response layers
	  for(unsigned int i = 0; i < responseMap.size(); ++i)
	    delete responseMap[i];
	  responseMap.clear();

	  // Get image attributes
	  int w = (int_image.cols / init_sample);
	  int h = (int_image.rows / init_sample);
	  int s = (init_sample);

	  // Calculate approximated determinant of hessian values
	  if (octaves >= 1)
	  {
	    responseMap.push_back(new ResponseLayer(w,   h,   s,   9));
	    responseMap.push_back(new ResponseLayer(w,   h,   s,   15));
	    responseMap.push_back(new ResponseLayer(w,   h,   s,   21));
	    responseMap.push_back(new ResponseLayer(w,   h,   s,   27));
	  }

	  if (octaves >= 2)
	  {
	    responseMap.push_back(new ResponseLayer(w/2, h/2, s*2, 39));
	    responseMap.push_back(new ResponseLayer(w/2, h/2, s*2, 51));
	  }

	  if (octaves >= 3)
	  {
	    responseMap.push_back(new ResponseLayer(w/4, h/4, s*4, 75));
	    responseMap.push_back(new ResponseLayer(w/4, h/4, s*4, 99));
	  }

	  if (octaves >= 4)
	  {
	    responseMap.push_back(new ResponseLayer(w/8, h/8, s*8, 147));
	    responseMap.push_back(new ResponseLayer(w/8, h/8, s*8, 195));
	  }

	  if (octaves >= 5)
	  {
	    responseMap.push_back(new ResponseLayer(w/16, h/16, s*16, 291));
	    responseMap.push_back(new ResponseLayer(w/16, h/16, s*16, 387));
	  }

	  // Extract responses from the image
	  for (unsigned int i = 0; i < responseMap.size(); ++i)
	  {
	    BuildResponseLayer(responseMap[i],int_image);
	  }

}
void SURF::BuildResponseLayer(ResponseLayer *rl,cv::Mat &int_image)
{
  float *responses = rl->responses;         // response storage
  unsigned char *laplacian = rl->laplacian; // laplacian sign storage
  int step = rl->step;                      // step size for this filter
  int b = (rl->filter - 1) / 2;             // border for this filter
  int l = rl->filter / 3;                   // lobe for this filter (filter size / 3)
  int w = rl->filter;                       // filter size
  float inverse_area = 1.f/(w*w);           // normalisation factor
  float Dxx, Dyy, Dxy;

  for(int r, c, ar = 0, index = 0; ar < rl->height; ++ar)
  {
    for(int ac = 0; ac < rl->width; ++ac, index++)
    {
      // get the image coordinates
      r = ar * step;
      c = ac * step;

      // Compute response components
      Dxx = BoxIntegral(int_image, r - l + 1, c - b, 2*l - 1, w)
          - BoxIntegral(int_image, r - l + 1, c - l / 2, 2*l - 1, l)*3;
      Dyy = BoxIntegral(int_image, r - b, c - l + 1, w, 2*l - 1)
          - BoxIntegral(int_image, r - l / 2, c - l + 1, l, 2*l - 1)*3;
      Dxy = + BoxIntegral(int_image, r - l, c + 1, l, l)
            + BoxIntegral(int_image, r + 1, c - l, l, l)
            - BoxIntegral(int_image, r - l, c - l, l, l)
            - BoxIntegral(int_image, r + 1, c + 1, l, l);

      // Normalise the filter responses with respect to their size
      Dxx *= inverse_area;
      Dyy *= inverse_area;
      Dxy *= inverse_area;

      // Get the determinant of hessian response & laplacian sign
      responses[index] = (Dxx * Dyy - 0.81f * Dxy * Dxy);
      laplacian[index] = (Dxx + Dyy >= 0 ? 1 : 0);

#ifdef RL_DEBUG
      // create list of the image coords for each response
      rl->coords.push_back(std::make_pair<int,int>(r,c));
#endif
    }
  }
}
void SURF::FastHessian(){

}



} /* namespace OpticalQuad */
