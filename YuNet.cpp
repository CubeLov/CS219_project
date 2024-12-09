#include <opencv2/opencv.hpp>
#include <memory>
#include "YuNet.h"
#include "BlurMode.h"
#include "MaskMode.h"
#include "PixelMode.h"
#include "NormalMode.h"


YuNet::YuNet(const string& model_path,
	const Size& input_size,
	float conf_threshold,
	float nms_threshold,
	int top_k,
	int backend_id,
	int target_id)
	: model_path_(model_path), input_size_(input_size),
	conf_threshold_(conf_threshold), nms_threshold_(nms_threshold),
	top_k_(top_k), backend_id_(backend_id), target_id_(target_id)
{
	model = FaceDetectorYN::create(model_path_, "", input_size_, conf_threshold_, nms_threshold_, top_k_, backend_id_, target_id_);
}

void YuNet::setInputSize(const Size& input_size)
{
	input_size_ = input_size;
	model->setInputSize(input_size_);
}

Mat YuNet::infer(const Mat image)
{
	Mat res;
	model->detect(image, res);
	return res;
}

void YuNet::visualize(const Mat& faces, vector<shared_ptr<Mode>>& modes, Mat& output_image, float fps)
{
	static Scalar box_color{ 0, 255, 0 };
	static vector<Scalar> landmark_color{
		Scalar(255,   0,   0), // right eye
		Scalar(0,   0, 255), // left eye
		Scalar(0, 255,   0), // nose tip
		Scalar(255,   0, 255), // right mouth corner
		Scalar(0, 255, 255)  // left mouth corner
	};
	static Scalar text_color{ 0, 255, 0 };

	if (fps >= 0)
	{
		putText(output_image, format("FPS: %.2f", fps), Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, text_color, 2);
	}

	for (int i = 0; i < faces.rows; ++i)
	{
		// Draw bounding boxes
		int x1 = static_cast<int>(faces.at<float>(i, 0));
		int y1 = static_cast<int>(faces.at<float>(i, 1));
		int w = static_cast<int>(faces.at<float>(i, 2));
		int h = static_cast<int>(faces.at<float>(i, 3));
		rectangle(output_image, Rect(x1, y1, w, h), box_color, 2);

		// Confidence as text
		float conf = faces.at<float>(i, 14);

		modes[i]->handle();

		// Draw landmarks
		if (modes[i]->getType() == Mode::Type::Normal) {
			putText(output_image, format("%.4f", conf), Point(x1, y1 + 12), FONT_HERSHEY_DUPLEX, 0.5, text_color);
			for (int j = 0; j < landmark_color.size(); ++j)
			{
				int x = static_cast<int>(faces.at<float>(i, 2 * j + 4)), y = static_cast<int>(faces.at<float>(i, 2 * j + 5));
				circle(output_image, Point(x, y), 2, landmark_color[j], 2);
			}
		}
	}
}

void YuNet::createMode(const Mat& faces, vector<shared_ptr<Mode>>& modes, Mat &output_image, Mode::Type type, Mat mask, int val)
{
	for (int i = 0;i < faces.rows;i++) {
		// Draw bounding boxes
		int x1 = static_cast<int>(faces.at<float>(i, 0));
		int y1 = static_cast<int>(faces.at<float>(i, 1));
		int w = static_cast<int>(faces.at<float>(i, 2));
		int h = static_cast<int>(faces.at<float>(i, 3));

		Mat face = output_image(cv::Rect(x1+4, y1+4, w-8, h-8));
		shared_ptr<Mode> mode;
		switch (type) {
		case Mode::Type::Blur:
			mode = make_shared<BlurMode>(face, val);
			break;
		case Mode::Type::Pixel:
			mode = make_shared<PixelMode>(face, val);
			break;
		case Mode::Type::Mask:
			mode = make_shared<MaskMode>(face, mask);
			break;
		default:
			mode = make_shared<NormalMode>();
			break;
		}
		modes.push_back(mode);
	}
}
