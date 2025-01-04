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

int main(int argc, char** argv) {
	CommandLineParser parser(argc, argv,
		"{help  h           |                                    | Print this message}"
		"{model m           | face_detection_yunet_2023mar.onnx  | Set path to the model}"
		"{mode  a           | blur                               | Set start mode}"
		"{blur_size         | 1                                  | Set start blur level}"
		"{pixel_size        | 1                                  | Set start pixel level}"
		"{mask_image        | 1.jpg                              | Set default image path}"
		"{device            | 0                                  | Set device id}"
	);
	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}

	string model_path = parser.get<string>("model");
	string default_mode = parser.get<string>("mode");
	string default_mask_path = parser.get<string>("mask_image");
	int default_blur_size = parser.get<int>("blur_size");
	int default_pixel_size = parser.get<int>("pixel_size");
	const int backend_id = dnn::DNN_BACKEND_OPENCV;
	const int target_id = dnn::DNN_TARGET_CPU;

	YuNet model(model_path, cv::Size(320, 320), 0.9, 0.3, 5000, backend_id, target_id);

	int device_id = parser.get<int>("device");
	auto cap = cv::VideoCapture(device_id);
	int w = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	int h = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	model.setInputSize(cv::Size(w, h));

	auto tick_meter = cv::TickMeter();
	cv::Mat frame;
	Mode::Type cur_mode = Mode::to_mode(default_mode);
	Mat mask = imread(cur_mode == Mode::Mask ? default_mask_path : "1.jpg");

	vector<shared_ptr<Mode>> modes;


	int val = 0, ind_blur = 0, ind_pixel = 0;
	switch (cur_mode) {
	case Mode::Type::Blur:
		ind_blur = default_blur_size - 1;
		val = levBlur[ind_blur];
		break;
	case Mode::Type::Pixel:
		ind_pixel = default_pixel_size - 1;
		val = levPix[ind_pixel];
		break;
	}

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
		int ind = (cur_mode == Mode::Type::Blur ? ind_blur : ind_pixel);
		YuNet::visualize(faces, modes, res_image, cur_mode, (float)tick_meter.getFPS(), ind);
		// Visualize in a new window
		cv::imshow("Real-Time Privacy Protection Tool", res_image);

		tick_meter.reset();
		char key = static_cast<char>(waitKey(5));
		switch (key) {
		case '1': // Key 1
			cur_mode = Mode::Type::Blur;
			val = levBlur[ind_blur];
			break;
		case '2': // Key 2
			cur_mode = Mode::Type::Pixel;
			val = levPix[ind_pixel];
			break;
		case '3': // Key 3
			cur_mode = Mode::Type::Mask;
			break;
		case '0': // Key 0
			cur_mode = Mode::Type::Normal;
			break;
		case ']': // Key ]
			if (cur_mode == Mode::Type::Blur) {
				ind_blur = (ind_blur + 1) % 6;
				val = levBlur[ind_blur];
			}
			else if (cur_mode == Mode::Type::Pixel) {
				ind_pixel = (ind_pixel + 1) % 6;
				val = levPix[ind_pixel];
			}
			break;
		case '[': // Key [
			if (cur_mode == Mode::Type::Blur) {
				ind_blur = (ind_blur + 5) % 6;
				val = levBlur[ind_blur];
			}
			else if (cur_mode == Mode::Type::Pixel) {
				ind_pixel = (ind_pixel + 5) % 6;
				val = levPix[ind_pixel];
			}
			break;
		case 'u':
			string path = openFileDialog();
			mask = imread(path);
			break;
		}
	}

}
