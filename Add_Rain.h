#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <random>

#include "Parameters.h"
#include "Draw_Rain_Streak.h"

#define pi 3.141592653

int addRain(std::string original_image_path, std::string rain_added_image_path)
{
	// check whether there are already having the rain added images 
	cv::String rain_added_image_file = rain_added_image_path + "*" + image_type;
	std::vector<cv::String> rain_added_image_file_names;
	cv::glob(rain_added_image_file, rain_added_image_file_names, false);
	int rain_added_image_file_num = rain_added_image_file_names.size();

	if (rain_added_image_file_num == 0)
	{
		// load the original images to add rain
		cv::String original_file = original_image_path + "*" + image_type;
		std::vector<cv::String> original_file_names;
		cv::glob(original_file, original_file_names, false);
		int original_image_file_num = original_file_names.size();
		int rain_streak_img_num = 0; 

		for (int original_img_index = 0; original_img_index < original_image_file_num; ++original_img_index)
		{
			// get rain streak image size
			cv::Mat original_img = cv::imread(original_file_names[original_img_index]);
			cv::Size image_size = original_img.size();
			int image_width_pixel = image_size.width;
			int image_height_pixel = image_size.height;

			// calculate the pixel size of the screen in millmeter
			double pixel_size = 25.4 * monitor_diagonal /
				std::sqrt(std::pow(monitor_rsl_height, 2) + std::pow(monitor_rsl_width, 2));
			double screen_sensor_pixel_size_ratio = pixel_size / sensor_pixel_size;

			// calculate the rain streak ratio between the sensor and the image
			double sensor_image_height_ratio = sensor_rsl_height / image_height_pixel;
			double sensor_image_width_ratio = sensor_rsl_width / image_width_pixel;
			double rain_streak_num_ratio = pixel_size / sensor_pixel_size;

			// get the random seed for drawing streaks based on time 
			std::srand((unsigned)std::time(0));
			//initialize the number of different types of rain streak images
			int rain_streak_image_counter = 0;

			for (int i = 0; i < rain_diameter.size(); ++i)
			{
				double rain_velocity = 100 * std::pow(rain_diameter[i], 3) - 90 * std::pow(rain_diameter[i], 2)
					+ 5000 * rain_diameter[i] - 200;
				double raindrop_mass = (4.0 * pi / 3.0) * std::pow(rain_diameter[i] / 2.0, 3);

				for (int j = 0; j < focal_length.size(); ++j)
				{
					// compute the angle of view filed 
					double sensor_angle_view_hrz = sensor_width / focal_length[j];
					double angle_view_hrz = sensor_angle_view_hrz / sensor_image_width_ratio;
					double sensor_angle_view_vrt = sensor_height / focal_length[j];
					double angle_view_vrt = sensor_angle_view_vrt / sensor_image_height_ratio;
					double view_distance_max = rain_diameter[i] * focal_length[j] / sensor_pixel_size;

					for (int k = 0; k < exps_time.size(); ++k)
					{
						for (int l = 0; l < rain_intensity.size(); ++l)
						{
							for (int m = 0; m < rain_orientation.size(); ++m)
							{
								// start to draw rain streaks images
								cv::Mat rain_streak_image = cv::Mat::zeros(image_size, original_img.type());
								std::string rain_added_image_name = rain_added_image_path + std::to_string(original_img_index) +
									"_" + std::to_string(rain_streak_image_counter) + image_type;

								// draw the line of rain streaks based on the view distance
								int view_distance_level_num = view_distance_max / view_distance_level;
								std::cout << "Draw the rain added image: " << original_img_index << "_" << rain_streak_image_counter << std::endl;
								/*
								std::cout << "Drawing rain streaks...";
								std::cout << "The raindrop diameter: " << rain_diameter[i] << std::endl << std::endl;
								std::cout << "The focal length: " << focal_length[j] << std::endl << std::endl;
								std::cout << "The exposure time: " << exps_time[k] << std::endl << std::endl;
								std::cout << "The rain intensity: " << rain_intensity[l] << std::endl << std::endl;
								std::cout << "The rain orientation: " << rain_orientation[m] << std::endl << std::endl;
								*/
								for (int n = 0; n < (view_distance_level_num - 1); ++n)
								{
									double view_distance = n * view_distance_level + view_distance_level / 2;
									double view_distance_hrz = 2 * view_distance * std::tan(angle_view_hrz / 2.0);
									double view_distance_vrt = 2 * view_distance * std::tan(angle_view_vrt / 2.0);
									double rain_pass_time = view_distance_vrt / rain_velocity;
									int rain_streak_num_sensor = (rain_intensity[l] / 3600) * rain_pass_time * view_distance * view_distance_hrz / raindrop_mass;
									// the number of the rain streaks appear in the image need to divide the rain_streak_num_ratio
									// here is for convinience, I just used the original number to get enought streaks in small rain
									int rain_streak_num = rain_streak_num_sensor;  // rain_streak_num_ratio; 
									double rain_streak_length = (rain_diameter[i] + rain_velocity * exps_time[k])
										* focal_length[j] / (view_distance / 2);
									int rain_streak_length_pixel = rain_streak_length / sensor_pixel_size / sensor_image_height_ratio;
									double rain_streak_width = rain_diameter[i] * focal_length[j] / (view_distance / 2);
									int rain_streak_width_pixel = rain_streak_width / sensor_pixel_size / sensor_image_width_ratio;
									// the sigma value for gaussian blur 
									double sigma = rain_streak_width_pixel; //rain_streak_width;
																			//std::cout << "the number of rain streaks: " << rain_streak_num << std::endl;
																			/*
																			std::cout << std::endl;
																			std::cout << "The view distance: " << view_distance << std::endl;
																			std::cout << "the pass time of the raindrop " << rain_pass_time << std::endl;

																			std::cout << "rain streak length in pixel: " << rain_streak_length_pixel << std::endl;
																			std::cout << "rain streak width in pixel: " << rain_streak_width_pixel << std::endl;
																			std::cout << "the sigma for Gaussian Blur: " << sigma << std::endl << std::endl;
																			*/
									cv::Mat rain_streak_image_part = draw_rain_streak(rain_streak_image, rain_streak_num, rain_streak_length_pixel, rain_streak_width_pixel,
										image_width_pixel, image_height_pixel, rain_orientation[m], sigma);
									rain_streak_image = rain_streak_image + rain_streak_image_part;
								}

								// combine the rain streak image and the original image
								//std::cout << std::endl << "Adding rain to the image " << original_img_index << "...";
								cv::Mat rain_added_image = original_img + rain_streak_image;
								cv::imwrite(rain_added_image_name, rain_added_image);
								cv::waitKey(10);
								std::cout << std::endl;
								//cv::imwrite(rain_streak_image_name, rain_streak_image);
								//cv::waitKey(10);
								rain_streak_image_counter++;
							}
						}
					}
				}
			}
			rain_streak_img_num = rain_streak_image_counter; 
		}
		std::cout << std::endl << "Every clear image has " << rain_streak_img_num << " corresponding rain added images. " << std::endl; 
	}
	else
	{
		std::cout << "The rain added images have already been generated!" << std::endl;
	}
	return(0);
}
#pragma once
