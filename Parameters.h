#ifndef Parameters
#define Parameters

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <vector>
#include <iostream>

// file paths 
// clear images used to generate training dataset 
cv::String clear_img_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Clear-Images/";
// different image formats that of the clear images 
std::vector<cv::String> img_types = {"jpg", "png"};  //you should add the image types based on your input images  

// images for the training of the model 
std::string training_dataset_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Dataset/training/";

// dataset of the model 
std::string dataset_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Dataset";

// the model of the GAN 
std::string trained_model_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Trained-Models/";

// the images with fog, rain or snow that need to be processed 
std::string img_to_process_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Your-Images/"; 

// testing dataset 
std::string testing_dataset_path = "/media/pengfei/DATA/Research-McMaster/My-GAN/Dataset/testing/";

#endif // !Parameters
#pragma once
