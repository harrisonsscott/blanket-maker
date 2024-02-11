#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "json.hpp"


using namespace cv;
using json = nlohmann::json;

//Vec3b is BGR, not RGB

float getSimilarity(Vec3b color1, Vec3b color2){
	return
		0.11 * std::pow(color1[0] - color2[0], 2) +
		0.59 * std::pow(color1[1] - color2[1], 2) +
		0.3 * std::pow(color1[2] - color2[2], 2);
}

int hexToInt(std::string hex) {
	return stoi(hex, nullptr, 16);

}

Vec3b hexToRGB(const std::string& hex){
	int red, green, blue;
	const char *hexstring = hex.c_str();
	char rstring[3];
	char gstring[3];
	char bstring[3];
	strncpy(rstring,&hexstring[1],2);
	strncpy(gstring,&hexstring[3],2);
	strncpy(bstring,&hexstring[5],2);

	return Vec3b(hexToInt(bstring), hexToInt(gstring), hexToInt(rstring));
}

int main(int argc, char *argv[]){
	if (argc < 4){
		std::cout << "./blanketMaker [image file] [output size x] [output size y] <--output outputFile> <--palette paletteFile> <--upscale true> <--textfile filename>" << std::endl;
		return -1;
	}

	int width = std::stoi(argv[2]);
	int height = std::stoi(argv[3]);
	bool upscaleImage = false;
	std::string paletteFile = "palette.json";
	std::string outputImage = "bar.png";
	std::string textFile = "bar.txt";
	
	int iarg = 4;
	while (iarg < argc ) {
	  // Read current argument
	  std::string argvi = std::string(argv[iarg]);
	  
	  if (iarg + 1 < argc && argvi == "--palette") {
	    iarg++;
	    paletteFile = argv[iarg];
	  }
	  if (iarg + 1 < argc && argvi == "--upscale") {
	    iarg++;
	    argvi = argv[iarg];
	    for (char &c : argvi) {
	      c = std::tolower(c);
	    }
	    if (argvi == "true"){
	      upscaleImage = true;
	    }
	  }	  
	  if (iarg + 1 < argc && argvi == "--textfile") {
	    iarg++;
	    textFile = argv[iarg];
	  }
	  if (iarg + 1 < argc && argvi == "--output") {
	    iarg++;
	    outputImage = argv[iarg];
	  }	  

	  // Move on to next argument
	  iarg++;
	}
	Mat im = imread(argv[1]);

	if (im.empty()){
		std::cout << "Can't find image!" << std::endl;
		return -1;
	}

	if (width <= 0 || height <= 0){
		std::cout << "Invalid dimensions!" << std::endl;
		return -1;
	}

	std::vector<Vec3b> palette;

	resize(im, im, Size(width, height));

	std::ifstream file(paletteFile);
	json data = json::parse(file);

	for (const auto& color: data["colors"]){
		palette.push_back(hexToRGB(color.get<std::string>().c_str()));
	}

	for (int x = 0; x < im.rows; x++){
		for (int y = 0; y < im.cols; y++ ){
			int similarity = 10000000;
			Vec3b & color = im.at<Vec3b>(x,y);
			Vec3b selectedColor;
			for (auto i: palette){
				if (getSimilarity(color, i) < similarity){
					similarity = getSimilarity(color, i);
					selectedColor = i;
				}
			}
			im.at<Vec3b>(Point(y,x)) = selectedColor;
		}
	}

	if (upscaleImage){
		int aspect = height/width;
		int sizeX = std::max(width * 16, 1024);
		int sizeY = sizeX * aspect;

		resize(im, im, Size(sizeX, sizeY), 0, 0, INTER_NEAREST);
	}

	imwrite(outputImage, im);

	return 0;
}
