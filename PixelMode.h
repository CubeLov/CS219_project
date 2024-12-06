#pragma once

#include <opencv2/opencv.hpp>
#include "mode.h"

using namespace cv;

void makePixel(Mat img, int val);

class PixelMode : public Mode {
public:
	PixelMode() : Mode(Pixel) {}
	PixelMode(Mat img, int val) : Mode(Pixel), img_(img), val_(val) {}
	~PixelMode() = default;

	void handle() override;
	void setImg(Mat img) { img_ = img; }
	void setVal(int val) { val_ = val; }

private:
	Mat img_;
	int val_;
};
