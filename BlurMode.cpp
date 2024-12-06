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
