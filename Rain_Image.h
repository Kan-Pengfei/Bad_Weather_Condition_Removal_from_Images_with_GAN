#ifndef Rain_Image
#define Rain_Image
// This is the head file of the RainImg class 
// This code is used to render rain streaks in the clear weather image
// all the length unit is mm 
// all images are in double type 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <random>
#include <cmath>
#include <vector>

#define pi 3.141592653

class RainImg
{
private:
	// parameters about the rain 
	double rain_intensity;		// randomly chosen from (2.5, 30) 
	double rain_intensity_max = 30.0;
	double rain_intensity_min = 2.5;
	double rain_orient;			// randomly chosen from (-45Pi / 180, 45Pi / 180) in radian
	int rain_orient_range = 45;
	double raindrop_r;
	double raindrop_r_max = 2.0;  // the max randrop radius, the default value is 2mm
	double rain_brightness = 1.0; // the brightness increase caused by rain streaks  
	cv::Scalar rainstreak_color = cv::Scalar(rain_brightness, rain_brightness, rain_brightness); // define the color in 3 channels 

protected:
	// the input image
	cv::Mat clear_img; 

	// parameters about the camera, 
	// the default is 1" sensor with 30 mm focal length 
	double sensor_w = 32.0;			  //sensor physical dimension (mm * mm)
	double sensor_h = 24.0;
	double sensor_pixel_size = 0.006; //dimension of the sensor pixel 
	double f_length = 45.0;	  //the focal length of the camera
	// based on IOS standard, I chose several common shutter speed 
	std::vector<double> exps_t_pool = { 1.0/30, 1.0/60, 1.0/125, 1.0/250, 1.0/500}; 
	double exps_t;			 //choose from the pool
	double min_focus_distance = 30 * f_length; 

public:
	RainImg(cv::Mat input_img); // constructor with defalt camera parameters 
	RainImg(cv::Mat input_img, double input_sensor_w, double input_sensor_h, double input_sensor_pixel_size,
		double input_f_length, double input_exps_t); // constructor with input camera parameters

	// get and set methods of class parameters 
	double getSensorW();
	double getSensorH();
	double getSensorPixelSize(); 
	double getFLength();
	double getExpsT(); 
	double getMinFDist();	//get min focus distance  
	double getRainInts(); 
	double getRainIntsMax(); 
	double getRainIntsMin();
	double getRainOrient();
	int getRainOrientRange(); 
	double getRaindropMaxR(); 
	double getRaindropR();
	double getRainBrightness(); 
	cv::Mat getClearImg(); 

	void setSensorW(double input_value);
	void setSensorH(double input_value);
	void setSensorPixelSize(double input_value);
	void setFLength(double input_value);
	void setExpsTs(std::vector<double> input_exps_ts); 
	void setExpsT(double input_value);
	void setMinFDist(double input_value); 
	void setRainInts(double input_value);
	void setRainIntsMax(double input_value);
	void setRainIntsMin(double input_value);
	void setRainOrient(double input_value);
	void setRainOrientRange(int input_value); 
	void setRaindropR(double input_value);
	void setRainBrightness(double input_value);
	void setClearImg(cv::Mat input_img); 

	cv::Mat generateRainMask(); 
	cv::Mat addRain(); 
};
#endif // !Rain_Image
#pragma once
