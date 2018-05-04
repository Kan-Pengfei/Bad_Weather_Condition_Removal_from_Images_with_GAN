#include "Fog_Image.h"

FogImg::FogImg(cv::Mat input_img)
{
	// Make sure the input image is double type 
	if (input_img.type() != CV_64FC3)
		input_img.convertTo(clear_img, CV_64FC3, 1.0 / 255); 
	else
		clear_img = input_img;
	// compute the eps for guided filter 
	cv::Scalar img_mean, img_std;
	cv::meanStdDev(clear_img, img_mean, img_std);
	for (int i = 0; i < 3; ++i)
		guided_filter_eps += std::pow(img_std.val[i], 2);
	guided_filter_eps /= 3;
}

FogImg::FogImg(cv::Mat input_img, double input_noise_ratio) : noise_ratio(input_noise_ratio)
{
	// Make sure the input image is double type 
	if (input_img.type() != CV_64FC3)
		input_img.convertTo(clear_img, CV_64FC3, 1.0 / 255);
	else
		clear_img = input_img;

	// compute the eps for guided filter 
	cv::Scalar img_mean, img_std;
	cv::meanStdDev(clear_img, img_mean, img_std);
	for (int i = 0; i < 3; ++i)
		guided_filter_eps += std::pow(img_std.val[i], 2);
	guided_filter_eps /= 3;
}

double FogImg::getNoiseRatio()
{
	return noise_ratio; 
}
void FogImg::setNoiseRatio(double input_ratio)
{
	noise_ratio = input_ratio; 
}
double FogImg::getSkyLight()
{
	return sky_light; 
}
void FogImg::setSkyLight(double input_sky_light)
{
	sky_light = input_sky_light; 
}
cv::Mat FogImg::getClearImg()
{
	return clear_img; 
}

// estimate the dark chanel of the image 
// which is the minimum value of a patch of image in all channels
cv::Mat FogImg::estDarkVeil()
{
	// declare an one_channel_image, if the clear image has multiple channels
	// get the minimum intensity of corrosponding pixels among all channels 
	// and use that as the new value for the pixel in one channel image 
	cv::Mat one_ch_img = cv::Mat::ones(clear_img.size(), CV_64F); 

	// modify 3 channels into one channel by choosing the smallest intensity among all channels 
	std::vector<cv::Mat> channels(3);
	cv::split(clear_img, channels);
	cv::min(channels[0], channels[1], one_ch_img);
	cv::min(one_ch_img, channels[2], one_ch_img);

	// preprocess image to get the dark channel 
	// extend the bountry of the clear image with 1
	cv::Mat pre_img = cv::Mat::ones(clear_img.rows + patch_size, clear_img.cols + patch_size, CV_64F);
	cv::Mat embed_area(pre_img, cv::Rect(0, 0, one_ch_img.cols, one_ch_img.rows));
	one_ch_img.copyTo(embed_area);

	dark_veil = cv::Mat::zeros(clear_img.size(), CV_64F);
	for (int i = 0; i < dark_veil.rows; ++i)
	{
		for (int j = 0; j < dark_veil.cols; ++j)
		{
			cv::Rect roi = cv::Rect(j, i, patch_size, patch_size); // watch out the rect uses x, y 
			cv::Mat roi_img = pre_img(roi);
			double min_inten;
			cv::minMaxIdx(roi_img, &min_inten);
			dark_veil.at<double>(i, j) = min_inten; // at uses row and col 
		}
	}
	return dark_veil; 
}

cv::Mat FogImg::estDarkVeilFast()
{
	int ch_num = clear_img.channels();
	// modify 3 channels into one channel by choosing the smallest intensity among all channels 
	std::vector<cv::Mat> channels(3);
	cv::split(clear_img, channels);
	cv::min(channels[0], channels[1], dark_veil);
	cv::min(dark_veil, channels[2], dark_veil);
	return dark_veil; 
}

cv::Mat FogImg::calcTransM()
{
	dark_veil = estDarkVeil();
	cv::Mat I = cv::Mat::ones(dark_veil.size(), dark_veil.type());
	cv::Mat estm_trans_M = I - dark_veil; 
	// apply guided filter here to do the edge preserving smoothing 
	// and use the filtered image as the estimation of the t 
	trans_mat = guidedFilter(clear_img, estm_trans_M, patch_size, guided_filter_eps, -1); 
	return trans_mat; 
}

// J = f*t + A(1 - t)
cv::Mat FogImg::calcRdScene()
{
	trans_mat = calcTransM(); 
	cv::Mat I = cv::Mat::ones(clear_img.size(), CV_64F); 

	std::vector<cv::Mat> input_chns(3), output_chns(3); // we need to declare the number of elements in the vector 
	// otherwise the compiler will report the vector subscript out of range error 
	cv::split(clear_img, input_chns); 
	for (int c = 0; c < 3; ++c)
	{
		cv::Mat A1, A2;
		cv::multiply(input_chns[c], trans_mat, A1, 1, -1);
		A2 = I - trans_mat;
		A2 = A2.mul(sky_light);
		output_chns[c] = A1 + A2;
	}
	cv::merge(output_chns, radiance_scene); 
	return radiance_scene; 
}

cv::Mat FogImg::getFogImg()
{
	radiance_scene = calcRdScene(); 
	PerlinNoise noise; 
	noise_ratio += (double)rand() / RAND_MAX / 10; 
	cv::Mat temp_fog_img = noise.addMultiplePNoise(radiance_scene, noise_ratio); 
	fog_img = temp_fog_img; // MyFuncs::MyImgFuncs::homoFilter(temp_fog_img);
	return fog_img; 
}