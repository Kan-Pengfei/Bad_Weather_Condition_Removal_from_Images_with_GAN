#ifndef Parameters
#define Parameters

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <vector>
#include <iostream>

// file paths 

std::string original_image_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Clear-Images/";
std::string rain_added_image_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Rain-Added-Images/";
std::string training_dataset_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Rain-Dataset/training/";
std::string dataset_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Rain-Dataset";

// the original images for generating training dataset
//std::string original_image_path = "/media/pengfei/OS/My-Code/C++/Rain-Streaks-Generation/Original-Images/"; 
//std::string rain_added_image_path = "/media/pengfei/OS/My-Code/C++/Rain-Streaks-Generation/Rain-Added-Images/";
//std::string training_dataset_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Training-Dataset-Test/training/";
// the general dataset path, which includes training, and testing 
//std::string dataset_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Training-Dataset-Test";
// the model after training will be stored here 
std::string trained_model_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Trained-Models/";
// the test image file 
std::string image_to_process_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Your-Images/"; 
std::string testing_dataset_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Rain-Dataset/testing/";

std::string image_type = ".jpg"; 

// sensor size in millmeter
double sensor_width = 13.2;
double sensor_height = 8.8;
double sensor_rsl_width = 5500; // unit is pixel
double sensor_rsl_height = 3667; 
double sensor_pixel_size = 0.0024;  // one inch 20 megapixel camera sensor 

// monitor diagonal in inch
double monitor_diagonal = 15.4;
// monitor resolution in pixels
double monitor_rsl_width = 1920;
double monitor_rsl_height = 1080;

// the distance of view field to draw rain streaks
// since the long distance contains too many raindrops
// it's impossible to draw all of them
// we only draw the rain among view_distance_max / view_distance_ratio
double view_distance_level = 1000; 

// the intnensity of rain fall
std::vector<double> rain_intensity = { 2.5, 5.0};

// the orientation of rain streaks 
std::vector<double> rain_orientation = { -30, -20, -10, 0, 10, 20, 30 };

// camera focal length in millmeter
std::vector<double> focal_length = { 35.0, 45.0};

// camera exposure time 
// double number needs decimal point to trigger decimal figure
std::vector<double> exps_time = { 1.0 / 30, 1.0 / 60, 1.0 / 125.0};  

// diameter of raindrops
std::vector<double> rain_diameter = {0.2, 0.35, 0.5};

// the brightness of the rain streaks
int rain_streak_intensity_value = 10; 
cv::Scalar rain_streak_color = cv::Scalar(rain_streak_intensity_value, rain_streak_intensity_value, rain_streak_intensity_value);



#endif // !Parameters
#pragma once
