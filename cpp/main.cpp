#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <sys/stat.h>
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

std::string RGBToHex(Vec3b color){
	int n;
   char r[4];
   char g[4];
   char b[4];
   sprintf(b, "%X", color[0]);
   sprintf(g, "%X", color[1]);
   sprintf(r, "%X", color[2]);
   return std::string("#") + r + g + b;
}

int main(int argc, char *argv[]){
	if (argc < 4){
		std::cout << "./blanketMaker [image file] [output size x] [output size y] <--output outputFile> <--palette paletteFile> <--upscale true> <--textfile filename>" << std::endl;
		return -1;
	}

	int width = std::stoi(argv[2]);
	int height = std::stoi(argv[3]);
	bool upscaleImage = false;
	bool autoPalette = false;
	int autoPaletteAmount;

	std::string paletteFile = "palette.json";
	std::string outputImage = "bar.png";
	std::string textFile = "bar.txt";

	int iarg = 4;
	while (iarg < argc ) {
		// Read current argument
		std::string argvi = std::string(argv[iarg]);

		if (iarg + 1 < argc && argvi == "--palette") {
			iarg++;
			argvi = argv[iarg];

			if (argvi == "auto"){
				autoPalette = true;
				iarg++;
				autoPaletteAmount = std::stoi(argv[iarg]);
			} else {
				paletteFile = argv[iarg];
			}
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

	if (!autoPalette){
		// check if files exist
		struct stat buffer;
		bool filesExist = true;

		for (const auto file: {paletteFile}){
			if (stat(file.c_str(), &buffer) != 0){
				std::cout << "file " << file << " doesn't exist!" << std::endl;
				filesExist = false;
			}
		}

		if (!filesExist){
			return -1;
		}
	}

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

	std::string outputText = "";

	if (!autoPalette){
		std::ifstream file(paletteFile);
		json data = json::parse(file);

		for (int i = 0; i < data["colors"].size(); i++)
		{
			outputText += data["colors"][i].get<std::string>() + ": " + std::to_string(i) + "\n";
			palette.push_back(hexToRGB(data["colors"][i].get<std::string>().c_str()));
		}

	} else {
		palette.push_back(im.at<Vec3b>(0,0));
		for (int x = 0; x < im.rows; x++){
			for (int y = 0; y < im.cols; y++ ){
				Vec3b & color = im.at<Vec3b>(x,y);
				int similarity = 10000000;
				for (int c = 0; c < palette.size(); c++){
					if (getSimilarity(color, palette[c]) < similarity){
						similarity = getSimilarity(color, palette[c]);
					}
				}

				if (similarity > 800){
					palette.push_back(color);
				}
			}
		}

		for (int i = 0; i < palette.size(); i++){
			outputText += RGBToHex(palette[i]) + ": " + std::to_string(i) + "\n";
		}
	}

	// loop over all pixels
	for (int x = 0; x < im.rows; x++){
		outputText += "\nRow #" + std::to_string(x) + ": ";
		for (int y = 0; y < im.cols; y++ ){
			int similarity = 10000000;
			Vec3b & color = im.at<Vec3b>(x,y);
			Vec3b selectedColor;
			int v = 0;
			int selectedColorIndex = 0;
			for (auto i: palette){
				// select a color in the palette that's the closest to the current pixel
				if (getSimilarity(color, i) < similarity){
					similarity = getSimilarity(color, i);
					selectedColor = i;
					v += 1;
					selectedColorIndex = v;
				}
			}
			outputText += std::to_string(selectedColorIndex) + " ";
			// set pixel color
			im.at<Vec3b>(Point(y,x)) = selectedColor;
		}
	}

	// write to text file
	std::ofstream outputFile(textFile);

    if (outputFile.is_open()) {
        outputFile << outputText;
        outputFile.close();
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
