#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>
#include "Mode.h"

using namespace std;
using namespace cv;

class YuNet
{
public:
	YuNet(const string& model_path,
		const Size& input_size = Size(320, 320),
		float conf_threshold = 0.6f,
		float nms_threshold = 0.3f,
		int top_k = 5000,
		int backend_id = 0,
		int target_id = 0);

	static void visualize(const Mat& faces,
		vector<shared_ptr<Mode>>& modes, Mat& output_image, float fps = -1.f);

	static void createMode(const Mat& faces, vector < shared_ptr<Mode>>& modes, Mat& output_image,
		Mode::Type type, Mat mask, int val = 0);
	/* Overwrite the input size when creating the model. Size format: [Width, Height].
	*/
	void setInputSize(const Size& input_size);

	Mat infer(const Mat image);

private:
	Ptr<FaceDetectorYN> model;

	string model_path_;
	Size input_size_;
	float conf_threshold_;
	float nms_threshold_;
	int top_k_;
	int backend_id_;
	int target_id_;
};
