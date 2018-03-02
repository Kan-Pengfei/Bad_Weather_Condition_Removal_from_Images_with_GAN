#ifndef Draw_Rain_Streak
#define Draw_Rain_Streak

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <random>
#include <ctime>
#include <cmath>

#include "Parameters.h"

#define pi 3.141592653

//std::default_random_engine generator;
//std::uniform_real_distribution<double> union_distribution; 

cv::Mat draw_rain_streak(cv::Mat rain_streak_image, int rain_steak_num, int rain_streak_length_pixel, int rain_streak_width_pixel, 
					     int image_width_pixel, int image_height_pixel, double rain_orientation, double sigma)
{
	// convert the orientation from angle to radian 
	double rain_orientation_radian = rain_orientation / 180 * pi; 

	// compute the kernel size of Gaussian Blur 
	cv::Size kernel_size = cv::Size(3, 3);
	if (rain_streak_width_pixel > 3) {
		int kernel_size_value = (rain_streak_width_pixel / 2) * 2 + 1;
		kernel_size = cv::Size(kernel_size_value, kernel_size_value);
	}

	for (int i = 0; i < rain_steak_num; ++i)
	{
		double random_union_num0 = (double)std::rand() / (double)RAND_MAX;
		int x_start_pixel = image_width_pixel * random_union_num0;
		double random_union_num1 = (double)std::rand() / (double)RAND_MAX;
		int y_start_pixel = image_height_pixel * random_union_num1;
		int x_end_pixel = x_start_pixel + rain_streak_length_pixel * sin(rain_orientation_radian);
		int y_end_pixel = y_start_pixel + rain_streak_length_pixel * cos(rain_orientation_radian);

		if (x_end_pixel < 0) {
			x_end_pixel = 0;
			y_end_pixel = y_start_pixel + (x_end_pixel - x_start_pixel) / tan(rain_orientation_radian); 
		}
		if (x_end_pixel > image_width_pixel) {
			x_end_pixel = image_width_pixel;
			y_end_pixel = y_start_pixel + (x_end_pixel - x_start_pixel) / tan(rain_orientation_radian);
		}
		if (y_end_pixel > image_height_pixel) {
			y_end_pixel = image_height_pixel;
			if (rain_orientation == 0) {
				x_end_pixel = x_start_pixel; 
			}
			else {
				x_end_pixel = x_start_pixel + (y_end_pixel - y_start_pixel) * tan(rain_orientation_radian);
			}
		}
		cv::Point start_point = cv::Point(x_start_pixel, y_start_pixel);
		cv::Point end_point = cv::Point(x_end_pixel, y_end_pixel);
		cv::line(rain_streak_image, start_point, end_point, rain_streak_color, rain_streak_width_pixel, CV_AA);
	}
	cv::GaussianBlur(rain_streak_image, rain_streak_image, kernel_size, sigma, IPL_BORDER_TRANSPARENT);
	return rain_streak_image;
}

#endif // !Draw_Rain_Streak
#pragma once
