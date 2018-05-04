#ifndef Weather_Effect_Addition_H
#define Weather_Effect_Addition_H

// this file add the fog, snow and rain effect to clear images 
// you need to offer the path of input clear images and the path for the storage of processed images 
// all the input images should be in RGB channels 
// Written by Peter Kan, May 3, 2018
///////////////////////////////////////////

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <random>
#include <ctime>

#include "My_Functions.h"
#include "Fog_Image.h"
#include "Snow_Image.h"
#include "Rain_Image.h"

class Weather_Effect_Addition
{
public:
	Weather_Effect_Addition();
	~Weather_Effect_Addition();
	// the img_types are a vector of differencet image types, like "jpg", "png", that contains in your input file
	static void add(cv::String input_file_path, cv::String output_file_path, std::vector<cv::String> img_types); 
	// generate the training images for the GAN network, put the clear image and the weather effect image side by side 
	static void add4GAN(cv::String input_file_path, cv::String output_file_path, std::vector<cv::String> img_types); 
};

#endif // !Weather_Effect_Addition

