// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <opencv2/opencv.hpp>
#include <iostream>
#include "MaskMode.h"

using namespace cv;
using namespace std;

void MaskMode::handle() {
	if (img_.empty() || mask_.empty()) {
		cerr << "Error: ÎÞ·¨¶ÁÈ¡Í¼Ïñ" << '\n';
		return;
	}
	Mat resizedMask;
	int h = img_.rows;
	int w = img_.cols;
	resize(mask_, resizedMask, Size(w, h));

	resizedMask.copyTo(img_);
}
