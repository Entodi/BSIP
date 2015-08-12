#include "image.h"

int Image::get_width() const
{
	return width_;
}

int Image::get_height() const
{
	return height_;
}

int Image::get_pixel_value(const Pixel& pxl) const
{
	return static_cast<int>(v_img_[pxl.col_ + pxl.row_ * width_]);
}