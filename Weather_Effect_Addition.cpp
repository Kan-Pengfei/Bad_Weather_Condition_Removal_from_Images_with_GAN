#include "Weather_Effect_Addition.h"

Weather_Effect_Addition::Weather_Effect_Addition() {}
Weather_Effect_Addition::~Weather_Effect_Addition() {}

void Weather_Effect_Addition::add(cv::String input_file_path, cv::String output_file_path, std::vector<cv::String> img_types)
{
	srand(time(NULL));
	std::vector<cv::String> input_img_paths, input_img_names;
	for (int i = 0; i < img_types.size(); ++i)
		cv::glob(input_file_path + "*." + img_types[i], input_img_paths, false);
	input_img_names = MyFuncs::MyStrFuncs::concat(input_img_paths);

	for (int i = 0; i < input_img_paths.size(); ++i)
	{
		cv::Mat input_img = cv::imread(input_img_paths[i]); // it's type is CV_8UC3

		// add the fog 
		FogImg fog_img(input_img);
		cv::Mat fog_added_img = fog_img.getFogImg();
		fog_added_img.convertTo(fog_added_img, CV_8UC3, 255);
		cv::String fog_add_img_path = output_file_path + "fog_" + input_img_names[i];
		cv::imwrite(fog_add_img_path, fog_added_img);
		cv::waitKey(10);

		// add the rain
		RainImg rain_img(input_img); // take the default camera parameters 
		cv::Mat rain_added_img = rain_img.addRain();
		rain_added_img.convertTo(rain_added_img, CV_8UC3, 255);
		//cv::imshow("The rain mask image", rain_added_img); 
		cv::String rain_add_img_path = output_file_path + "rain_" + input_img_names[i];
		cv::imwrite(rain_add_img_path, rain_added_img);
		cv::waitKey(10);

		// add the snow 
		SnowImg snow_img(input_img);
		cv::Mat snow_added_img = snow_img.addSnow();
		snow_added_img.convertTo(snow_added_img, CV_8UC3, 255);
		cv::String snow_added_img_path = output_file_path + "snow_" + input_img_names[i];
		cv::imwrite(snow_added_img_path, snow_added_img);
		cv::waitKey(10);
	}
}

void Weather_Effect_Addition::add4GAN(cv::String input_file_path, cv::String output_file_path, std::vector<cv::String> img_types)
{
	srand(time(NULL));
	std::vector<cv::String> input_img_paths, input_img_names;
	for (int i = 0; i < img_types.size(); ++i)
		cv::glob(input_file_path + "*." + img_types[i], input_img_paths, false);
	input_img_names = MyFuncs::MyStrFuncs::concat(input_img_paths);

	for (int i = 0; i < input_img_paths.size(); ++i)
	{
		cv::Mat input_img = cv::imread(input_img_paths[i]);

		// add the fog 
		FogImg fog_img(input_img);
		cv::Mat fog_added_img = fog_img.getFogImg();
		fog_added_img.convertTo(fog_added_img, CV_8UC3, 255);
		cv::Mat GAN_fog_img = MyFuncs::MyImgFuncs::combineParal(input_img, fog_added_img);  // put two images side by side for the GAN 
		cv::String fog_add_img_path = output_file_path + "fog_" + input_img_names[i];
		cv::imwrite(fog_add_img_path, GAN_fog_img);
		cv::waitKey(10);

		// add the rain
		RainImg rain_img(input_img); // take the default camera parameters 
		cv::Mat rain_added_img = rain_img.addRain();
		rain_added_img.convertTo(rain_added_img, CV_8UC3, 255);
		cv::Mat GAN_rain_img = MyFuncs::MyImgFuncs::combineParal(input_img, rain_added_img); 
		cv::String rain_add_img_path = output_file_path + "rain_" + input_img_names[i];
		cv::imwrite(rain_add_img_path, GAN_rain_img);
		cv::waitKey(10);

		// add the snow 
		SnowImg snow_img(input_img);
		cv::Mat snow_added_img = snow_img.addSnow();
		snow_added_img.convertTo(snow_added_img, CV_8UC3, 255);
		cv::Mat GAN_snow_img = MyFuncs::MyImgFuncs::combineParal(input_img, snow_added_img); 
		cv::String snow_added_img_path = output_file_path + "snow_" + input_img_names[i];
		cv::imwrite(snow_added_img_path, GAN_snow_img);
		cv::waitKey(10);
	}
}