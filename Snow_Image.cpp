#include "Snow_Image.h"
#include "My_Functions.h"

SnowImg::SnowImg(cv::Mat input_img) : RainImg(input_img)// constructor with defalt camera parameters 
{
	snow_orint_mean = (double)(std::rand() % (2 * snow_orient_mean_range) - snow_orient_mean_range) / 180 * pi;
	snow_intensity = (double)rand() / RAND_MAX * (snow_intensity_max - snow_intensity_min) + snow_intensity_min;
}

SnowImg::SnowImg(cv::Mat input_img, double input_sensor_w, double input_sensor_h, double input_sensor_pixel_size,
	double input_f_length, double input_exps_t) // constructor with input camera parameters
	:RainImg(input_img, input_sensor_w, input_sensor_h, input_sensor_pixel_size, input_f_length, input_exps_t)
{
	snow_orint_mean = (double)(std::rand() % (2 * snow_orient_mean_range) - snow_orient_mean_range) / 180 * pi;
	snow_intensity = (double)rand() / RAND_MAX * (snow_intensity_max - snow_intensity_min) + snow_intensity_min;
}

cv::Mat SnowImg::getClearImg()
{
	return clear_img; 
}
double SnowImg::getSnowflakeRmax()
{
	return snowflake_r_max; 
}
int SnowImg::getSnowOrientmeanRange()
{
	return snow_orient_mean_range;
}
int SnowImg::getSnowOrientVarRange()
{
	return snow_orient_var_range; 
}
double SnowImg::getSnowIntn()
{
	return snow_intensity;
}
double SnowImg::getSnowIntnMax()
{
	return snow_intensity_max; 
}
double SnowImg::getSnowIntnMin()
{
	return snow_intensity_min; 
}

void SnowImg::setSnowflakeRmax(double input_value)
{
	snowflake_r_max = input_value;
}
void SnowImg::setSnowOrientmeanRange(double input_value)
{
	snow_orient_mean_range = input_value; 
}
void SnowImg::setSnowOrientVarRange(int input_value)
{
	snow_orient_var_range = input_value; 
}
void SnowImg::setSnowIntnMax(double input_value)
{
	snow_intensity_max = input_value; 
}
void SnowImg::setSnowIntnMin(double input_value)
{
	snow_intensity_min = input_value; 
}

cv::Mat SnowImg::generateSnowMask()
{
	int sensor_rsl_w = sensor_w / sensor_pixel_size; // get the sensor resolution size 
	int sensor_rsl_h = sensor_h / sensor_pixel_size;
	int snow_mask_w = sensor_rsl_w * 1.1;
	int snow_mask_h = sensor_rsl_h * 1.1;
	cv::Mat snow_mask_img = cv::Mat::zeros(cv::Size(snow_mask_w, snow_mask_h), CV_64F);
	// figure out the number of raindrops 
	double n = 250 / 22.9 / pow(snow_intensity, -0.49);  // the number density of raindrops in the volume / m3
	double Zm = 2.0 * snowflake_r_max * f_length / sensor_pixel_size; // the length of the filed of view 
	double N = pow((Zm - min_focus_distance) / 1000, 3) * sensor_h * sensor_w / pow(f_length, 2) * n; // the total number of raindrops 
	N = N / 2.0; // suppose only half of the snowfalkes face the camera with with the front or back side 

	// draw rain streaks 
	for (int i = 0; i < N; ++i)
	{
		double z = (double)rand() / RAND_MAX * (Zm - min_focus_distance) + min_focus_distance; //get the distance of the raindrop from the camera
																							   // use the uniform distribution to get the raindrop radius distribution  
		double snowflake_d_uni = (double)rand() / RAND_MAX;
		if (snowflake_d_uni == 1) snowflake_d_uni = 0.5;	//make sure log function is valid 
		snowflake_r = -1 * log(1 - snowflake_d_uni) / 22.9 / pow(snow_intensity, -0.45) * snowflake_r_max;

		int streak_b = 2.0 * snowflake_r * f_length / z / sensor_pixel_size; //get the breadth of rain streak in pixels 
		if (streak_b >= 1) //the rain streak should at least wider than one pixel
		{
			double v = 100.0 / pow(snowflake_r / 1000, 0.2); //terminal velocity of raindrop in mm/s
			int streak_l = (2.0 * snowflake_r + v * exps_t) * f_length / z / sensor_pixel_size; //length of rain streak in pixel
			snow_orient = snow_orint_mean + (double)((rand() % snow_orient_var_range) * 2 - snow_orient_var_range) / 180 * pi; 
			int x_start_pixel = snow_mask_w * ((double)rand() / RAND_MAX); //get the random start position of the streak
			int y_start_pixel = snow_mask_h * ((double)rand() / RAND_MAX);
			int x_end_pixel = x_start_pixel + streak_l * sin(snow_orient);
			int y_end_pixel = y_start_pixel + streak_l * cos(snow_orient);
			// tackle the situation the rain streak passes over the boundry 
			if (x_end_pixel < 0) {
				x_end_pixel = 0;
				y_end_pixel = y_start_pixel + (x_end_pixel - x_start_pixel) / tan(snow_orient);
			}
			else if (x_end_pixel > snow_mask_w) {
				x_end_pixel = snow_mask_w;
				y_end_pixel = y_start_pixel + (x_end_pixel - x_start_pixel) / tan(snow_orient);
			}
			else if (y_end_pixel > snow_mask_h) {
				y_end_pixel = snow_mask_h;
				if (snow_orient == 0) {
					x_end_pixel = x_start_pixel;
				}
				else {
					x_end_pixel = x_start_pixel + (y_end_pixel - y_start_pixel) * tan(snow_orient);
				}
			}
			cv::Point start_point = cv::Point(x_start_pixel, y_start_pixel);
			cv::Point end_point = cv::Point(x_end_pixel, y_end_pixel);
			cv::line(snow_mask_img, start_point, end_point, snowflake_color, streak_b, CV_AA);
		}
	}
	cv::Size kernel_size = cv::Size(15, 15);
	cv::GaussianBlur(snow_mask_img, snow_mask_img, kernel_size, 0, IPL_BORDER_TRANSPARENT);
	//trim the rain mask image to the sensor size 
	cv::Rect roi = cv::Rect((int)sensor_rsl_w * 0.05, (int)sensor_rsl_h * 0.05, sensor_rsl_w, sensor_rsl_h);
	snow_mask_img = snow_mask_img(roi);
	return snow_mask_img;
}

cv::Mat SnowImg::addSnow()
{
	cv::Mat snow_add_img;
	cv::Mat snow_mask = generateSnowMask();
	int img_w = clear_img.cols;
	int img_h = clear_img.rows;
	// for image size smaller than the sensor relosution size
	// downsize the rain mask image to the clear image size 
	if (img_w <= snow_mask.cols && img_h <= snow_mask.rows)
	{
		cv::Size down_size = cv::Size(img_w, img_h);
		cv::Mat snow_mask_img;
		cv::resize(snow_mask, snow_mask_img, down_size);

		std::vector<cv::Mat> chns;
		cv::split(clear_img, chns);
		// take the max value of three channels to calculate the mean
		cv::Mat max_chn = cv::max(chns[0], chns[1]);
		max_chn = cv::max(max_chn, chns[2]);
		cv::Scalar means = cv::mean(max_chn);
		double mean = means.val[0];
		cv::Mat I = cv::Mat::ones(clear_img.size(), CV_64F); 
		for (int i = 0; i < 3; ++i)
			chns[i] = chns[i].mul(0.7) + snow_mask_img.mul(0.5);
		cv::merge(chns, snow_add_img);
		return snow_add_img;
	}
	else
	{
		std::cout << "The inut image is out of range of most current camera sensor size, please set the new sensor size!" << std::endl;
		return clear_img;
	}
}
