#pragma once

class Mode {
public:
	enum Type {
		Blur,
		Pixel,
		Mask
	};

	Mode(Type type) : type_(type) {}
	~Mode() = default;

	Type getType() const { return type_; }
	virtual void handle() = 0;

protected:
	Type type_;
};
