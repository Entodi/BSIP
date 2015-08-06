#include "sample.h"
#include <string>

const Image& Sample::get_image() const
{
	return img_;
}

int Sample::get_label() const
{
	return label_;
}

double Sample::get_weight() const
{
	return weight_;
}

void Sample::set_weight(double weight)
{
	weight_ = weight;
}