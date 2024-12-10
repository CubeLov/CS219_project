#include <opencv2/opencv.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <windows.h>
#include "YuNet.h"
#include "Mode.h"
#include "MaskMode.h"
#include "PixelMode.h"
#include "BlurMode.h"
#include "NormalMode.h"

using namespace cv;
using namespace std;

const int levBlur[] = { 11,17,23,29,35,41 };
const int levPix[] = { 6,11,16,21,26,32 };

string openFileDialog() {
	char filePath[MAX_PATH] = { 0 };
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Image Files\0*.jpg;*.png;*.bmp\0All Files\0*.*\0";
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (GetOpenFileName(&ofn)) {
		return std::string(filePath);
	}
	return "";
}

int main() {
	string model_path = "face_detection_yunet_2023mar.onnx";
	const int backend_id = dnn::DNN_BACKEND_OPENCV;
	const int target_id = dnn::DNN_TARGET_CPU;

	YuNet model(model_path, cv::Size(320, 320), 0.9, 0.3, 5000, backend_id, target_id);

	int device_id = 0;
	auto cap = cv::VideoCapture(device_id);
	int w = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	int h = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	model.setInputSize(cv::Size(w, h));

	auto tick_meter = cv::TickMeter();
	cv::Mat frame;
	Mat mask = imread("D:\\tmp\\OIP.jpg");

	Mode::Type cur_mode = Mode::Type::Normal;
	vector<shared_ptr<Mode>> modes;


	int val = 0, ind = 0;
	while (true) {
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
		modes.clear();
		// Draw results on the input image
		YuNet::createMode(faces, modes, res_image, cur_mode, mask, val);
		YuNet::visualize(faces, modes, res_image, cur_mode, (float)tick_meter.getFPS(), ind);
		// Visualize in a new window
		cv::imshow("Real-Time Privacy Protection Tool", res_image);

		tick_meter.reset();
		char key = static_cast<char>(waitKey(5));
		switch (key) {
		case '1': // Key 1
			cur_mode = Mode::Type::Blur;
			val = levBlur[ind];
			break;
		case '2': // Key 2
			cur_mode = Mode::Type::Pixel;
			val = levPix[ind];
			break;
		case '3': // Key 3
			cur_mode = Mode::Type::Mask;
			break;
		case '0': // Key 0
			cur_mode = Mode::Type::Normal;
			break;
		case ']': // Key ]
			if (cur_mode == Mode::Type::Blur) {
				ind = (ind + 1) % 6;
				val = levBlur[ind];
			}
			else if (cur_mode == Mode::Type::Pixel) {
				ind = (ind + 1) % 6;
				val = levPix[ind];
			}
			break;
		case '[': // Key [
			if (cur_mode == Mode::Type::Blur) {
				ind = (ind + 5) % 6;
				val = levBlur[ind];
			}
			else if (cur_mode == Mode::Type::Pixel) {
				ind = (ind + 5) % 6;
				val = levPix[ind];
			}
			break;
		case 'u':
			string path = openFileDialog();
			mask = imread(path);
			break;
		}
	}

}
