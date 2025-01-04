// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <opencv2/opencv.hpp>
#include <iostream>
#include "PixelMode.h"

using namespace cv;
using namespace std;

void PixelMode::handle() {
	if (img_.empty()) {
		cout << "Error: ÎÞ·¨¶ÁÈ¡Í¼Ïñ" << '\n';
		return;
	}
	if (val_ < 1) {
		cout << "Error: ÏñËØ»­²ÎÊý´íÎó" << '\n';
		return;
	}

	Mat imgSmall;
	int h = img_.rows;
	int w = img_.cols;
	int hNew = h / val_;
	int wNew = w / val_;

	resize(img_, imgSmall, Size(wNew, hNew), INTER_LINEAR);
	resize(imgSmall, img_, Size(w, h), INTER_NEAREST);
}
