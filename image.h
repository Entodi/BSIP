#pragma once

#include <vector>

typedef unsigned char uint8_t;

class Pixel
{
public:
	int row_;
	int col_;

	Pixel() : row_(0), col_(0) {};
	Pixel(int row, int col) : row_(row), col_(col) {};
};

class Image
{
public:
	Image() : v_img_(std::vector<uint8_t>()), height_(0), width_(0) {};
	Image(std::vector<uint8_t>& v_img, int height, int width) :
		v_img_(v_img), height_(height), width_(width) {};

	int	get_width() const;
	int	get_height() const;
	int	get_pixel_value(const Pixel& pxl) const;
private:
	std::vector<uint8_t> v_img_;
	int height_;
	int width_;
};