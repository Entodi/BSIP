#include "feature.h"

#include <iostream>

bool Feature::operator<(const Feature& ftr) const
{
	return error_ < ftr.error_;
}

void Feature::set_pixels(const Pixel& first_pixel, const Pixel& second_pixel)
{
	first_pixel_ = first_pixel;
	second_pixel_ = second_pixel;
}

void Feature::set_inverse_parity(int parity)
{
	inverse_parity_ = parity;
}

void Feature::set_feature_type(FeatureType ftrType)
{
	ftr_type_ = ftrType;
}

void Feature::set_error(double error)
{
    error_ = error;
}

void Feature::set_beta(double beta)
{
    beta_ = beta;
}

void Feature::set_log_beta(double log_beta)
{
	log_beta_ = log_beta;
}

int Feature::get_inverse_parity() const
{
	return inverse_parity_;
}

double Feature::get_error() const
{
	return error_;
}

double Feature::get_beta() const
{
	return beta_;
}

double Feature::get_log_beta() const
{
	return log_beta_;
}

FeatureType Feature::get_feature_type() const
{
	return ftr_type_;
}

Pixel Feature::get_first_pixel() const
{
	return first_pixel_;
}

Pixel Feature::get_second_pixel() const
{
	return second_pixel_;
}

void Feature::took()
{
	is_taken_flag_ = true;
}

bool Feature::is_taken() const
{
	return is_taken_flag_;
}

void Feature::subFromError(double weight)
{
    error_ -= weight;
}

int Feature::computeFeature(const Sample& smpl)
{
	int answer = 0;
	
	switch (ftr_type_)
	{
	case greater_type:
		if (smpl.get_image().get_pixel_value(first_pixel_) > smpl.get_image().get_pixel_value(second_pixel_))
			answer = 1;
		break;
	case within5_type:
		answer = get_asnwer_sample(smpl, 5);
		break;
	case within10_type:
		answer = get_asnwer_sample(smpl, 10);
		break;
	case within25_type:
		answer = get_asnwer_sample(smpl, 25);
		break;
	case within50_type:
		answer = get_asnwer_sample(smpl, 50);
		break;
	case no_type:
		std::cout << "Type isn't chosen!\n";
		break;
	default:
		std::cout << "Type " << ftr_type_ << " doesn't exist.";
		break;
	}

	if (inverse_parity_)
		answer = (answer == 1) ? 0 : 1;

	return answer;
}

inline int Feature::get_asnwer_sample(const Sample& smpl, int type_value)
{
    int value;
    uint8_t lowerBound;
    uint8_t upperBound;
    int second_pixel_value = static_cast<int>(smpl.get_image().get_pixel_value(second_pixel_));
    int first_pixel_value = static_cast<int>(smpl.get_image().get_pixel_value(first_pixel_));

    value = second_pixel_value - type_value;
    lowerBound = value < 0 ? 0 : static_cast<uint8_t>(value);
    value = second_pixel_value + type_value;
    upperBound = value > 255 ? 255 : static_cast<uint8_t>(value);

    if ((first_pixel_value >= lowerBound)
        && (first_pixel_value <= upperBound))
        return 1;
    return 0;
}

int Feature::computeFeature(int first_pixel_value, int second_pixel_value)
{
    int answer = 0;
    switch (ftr_type_)
    {
    case greater_type:
        answer = first_pixel_value > second_pixel_value ? 1 : 0;
        break;
    case within5_type:
        answer = get_asnwer_pixel(first_pixel_value, second_pixel_value, 5);
        break;
    case within10_type:
        answer = get_asnwer_pixel(first_pixel_value, second_pixel_value, 10);
        break;
    case within25_type:
        answer = get_asnwer_pixel(first_pixel_value, second_pixel_value, 25);
        break;
    case within50_type:
        answer = get_asnwer_pixel(first_pixel_value, second_pixel_value, 50);
        break;
    case no_type:
        std::cout << "Type isn't chosen!\n";
        break;
    default:
        std::cout << "Type " << ftr_type_ << " doesn't exist.";
        break;
    }

	if (inverse_parity_)
		answer = (answer == 1) ? 0 : 1;

    return answer;
}

inline int Feature::get_asnwer_pixel(int first_pixel_value,
    int second_pixel_value, int type_value)
{
    int value;
    uint8_t lowerBound;
    uint8_t upperBound;
    
	value = second_pixel_value - type_value;
	lowerBound = value < 0 ? 0 : static_cast<uint8_t>(value);
	value = second_pixel_value + type_value;
	upperBound = value > 255 ? 255 : static_cast<uint8_t>(value);

	if ((first_pixel_value >= lowerBound)
		&& (first_pixel_value <= upperBound))
		return 1;
	return 0;
}

void Feature::computeBetaAndLogBeta()
{
    beta_ = error_ / (1 - error_);
    log_beta_ = log(1.0 / beta_);
}

void Feature::show()
{
	std::cout << "Best feature type: " << ftr_type_ <<
		" first pixel: (" << first_pixel_.row_ << ',' << first_pixel_.col_ <<
		") second pixel: (" << second_pixel_.row_ << ',' << second_pixel_.col_ <<
		")\n error: " << error_ <<
		" parity: " << inverse_parity_ << ".\n";
}