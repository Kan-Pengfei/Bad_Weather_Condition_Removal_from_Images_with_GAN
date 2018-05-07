#include "Rain_Image.h"
#include "My_Functions.h"

RainImg::RainImg(cv::Mat input_img) : clear_img(input_img)
{
	clear_img = MyFuncs::MyImgFuncs::myConvertTo(clear_img, "double");
	// deal with portrait instead of landscope 
	if (input_img.rows > input_img.cols) // swap the sensor size 
	{
		double temp = sensor_w;
		sensor_w = sensor_h;
		sensor_h = temp;
	}
	// if the input image is too small, adjust the size 
	if (input_img.rows < 1000 || input_img.cols < 1000)
	{
		double w_ratio = 1000 / (double)input_img.rows;
		double h_ratio = 1000 / (double)input_img.cols;
		double size_ratio = w_ratio > h_ratio ? w_ratio : h_ratio;
		sensor_w /= size_ratio;
		sensor_h /= size_ratio;
	}
	// choose the exposure time from the the exposure time pool
	int exps_t_index = std::rand() % exps_t_pool.size(); 
	exps_t = exps_t_pool[exps_t_index];
	
	rain_orient = (double)(std::rand() % (2 * rain_orient_range) - rain_orient_range) / 180 * pi; 
	rain_intensity = (double)rand() / RAND_MAX * (rain_intensity_max - rain_intensity_min) + rain_intensity_min;
}

RainImg::RainImg(cv::Mat input_img, double input_sensor_w, double input_sensor_h, 
	double input_sensor_pixel_size, double input_f_length, double input_exps_t) // constructor with input camera parameters
	: clear_img(input_img), sensor_w(input_sensor_w), sensor_h(input_sensor_h),
	sensor_pixel_size(input_sensor_pixel_size), f_length(input_f_length), exps_t(input_exps_t)
{
	clear_img = MyFuncs::MyImgFuncs::myConvertTo(clear_img, "double");
	min_focus_distance = 20 * f_length; 
	// deal with portrait instead of landscope 
	if (input_img.rows > input_img.cols) // swap the sensor size 
	{
		double temp = sensor_w;
		sensor_w = sensor_h;
		sensor_h = temp;
	}

	rain_orient = 0; // (double)(std::rand() % (2 * rain_orient_range) - rain_orient_range) / 180 * pi;
	rain_intensity = 5.0; // (double)rand() / RAND_MAX * (rain_intensity_max - rain_intensity_min) + rain_intensity_min;
}

double RainImg::getSensorW()
{
	return sensor_w; 
}
double RainImg::getSensorH()
{
	return sensor_h;
}
double RainImg::getSensorPixelSize()
{
	return sensor_pixel_size; 
}
double RainImg::getFLength()
{
	return f_length;
}
double RainImg::getExpsT()
{
	return exps_t;
}
double RainImg::getMinFDist()
{
	return min_focus_distance; 
}
double RainImg::getRainInts()
{
	return rain_intensity;
}
double RainImg::getRainIntsMax()
{
	return rain_intensity_max; 
}
double RainImg::getRainIntsMin()
{
	return rain_intensity_min;
}
double RainImg::getRainOrient()
{
	return rain_orient; 
}
int RainImg::getRainOrientRange()
{
	return rain_orient_range; 
}
double RainImg::getRaindropMaxR()
{
	return raindrop_r_max;
}
double RainImg::getRaindropR()
{
	return raindrop_r;
}
double RainImg::getRainBrightness()
{
	return rain_brightness;
}
cv::Mat RainImg::getClearImg()
{
	return clear_img; 
}

void RainImg::setSensorW(double input_value)
{
	sensor_w = input_value;
}
void RainImg::setSensorH(double input_value)
{
	sensor_h = input_value;
}
void RainImg::setSensorPixelSize(double input_value)
{
	sensor_pixel_size = input_value;
}
void RainImg::setFLength(double input_value)
{
	f_length = input_value;
}
void RainImg::setExpsTs(std::vector<double> input_exps_ts)
{
	exps_t_pool = input_exps_ts;
}
void RainImg::setExpsT(double input_value)
{
	exps_t = input_value;
}
void RainImg::setMinFDist(double input_value)
{
	min_focus_distance = input_value; 
}
void RainImg::setRainInts(double input_value)
{
	rain_intensity = input_value;
}
void RainImg::setRainIntsMax(double input_value)
{
	rain_intensity_max = input_value; 
}
void RainImg::setRainIntsMin(double input_value)
{
	rain_intensity_min = input_value;
}
void RainImg::setRainOrient(double input_value)
{
	rain_orient = input_value;
}
void RainImg::setRainOrientRange(int input_value)
{
	rain_orient_range = input_value; 
}
void RainImg::setRaindropR(double input_value)
{
	raindrop_r = input_value;
}
void RainImg::setRainBrightness(double input_value)
{
	rain_brightness = input_value;
}
void RainImg::setClearImg(cv::Mat input_img)
{
	clear_img = input_img; 
}

// generate the rain mask image that has the same size as the clear image
// considering the boder of the image
// we first generate a larger size of rain mask image and then trim it ot the sensor size 
cv::Mat RainImg::generateRainMask()
{
	int sensor_rsl_w = sensor_w / sensor_pixel_size; // get the sensor resolution size 
	int sensor_rsl_h = sensor_h / sensor_pixel_size;
	int rain_mask_w = sensor_rsl_w * 1.2;
	int rain_mask_h = sensor_rsl_h * 1.2;
	cv::Mat rain_mask_img = cv::Mat::zeros(cv::Size(rain_mask_w, rain_mask_h), CV_64F);
	// figure out the number of raindrops 
	double n = 8000 / 82 / pow(rain_intensity, -0.21);  // the number density of raindrops in the volume / m3
	double Zm = 2 * raindrop_r_max * f_length / sensor_pixel_size; // the length of the filed of view 
	double N = pow((Zm - min_focus_distance) / 1000, 3) * sensor_h * sensor_w / pow(f_length, 2) * n; // the total number of raindrops 
	std::vector<int> streaks_bs; //store all the visible streak breadth 

	// draw rain streaks 
	for (int i = 0; i < N; ++i)
	{
		double z = (double)rand() / RAND_MAX * (Zm - min_focus_distance) + min_focus_distance; //get the distance of the raindrop from the camera
		// use the uniform distribution to get the raindrop radius distribution  
		double raindrop_r_uni = (double)rand() / RAND_MAX; 
		if (raindrop_r_uni == 1) raindrop_r_uni = 0.5;	//make sure log function is valid 
		raindrop_r = -1 * log(1 - raindrop_r_uni) / 82 / pow(rain_intensity, -0.21) * raindrop_r_max; 
		
		int streak_b = 2 * raindrop_r * f_length / z / sensor_pixel_size ; //get the breadth of rain streak in pixels 
		if (streak_b >= 1) //the rain streak should at least wider than one pixel
		{
			streaks_bs.push_back(streak_b); 
			double v = 200 * sqrt(raindrop_r / 1000) * 1000;//terminal velocity of raindrop in mm/s
			int streak_l = (2 * raindrop_r + v * exps_t) * f_length / z / sensor_pixel_size; //length of rain streak in pixel
			int x_start_pixel = rain_mask_w * ((double)rand() / RAND_MAX); //get the random start position of the streak
			int y_start_pixel = rain_mask_h * ((double)rand() / RAND_MAX);
			int x_end_pixel = x_start_pixel + streak_l * sin(rain_orient);
			int y_end_pixel = y_start_pixel + streak_l * cos(rain_orient);
			// tackle the situation the rain streak passes over the boundry 
			if (x_end_pixel < 0) 
			{
				x_end_pixel = 0;
				y_end_pixel = y_start_pixel + (x_end_pixel - x_start_pixel) / tan(rain_orient);
			}
			else if (x_end_pixel > rain_mask_w) 
			{
				x_end_pixel = rain_mask_w;
				y_end_pixel = y_start_pixel + (x_end_pixel - x_start_pixel) / tan(rain_orient);
			}
			else if (y_end_pixel > rain_mask_h) 
			{
				y_end_pixel = rain_mask_h;
				if (rain_orient == 0) {
					x_end_pixel = x_start_pixel;
				}
				else {
					x_end_pixel = x_start_pixel + (y_end_pixel - y_start_pixel) * tan(rain_orient);
				}
			}
			cv::Point start_point = cv::Point(x_start_pixel, y_start_pixel);
			cv::Point end_point = cv::Point(x_end_pixel, y_end_pixel);
			cv::line(rain_mask_img, start_point, end_point, rainstreak_color, streak_b, CV_AA);
		}
	}
	// gaussian blur the rain mask image 
	double streak_b_ave = 0;  //average breadth of the visible rain streaks 
	for (int i = 0; i < streaks_bs.size(); ++i)
	{
		streak_b_ave += streaks_bs[i]; 
	}
	int streak_num = streaks_bs.size(); 
	if (streak_num != 0) streak_b_ave /= (double)streaks_bs.size(); 
	else streak_b_ave = 0.1; 
	double sigma = streak_b_ave;  
	int kernel_size_value = ((int)streak_b_ave / 2) * 2 + 1; //make the kernel size be odd number of pixels 
	if (kernel_size_value == 1) kernel_size_value = 3; 
	cv::Size kernel_size = cv::Size(kernel_size_value, kernel_size_value); 
	cv::GaussianBlur(rain_mask_img, rain_mask_img, kernel_size, sigma, IPL_BORDER_TRANSPARENT);
	//trim the rain mask image to the sensor size 
	cv::Rect roi = cv::Rect((int)sensor_rsl_w * 0.1, (int)sensor_rsl_h * 0.1, sensor_rsl_w, sensor_rsl_h);
	rain_mask_img = rain_mask_img(roi); 
	return rain_mask_img; 
}

cv::Mat RainImg::addRain()
{
	cv::Mat rain_add_img;
	cv::Mat rain_mask = generateRainMask();
	int img_w = clear_img.cols;
	int img_h = clear_img.rows;
	// for image size smaller than the sensor relosution size
	// downsize the rain mask image to the clear image size 
	if (img_w <= rain_mask.cols && img_h <= rain_mask.rows)
	{
		cv::Size down_size = cv::Size(img_w, img_h);
		cv::Mat rain_streak_img;
		cv::resize(rain_mask, rain_streak_img, down_size);

		std::vector<cv::Mat> chns;
		cv::split(clear_img, chns);
		// take the max value of three channels to calculate the mean
		cv::Mat max_chn = cv::max(chns[0], chns[1]);
		max_chn = cv::max(max_chn, chns[2]);
		cv::Scalar means = cv::mean(max_chn);
		double mean = means.val[0];
		cv::Mat I = cv::Mat::ones(clear_img.size(), CV_64F); 
		for (int i = 0; i < 3; ++i)
		{
			cv::multiply((I - chns[i]), rain_streak_img, rain_streak_img, 1); 
			chns[i] = chns[i].mul(0.7) + rain_streak_img.mul(0.5);
		}
		cv::merge(chns, rain_add_img);
		return rain_add_img;
	}
	else
	{
		std::cout << "The inut image is out of range of most current camera sensor size, please set the new sensor size!" << std::endl;
		return clear_img;
	}
}
