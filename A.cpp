#include <opencv2/opencv.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "YuNet.h"
#include "Mode.h"
#include "MaskMode.h"
#include "PixelMode.h"
#include "BlurMode.h"
#include "NormalMode.h"

using namespace cv;
using namespace std;

const std::map<std::string, int> str2backend{
	{"opencv", cv::dnn::DNN_BACKEND_OPENCV}, {"cuda", cv::dnn::DNN_BACKEND_CUDA},
	{"timvx",  cv::dnn::DNN_BACKEND_TIMVX},  {"cann", cv::dnn::DNN_BACKEND_CANN}
};
const std::map<std::string, int> str2target{
	{"cpu", cv::dnn::DNN_TARGET_CPU}, {"cuda", cv::dnn::DNN_TARGET_CUDA},
	{"npu", cv::dnn::DNN_TARGET_NPU}, {"cuda_fp16", cv::dnn::DNN_TARGET_CUDA_FP16}
};


int main() {
	string model_path = "face_detection_yunet_2023mar.onnx";
	const int backend_id = str2backend.at("opencv");
	const int target_id = str2target.at("cpu");

	YuNet model(model_path, cv::Size(320, 320), 0.9, 0.3, 5000, backend_id, target_id);

	int device_id = 0;
	auto cap = cv::VideoCapture(device_id);
	int w = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	int h = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	model.setInputSize(cv::Size(w, h));

	auto tick_meter = cv::TickMeter();
	cv::Mat frame;
	Mat mask = imread("D:\\tmp\\OIP.jpg");
	while (cv::waitKey(1) < 0)
	{
		try {
			bool has_frame = cap.read(frame);
			if (!has_frame)
			{
				std::cout << "No frames grabbed! Exiting ...\n";
				break;
			}

			// Inference
			tick_meter.start();
			cv::Mat faces = model.infer(frame);
			tick_meter.stop();

			Mat res_image = frame.clone();
			vector<shared_ptr<Mode>> modes;
			// Draw results on the input image
			YuNet::createMode(faces, modes, res_image, Mode::Type::Mask, mask, 25);
			YuNet::visualize(faces, modes, res_image, (float)tick_meter.getFPS());
			// Visualize in a new window
			cv::imshow("YuNet Demo", res_image);

			tick_meter.reset();
			waitKey(1);
		}
		catch (const std::exception& e) {
			//std::cout << "Exception caught: " << e.what() << std::endl;
		}
		catch (...) {
			//std::cout << "Unknown exception caught!" << std::endl;
		}
	
	}
	return 0;
}