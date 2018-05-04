#ifndef Snow_Image_H
#define Snow_Image_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "Rain_Image.h"

class SnowImg : public RainImg
{
private:
	double snowflake_r; // in mm 
	double snowflake_r_max = 5;
	double snow_orient;
	double snow_orint_mean;
	int snow_orient_mean_range = 45; // in degree
	int snow_orient_var_range = 20;
	double snow_intensity; // in mm/hr
	double snow_intensity_max = 5.0;
	double snow_intensity_min = 0.5;
	double snowflake_brightness = 1.0;
	cv::Scalar snowflake_color = cv::Scalar(snowflake_brightness, snowflake_brightness, snowflake_brightness); 

public:
	SnowImg(cv::Mat input_img); // constructor with defalt camera parameters 
	SnowImg(cv::Mat input_img, double input_sensor_w, double input_sensor_h, double input_sensor_pixel_size,
		double input_f_length, double input_exps_t); // constructor with input camera parameters

	cv::Mat getClearImg(); 
	double getSnowflakeRmax();
	int getSnowOrientmeanRange();
	int getSnowOrientVarRange();
	double getSnowIntn(); 
	double getSnowIntnMax();
	double getSnowIntnMin();

	void setSnowflakeRmax(double input_value);
	void setSnowOrientmeanRange(double input_value);
	void setSnowOrientVarRange(int input_value);
	void setSnowIntnMax(double input_value);
	void setSnowIntnMin(double input_value);

	cv::Mat generateSnowMask();
	cv::Mat addSnow();
};
#endif // !Snow_Image
