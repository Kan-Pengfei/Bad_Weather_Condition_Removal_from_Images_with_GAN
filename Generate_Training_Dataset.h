#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <iostream>
#include <vector>

#include "Parameters.h"

int generateTrainingSet(std::string clear_img_path, std::string rain_added_img_path, 
	std::string dataset_path)
{
	std::cout << std::endl << "It is generating the training data set!" << std::endl; 
	// get the clear images 
	cv::String clear_img_file = clear_img_path + "*" + image_type; 
	std::vector<cv::String> clear_img_names; // store all image names from the folder
	cv::glob(clear_img_file, clear_img_names, false); // use glob function to access all images in a folder
	int clear_img_file_size = clear_img_names.size();
	for (int clear_img_index = 0; clear_img_index < clear_img_file_size; ++clear_img_index)
	{
		cv::Mat clear_image = cv::imread(clear_img_names[clear_img_index]);
		//if (clear_image.empty()) break; // check the file is opened successfully
		cv::Size image_size = clear_image.size();

		// get the corresponding rain added image file 
		cv::String rain_img_file = rain_added_img_path + std::to_string(clear_img_index)
			+ "_" + "*" + image_type; 
		std::vector<cv::String> rain_img_names;
		cv::glob(rain_img_file, rain_img_names, false); 
		int rain_img_file_size = rain_img_names.size(); 
		
		std::cout << "."; 
		for (int rain_img_index = 0; rain_img_index < rain_img_file_size; ++rain_img_index)
		{
			cv::Mat destination_image(image_size.height, image_size.width * 2, CV_8UC3);
			// place clear images in the left side 
			cv::Mat left_image(destination_image, cv::Rect(0, 0, image_size.width, image_size.height));
			clear_image.copyTo(left_image);
			// place rain images in the right side 
			cv::Mat rain_image = cv::imread(rain_img_names[rain_img_index]); 
			cv::Mat right_image(destination_image, cv::Rect(image_size.width, 0, image_size.width, image_size.height));
			rain_image.copyTo(right_image);

			// save images to specific folder
			std::string saving_name = dataset_path + std::to_string(clear_img_index) 
				+ "_" + std::to_string(rain_img_index) + image_type;

			cv::imwrite(saving_name, destination_image);
			cv::waitKey(10);
		}
	}
	//cv::waitKey(0);
	return 0;
}

#pragma once
