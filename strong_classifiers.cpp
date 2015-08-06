#include "strong_classifier.h"

#include <fstream>
#include <iostream>

void StrongClassifier::addFeature(Feature& ftr)
{
	v_best_features_.push_back(ftr);
	threshold_ = (threshold_ * 2.0 + ftr.get_log_beta()) / 2.0;
}

int StrongClassifier::predict(const Sample& smpl)
{
	double sum = 0;
	int num_features = static_cast<int>(v_best_features_.size());

	for (int i = 0; i < num_features; i++)
		sum += v_best_features_[i].get_log_beta() * v_best_features_[i].computeFeature(smpl);

	return (sum >= threshold_) ? 1 : 0;
}

double StrongClassifier::evaluateAccuracy(const SamplesHandler& test_samples_handler)
{
	int num_samples = test_samples_handler.get_amount();
	int num_true_predicted = 0;

	for (int i = 0; i < num_samples; i++)
	{
		int asnwer = predict(test_samples_handler[i]);
		if (test_samples_handler[i].get_label() == asnwer)
			num_true_predicted++;
	}

	double accuracy = static_cast<double>(num_true_predicted)
		/ static_cast<double>(num_samples);

	return accuracy;
}

void StrongClassifier::set_threshold(double threshold)
{
	threshold_ = threshold;
}

double StrongClassifier::get_threshold() const
{
	return 0.5*threshold_;
}

int StrongClassifier::get_amount() const
{
	return static_cast<int>(v_best_features_.size());
}

void StrongClassifier::set_model_filename(const std::string& model_filename)
{
	model_filename_ = model_filename;
}

bool StrongClassifier::saveModel()
{
	if (model_filename_.empty())
	{
		std::cout << "Filename to save model isn't set.";
		return false;
	}

	std::ofstream ofs;
	ofs.open(model_filename_, std::ofstream::out);
	if (!ofs.is_open())
	{
		std::cout << "Error: opening file " << model_filename_ << ".\n";
		return false;
	}

	int amount = static_cast<int>(v_best_features_.size());
	ofs << amount << ' ';

	for (int i = 0; i < amount; i++)
	{
		Pixel first_pxl = v_best_features_[i].get_first_pixel();
		Pixel second_pxl = v_best_features_[i].get_second_pixel();
		double log_beta = v_best_features_[i].get_log_beta();
		int ftr_type = static_cast<int>(v_best_features_[i].get_feature_type());
		int inv_parity = v_best_features_[i].get_inverse_parity();

		ofs << first_pxl.row_ << ' ' << first_pxl.col_ << ' ' << second_pxl.row_ << ' ' <<
			second_pxl.col_ << ' ' << log_beta << ' ' << ftr_type << ' ' << inv_parity << '\n';
	}

	ofs.close();

	std::cout << "Saved model to " << model_filename_ << ".\n";
	return true;
}

bool StrongClassifier::loadModel()
{
	if (model_filename_.empty())
	{
		std::cout << "Filename to save model isn't set.";
		return false;
	}

	std::ifstream ifs;
	ifs.open(model_filename_, std::ofstream::in);
	if (!ifs.is_open())
	{
		std::cout << "Error: opening file " << model_filename_ << '.';
		return false;
	}

	int amount;
	ifs >> amount;

	Pixel first_pxl;
	Pixel second_pxl;
	double log_beta;
	int ftr_type;
	int inv_parity;

	for (int i = 0; i < amount; i++)
	{
		Feature ftr;
		ifs >> first_pxl.row_ >> first_pxl.col_ >> second_pxl.row_ >>
			second_pxl.col_ >> log_beta >> ftr_type >> inv_parity;
		ftr.set_feature_type(static_cast<FeatureType>(ftr_type));
		ftr.set_pixels(first_pxl, second_pxl);
		ftr.set_log_beta(log_beta);
		ftr.set_inverse_parity(inv_parity);

		addFeature(ftr);
	}

	ifs.close();

	std::cout << "Loaded model from " << model_filename_ << ".\n";
	return true;
}

Feature& StrongClassifier::operator[](int id)
{
	return v_best_features_[id];
}

const Feature& StrongClassifier::operator[](int id) const
{
	return v_best_features_[id];
}

