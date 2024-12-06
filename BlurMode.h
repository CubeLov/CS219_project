#pragma once

#include<opencv2/opencv.hpp>
#include "Mode.h"

using namespace cv;

void makeBlur(Mat img, int val);

class BlurMode : public Mode {
public:
	BlurMode() : Mode(Blur) {}
	BlurMode(Mat img, int val) :Mode(Blur), img_(img), val_(val) {}
	~BlurMode() = default;

	void handle() override;
	void setImg(Mat img) { img_ = img; }
	void setVal(int val) { val_ = val; }

private:
	Mat img_;
	int val_;
};
