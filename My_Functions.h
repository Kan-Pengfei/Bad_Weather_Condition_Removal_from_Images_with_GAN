// define my own functions 

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

namespace MyFuncs
{
	class MyImgFuncs
	{
	public:
		static cv::Mat myConvertTo(cv::Mat input_img, std::string dst_type);	// convert image types 
		static cv::Mat homoFilter(cv::Mat input_img); 
		static cv::Mat combineParal(cv::Mat left_img, cv::Mat right_img);		// put two images together in a row 
	};

	class MyStrFuncs
	{
	public:
		static std::vector<cv::String> concat(std::vector<cv::String> input_file_paths);
	};
}

#pragma once
