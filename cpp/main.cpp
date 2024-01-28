#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>


using namespace cv;

//Vec3 is BGR, not RGB

float getSimilarity(Vec3b color1, Vec3b color2){
	return
		0.11 * std::pow(color1[0] - color2[0], 2) +
		0.59 * std::pow(color1[1] - color2[1], 2) +
		0.3 * std::pow(color1[2] - color2[2], 2);
}

Vec3b hexToRGB(char* hex){
	int r, g, b;

	sscanf(hex, "%02x%02x%02x", &r, &g, &b);

	return Vec3b(b,g,r);
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
			im.at<Vec3b>(Point(y,x)) = color;
		}
	}

	imwrite("bar.png", im);

	return 0;
}
