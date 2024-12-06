#include <opencv2/opencv.hpp>
#include <iostream>
#include "MaskMode.h"

using namespace cv;
using namespace std;

void MaskMode::handle() {
	if (img_.empty() || mask_.empty()) {
		cout << "Error: �޷���ȡͼ��" << '\n';
		return;
	}
	Mat resizedMask;
	int h = img_.rows;
	int w = img_.cols;
	resize(mask_, resizedMask, Size(w, h));

	resizedMask.copyTo(img_);
}
