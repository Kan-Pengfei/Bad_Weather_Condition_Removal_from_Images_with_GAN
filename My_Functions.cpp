#include "My_Functions.h"

namespace MyFuncs
{
	// convert image types, only works with 1 and 3 channel RBG images 
	cv::Mat MyImgFuncs::myConvertTo(cv::Mat input_image, std::string dst_type)
	{
		int image_type = input_image.type();
		cv::Mat output_image;
		if ((image_type == CV_8U) || (image_type == CV_8UC3))
		{
			if (image_type == CV_8U)
			{
				if ((dst_type == "double") || (dst_type == "Double") || (dst_type == "DOUBLE")) {
					input_image.convertTo(output_image, CV_64F, 1.0 / 255);
				}
				else if ((dst_type == "float") || (dst_type == "Float") || (dst_type == "FLOAT")) {
					input_image.convertTo(output_image, CV_32F, 1.0 / 255);
				}
				else if ((dst_type == "int") || (dst_type == "Int") || (dst_type == "INT")) {
					input_image.convertTo(output_image, CV_32S, 65535 / 255);
				}
				else if ((dst_type == "short") || (dst_type == "Short") || (dst_type == "SHORT")) {
					input_image.convertTo(output_image, CV_16S, 32767 / 255);
				}
				else if ((dst_type == "ushort") || (dst_type == "Ushort") || (dst_type == "USHORT")) {
					input_image.convertTo(output_image, CV_16U, 65535 / 255);
				}
				else if ((dst_type == "char") || (dst_type == "Char") || (dst_type == "CHAR")) {
					input_image.convertTo(output_image, CV_8S);
				}
				else if ((dst_type == "uchar") || (dst_type == "Uchar") || (dst_type == "UCHAR")) {
					input_image.convertTo(output_image, CV_8U);
				}
				else {
					std::cout << "The destination image type is invalid!" << std::endl;
				}
			}
			else
			{
				if ((dst_type == "double") || (dst_type == "Double") || (dst_type == "DOUBLE")) {
					input_image.convertTo(output_image, CV_64FC3, 1.0 / 255);
				}
				else if ((dst_type == "float") || (dst_type == "Float") || (dst_type == "FLOAT")) {
					input_image.convertTo(output_image, CV_32FC3, 1.0 / 255);
				}
				else if ((dst_type == "int") || (dst_type == "Int") || (dst_type == "INT")) {
					input_image.convertTo(output_image, CV_32SC3, 65535 / 255);
				}
				else if ((dst_type == "short") || (dst_type == "Short") || (dst_type == "SHORT")) {
					input_image.convertTo(output_image, CV_16SC3, 32767 / 255);
				}
				else if ((dst_type == "ushort") || (dst_type == "Ushort") || (dst_type == "USHORT")) {
					input_image.convertTo(output_image, CV_16UC3, 65535 / 255);
				}
				else if ((dst_type == "char") || (dst_type == "Char") || (dst_type == "CHAR")) {
					input_image.convertTo(output_image, CV_8SC3);
				}
				else if ((dst_type == "uchar") || (dst_type == "Uchar") || (dst_type == "UCHAR")) {
					input_image.convertTo(output_image, CV_8UC3);
				}
				else {
					std::cout << "The destination image type is invalid!" << std::endl;
				}
			}
		}
		else if ((image_type == CV_64FC3) || (image_type == CV_64F))
		{
			// if the input is one channel 
			if (image_type == CV_64F)
			{
				if ((dst_type == "double") || (dst_type == "Double") || (dst_type == "DOUBLE")) {
					output_image = input_image; 
				}
				else if ((dst_type == "float") || (dst_type == "Float") || (dst_type == "FLOAT")) {
					input_image.convertTo(output_image, CV_32F); 
				}
				else if ((dst_type == "int") || (dst_type == "Int") || (dst_type == "INT")) {
					input_image.convertTo(output_image, CV_32S, 65535); 
				}
				else if ((dst_type == "short") || (dst_type == "Short") || (dst_type == "SHORT")) {
					input_image.convertTo(output_image, CV_16S, 32767); 
				}
				else if ((dst_type == "ushort") || (dst_type == "Ushort") || (dst_type == "USHORT")) {
					input_image.convertTo(output_image, CV_16U, 65535); 
				}
				else if ((dst_type == "char") || (dst_type == "Char") || (dst_type == "CHAR")) {
					input_image.convertTo(output_image, CV_8S, 125); 
				}
				else if ((dst_type == "uchar") || (dst_type == "Uchar") || (dst_type == "UCHAR")) {
					input_image.convertTo(output_image, CV_8U, 255); 
				}
				else {
					std::cout << "The destination image type is invalid!" << std::endl; 
				}
			}
			if (image_type == CV_64FC3)
			{
				if ((dst_type == "double") || (dst_type == "Double") || (dst_type == "DOUBLE")) {
					output_image = input_image;
				}
				else if ((dst_type == "float") || (dst_type == "Float") || (dst_type == "FLOAT")) {
					input_image.convertTo(output_image, CV_32FC3);
				}
				else if ((dst_type == "int") || (dst_type == "Int") || (dst_type == "INT")) {
					input_image.convertTo(output_image, CV_32SC3, 65535);
				}
				else if ((dst_type == "short") || (dst_type == "Short") || (dst_type == "SHORT")) {
					input_image.convertTo(output_image, CV_16SC3, 32767);
				}
				else if ((dst_type == "ushort") || (dst_type == "Ushort") || (dst_type == "USHORT")) {
					input_image.convertTo(output_image, CV_16UC3, 65535);
				}
				else if ((dst_type == "char") || (dst_type == "Char") || (dst_type == "CHAR")) {
					input_image.convertTo(output_image, CV_8SC3, 125);
				}
				else if ((dst_type == "uchar") || (dst_type == "Uchar") || (dst_type == "UCHAR")) {
					input_image.convertTo(output_image, CV_8UC3, 255);
				}
				else {
					std::cout << "The destination image type is invalid!" << std::endl;
				}
			}
		}
		else {
			std::cout << std::endl << "Error: The input image must have uchar or double type!" << std::endl;
		}
		return output_image;
	}
	
	// This is the homomorphic filter that balances the brightness of the image 
	cv::Mat MyImgFuncs::homoFilter(cv::Mat input_image)
	{
		if (input_image.type() != CV_8U || input_image.type() != CV_8UC3)
		{
			input_image = myConvertTo(input_image, "uchar"); 
		}
		double minVal, maxVal;
		float gainMax = 2, gainMin = 1;
		int begSlope = 2 * input_image.cols / 8;
		int endSlope = 3 * input_image.cols / 8;

		cv::Mat image_Yuv, image_f;
		if (input_image.channels() >= 3)
			cv::cvtColor(input_image, image_Yuv, cv::COLOR_BGR2YCrCb);
		else
			input_image.copyTo(image_Yuv);

		// do the computation of log10 
		std::vector<cv::Mat> channels;
		cv::split(image_Yuv, channels);
		for (int i = 0; i < 3; i++)
		{
			cv::minMaxLoc(channels[i], &minVal, &maxVal);
			//std::cout << minVal << "\t" << maxVal << std::endl;
		}
		channels[0].convertTo(image_f, CV_32FC1);
		image_f = image_f + 0.01;  // to remove the 0 intensity 
		cv::log(image_f, image_f);

		cv::Mat tfMf, filter = cv::Mat::zeros(image_f.rows, image_f.cols, CV_32FC1), tfMfFilter, mfHomomorphic;
		cv::dft(image_f, tfMf, cv::DFT_COMPLEX_OUTPUT); // discrete Fourier transform 
		tfMfFilter = tfMf.clone();
		int middleWidth = image_f.cols / 2, middleHeight = image_f.rows / 2;
		//Butterworth equation parameters for homomorphic fltering 
		double a = 0.15;
		double d = 1.5, e = 0.5;
		double n = 1.0;
		for (int i = 0; i <= middleHeight; i++)
		{
			for (int j = 0; j <= middleWidth; j++)
			{
				double r = sqrt(i*i*1.0 / image_f.rows*1.0 / image_f.rows + j*j*1.0 / image_f.cols*1.0 / image_f.cols);
				double fLow = 1. / (1 + pow(r / a, n));
				double fHigh = 1 - fLow;
				double fBoost = d*fHigh + e;
				if (i == 0 && j == 0)
					fBoost = 1;
				filter.at<float>(i, j) = fBoost;
				if (i != 0)
					filter.at<float>(image_f.rows - i, j) = filter.at<float>(i, j);
				if (j != 0)
					filter.at<float>(i, image_f.cols - j) = filter.at<float>(i, j);
				if (j != 0 && i != 0)
					filter.at<float>(image_f.rows - i, image_f.cols - j) = filter.at<float>(i, j);
			}
		}

		for (int i = 0; i < image_f.rows; i++)
		{
			for (int j = 0; j < image_f.cols; j++)
			{
				cv::Vec2f v = tfMf.at<cv::Vec2f>(i, j);
				std::complex<float> z(v[0], v[1]);
				z = z * filter.at<float>(i, j);
				v[0] = z.real(); v[1] = z.imag();
				tfMfFilter.at<cv::Vec2f>(i, j) = v;
			}
		}

		cv::idft(tfMfFilter, mfHomomorphic, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
		cv::minMaxLoc(mfHomomorphic, &minVal, &maxVal);
		cv::exp(mfHomomorphic, mfHomomorphic);
		cv::minMaxLoc(mfHomomorphic, &minVal, &maxVal);
		cv::Mat result_raw, result;
		mfHomomorphic.convertTo(result, CV_8UC1);
		channels[0] = result;
		if (input_image.channels() >= 3)
			merge(channels, result_raw);
		else
			result_raw = result;
		cv::FileStorage fs1("homomorphic.yml", cv::FileStorage::WRITE);
		fs1 << "Image" << mfHomomorphic;
		cv::cvtColor(result_raw, result, cv::COLOR_YCrCb2BGR);
		return result;
	}

	cv::Mat MyImgFuncs::combineParal(cv::Mat left_img, cv::Mat right_img)
	{
		cv::Size img_size = left_img.size(); 
		cv::Mat destination_image(img_size.height, img_size.width * 2, left_img.type());
		cv::Mat left_part(destination_image, cv::Rect(0, 0, img_size.width, img_size.height));
		left_img.copyTo(left_part);
		cv::Mat right_part(destination_image, cv::Rect(img_size.width, 0, img_size.width, img_size.height));
		right_img.copyTo(right_part);
		return destination_image; 
	}

	std::vector<cv::String> MyStrFuncs::concat(std::vector<cv::String> input_file_paths)
	{
		std::vector<cv::String> output_names; 
		for (int i = 0; i < input_file_paths.size(); ++i)
		{
			cv::String file_path = input_file_paths[i]; 
			std::size_t start_index = file_path.find_last_of('/', file_path.size()); 
			output_names.push_back(file_path.substr(start_index + 1, std::string::npos)); 
		}
		return output_names; 
	}
}
