#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <nlohmann/json.hpp>


using namespace cv;
using json = nlohmann::json;

//Vec3b is BGR, not RGB

float getSimilarity(Vec3b color1, Vec3b color2){
	return
		0.11 * std::pow(color1[0] - color2[0], 2) +
		0.59 * std::pow(color1[1] - color2[1], 2) +
		0.3 * std::pow(color1[2] - color2[2], 2);
}

Vec3b hexToRGB(const char* hex){
	int r, g, b;

	sscanf(hex, "%02x%02x%02x", &r, &g, &b);
	
	std::cout << r << g << b << std::endl;

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

	if (im.empty()){
		std::cout << "Can't find image!" << std::endl;
		return -1;
	}

	std::vector<Vec3b> palette;

	resize(im, im, Size(width, height));

	std::ifstream file("palette.json");
	json data = json::parse(file);

	for (const auto& color: data["colors"]){
		std::cout << color << std::endl;
		std::cout << hexToRGB(color.get<std::string>().c_str()) << std::endl;
		palette.push_back(hexToRGB(color.get<std::string>().c_str()));
	}

	std::cout << palette[3] << std::endl;

	for (int x = 0; x < im.rows; x++){
		for (int y = 0; y < im.cols; y++ ){
			int similarity = 10000000;
			Vec3b & color = im.at<Vec3b>(x,y);
			std::cout << color << std::endl;
			Vec3b selectedColor;
			for (auto i: palette){
				if (getSimilarity(color, i) < similarity){
					similarity = getSimilarity(color, i);
					selectedColor = i;
				}
			}
			std::cout << selectedColor << std::endl;
			im.at<Vec3b>(Point(y,x)) = selectedColor;
		}
	}

	imwrite("bar.png", im);

	return 0;
}
