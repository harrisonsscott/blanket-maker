#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <opencv2/opencv.hpp>
#include "nlohmann/json.hpp"

using namespace cv;

class Color {
	public:
		int r;
		int g;
		int b;
};

float getSimilarity(Color color1, Color color2){
	return
		0.3 * std::pow(color1.r - color2.r, 2) +
		0.59 * std::pow(color1.g - color2.g, 2) +
		0.11 * std::pow(color1.b - color2.b, 2);
}

int* hexToRGB(std::string hex){
	hex.erase(std::remove(hex.begin(), hex.end(), '#'), hex.end());
	return new int[2];
}

int main(int argc, char *argv[]){
	if (argc < 4){
		std::cout << "main.py [image file] [output size x] [output size y]" << std::endl;
		return -1;
	}

	int width = std::stoi(argv[2]);
	int height = std::stoi(argv[3]);

	Mat im = imread(argv[1]);

	resize(im, im, Size(width, height));

	if (im.empty()){
		std::cout << "Can't find image!" << std::endl;
		return -1;
	}

	for (int x = 0; x < im.rows; x++){
		for (int y = 0; y < im.cols; y++ ){
			Vec3b & color = im.at<Vec3b>(x,y);
			std::cout << color << std::endl;

			im.at<Vec3b>(Point(y,x)) = color;
		}
	}

	imwrite("bar.png", im);

	return 0;
}
