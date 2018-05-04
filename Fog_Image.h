// This is a class of the fog image 
// The theory is based on the model of J = f*t + A(1 - t)
// where the f is the clear image, A is the general sky light of the clear image
// t is the medium transmission matrix, which we need to estimate from the clear image f
// and J is the radiance scene of the fog image
// the final fog image g = J * (1 - alpha) + alpha * p
// where p is the perlin noise and alpha is the weight 
// Written by Pengfei Kan on Dec 2017 
#ifndef FogImg_Class
#define FogImg_Class

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#include "Guided_Filter.h" // the guided filter file for the edge smoothing to estimate t
#include "Perlin_Noise.h"
#include "My_Functions.h"

class FogImg
{
private: 
	double noise_ratio = 0.7;	// the percentage of the brightness of perlin noise accounts in the fog image 
								// the larger the more obvious the fog effect is 
	double sky_light = 1;		// for clear image the sky light is always 1 
	int patch_size = 15;		// the size of the patch used to get the dark channel image and guided filter 
	double guided_filter_eps;	// coefficient for the guided filter 

	cv::Mat dark_veil;		// used to estimate t 
	cv::Mat trans_mat;		// t
	cv::Mat clear_img;		// f
	cv::Mat radiance_scene; // J 
	cv::Mat perlin_noise;	// p
	cv::Mat fog_img;		// g 
public:
	FogImg(cv::Mat input_img); // constructor, will convert the image to double type 
	FogImg(cv::Mat input_img, double input_noise_ratio); 
	double getNoiseRatio(); 
	void setNoiseRatio(double input_ratio); 
	cv::Mat getClearImg(); 
	double getSkyLight(); 
	void setSkyLight(double input_sky_light); 
	cv::Mat estDarkVeil(); // estimate the dark chanel of the image using patch 
	cv::Mat estDarkVeilFast(); // only get the minimum intensity between channels  
	cv::Mat calcTransM();	// calculate the medium transmission matrix t
	cv::Mat calcRdScene();	// calculate the radiance scene J 
	cv::Mat getFogImg(); 
};
#endif // !FogImg_Class
#pragma once
