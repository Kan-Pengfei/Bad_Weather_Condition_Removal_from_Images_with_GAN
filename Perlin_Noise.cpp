#include "Perlin_Noise.h"
#include <random>
#include <ctime>

double PerlinNoise::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
	return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

PerlinNoise::PerlinNoise() {
	int permutation[] = { 151, 160, 137, 91, 90, 15, 131, 13, 201,
		95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37,
		240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62,
		94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56,
		87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139,
		48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
		230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
		63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200,
		196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3,
		64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255,
		82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
		223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153,
		101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79,
		113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242,
		193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249,
		14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204,
		176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222,
		114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 };

	for (int i = 0; i < 256; i++) {
		p[256 + i] = p[i] = permutation[i];
	}
}

double PerlinNoise::noise(const double &src_x, const double &src_y, const double &src_z) {
	int X = (int)floor(src_x) & 255;
	int Y = (int)floor(src_y) & 255;
	int Z = (int)floor(src_z) & 255;

	double x = src_x - floor(src_x);
	double y = src_y - floor(src_y);
	double z = src_z - floor(src_z);

	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	double r = lerp(
		w,
		lerp(v,
			lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)),
			lerp(u, grad(p[AB], x, y - 1, z),
				grad(p[BB], x - 1, y - 1, z))),
		lerp(v,
			lerp(u, grad(p[AA + 1], x, y, z - 1),
				grad(p[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
				grad(p[BB + 1], x - 1, y - 1, z - 1))));

	return r;
}	

// create one layer of perlin noise 
cv::Mat PerlinNoise::createPNoiseImg(const cv::Size &img_size, const double &scale, const double &noise_cff)
{
	cv::Mat img;
	img.create(img_size, CV_64F);
	//double noise_cff;
	//noise_cff = (double)rand() / RAND_MAX; 

	for (int y = 0; y < img_size.height; ++y) {
		for (int x = 0; x < img_size.width; ++x) {
			double p = noise(x  * scale, y * scale, noise_cff); // the range is (-1.0 ~ 1.0)
			p = (p + 1.0) / 2.0; // change the range to (0.0 ~ 1.0)			
			img.at<double>(cv::Point(x, y)) = p;
		}
	}
	return img;
}

// create multi-layer perlin noise and combine them together to get better visual effect 
cv::Mat PerlinNoise::addMultiplePNoise(cv::Mat &input_img, double noise_ratio)
{
	int layer_num = rand() % 2 + 5; // the layer of noise 
	double scale = (double)rand() / RAND_MAX / 400 + 0.0025;
	double noise_cff = (double)rand() / RAND_MAX;
	cv::Mat noise_img = createPNoiseImg(input_img.size(), scale, noise_cff);
	double img_num = 2; // initial the noise image number as 2
	double alpha, beta; 
	for (int i = 1; i < layer_num; ++i)
	{
		scale *= 0.5;
		noise_cff = (double)rand() / RAND_MAX;
		// get the next layer of noise 
		cv::Mat scr = createPNoiseImg(noise_img.size(), scale, noise_cff);
		alpha = 1.0 / img_num;
		beta = 1 - alpha;
		cv::addWeighted(noise_img, beta, scr, alpha, 0, noise_img);
		img_num++;
	}
	cv::Mat output_img; 
	int chan_num = input_img.channels(); 
	if (chan_num == 3)
	{
		std::vector<cv::Mat> channels(3);
		cv::split(input_img, channels);

		for (int c = 0; c < 3; ++c)
		{
			channels[c] = channels[c].mul(1 - noise_ratio) + noise_img.mul(noise_ratio);
		}
		cv::merge(channels, output_img);
	}
	else if (chan_num == 1)
	{
		output_img = input_img.mul(1 - noise_ratio) + noise_img.mul(noise_ratio);
	}

	return output_img;
}