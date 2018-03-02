#ifndef Preprocess_Image
#define Preprocess_Image

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <vector>
#include <iostream>

int preprocessImg(std::string image_to_process_path, std::string image_restorage_path)
{
	// access to all images need to be processed 
	cv::String img_path = image_to_process_path + "*" + image_type;  
	std::vector<cv::String> file_names; // store all image names from the folder
	cv::glob(img_path, file_names, false); // use glob function to access all images in a folder
	int file_size = file_names.size(); 

	for (int i = 0; i < file_size; ++i)
	{
		cv::Mat source_image = cv::imread(file_names[i]);
		//if (source_image.empty()) break; // check the file is opened successfully

		// place two images side by side
		cv::Size image_size = source_image.size();
		cv::Mat destination_image(image_size.height, image_size.width * 2, CV_8UC3);
		cv::Mat left_image(destination_image, cv::Rect(0, 0, image_size.width, image_size.height));
		source_image.copyTo(left_image);
		cv::Mat right_image(destination_image, cv::Rect(image_size.width, 0, image_size.width, image_size.height));
		source_image.copyTo(right_image);

		// save images to the testing dataset for processing latter 
		std::string saving_name = testing_dataset_path + std::to_string(i) + image_type;
		cv::imwrite(saving_name, destination_image);
		cv::waitKey(10);
	}
}

#endif 