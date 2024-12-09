#pragma once

#include <opencv2/opencv.hpp>
#include "Mode.h"

class NormalMode : public Mode {
public:
	NormalMode() : Mode(Normal){}
	~NormalMode() = default;
	void handle() override {}
};

