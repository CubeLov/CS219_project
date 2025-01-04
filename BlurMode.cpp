// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <opencv2/opencv.hpp>
#include <iostream>
#include "BlurMode.h"

using namespace cv;
using namespace std;

void BlurMode::handle() {
	if (img_.empty()) {
		cout << "Error: ÎÞ·¨¶ÁÈ¡Í¼Ïñ" << '\n';
		return;
	}
	GaussianBlur(img_, img_, Size(val_, val_), 0, 0);
}
