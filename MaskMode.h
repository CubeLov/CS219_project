#pragma once

#include <opencv2/opencv.hpp>
#include "Mode.h"

using namespace cv;
using namespace std;

class MaskMode : public Mode {
public:
	MaskMode() : Mode(Mask) {}
	MaskMode(Mat img, Mat mask) : Mode(Mask), img_(img), mask_(mask) {}
	~MaskMode() = default;

	void handle() override;
	void setImg(Mat img) { img_ = img; }
	void setMask(Mat mask) { mask_ = mask; }

private:
	Mat img_;
	Mat mask_;
};
