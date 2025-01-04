#pragma once

#include <iostream>

using namespace std;

class Mode {
public:
	enum Type {
		Normal,
		Blur,
		Pixel,
		Mask
	};

	Mode(Type type) : type_(type) {}
	~Mode() = default;

	Type getType() const { return type_; }
	virtual void handle() = 0;

	static string to_string(Type type) {
		string res;
		switch (type) {
		case Blur:
			res = "Blur Mode";
			break;
		case Pixel:
			res = "Pixel Mode";
			break;
		case Mask:
			res = "Mask Mode";
			break;
		default:
			res = "Normal Mode";
			break;
		}
		return res;
	}

	static Type to_mode(const string& str) {
		if (str == "blur")
			return Blur;
		else if (str == "pixel")
			return Pixel;
		else if (str == "mask")
			return Mask;
		return Normal;
	}
	
protected:
	Type type_;
};
