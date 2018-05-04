// The generation of the Perlin noise
// The code is taken reference from https://github.com/yoggy/cv_perlin_noise
// Call the createPerlinNoiseImage function to generate perlin noise 
// The generated noise image has double intensity type 
//////////////////////////////////

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <cmath>

class PerlinNoise
{
protected:
	double fade(double t); 
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
public:
	int p[512];
	PerlinNoise(); 
	double noise(const double &src_x, const double &src_y, const double &src_z); 
	cv::Mat createPNoiseImg(const cv::Size &img_size, const double &scale, const double &noise_cff); // create one layer of perlin noise 
	cv::Mat addMultiplePNoise(cv::Mat &input_img, double noise_ratio);
};
#pragma once
