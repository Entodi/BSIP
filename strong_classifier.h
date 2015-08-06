#pragma once

#include "feature.h"

#include <string>

class StrongClassifier
{
public:
    StrongClassifier() : v_best_features_(std::vector<Feature>()), 
        threshold_(0.0), model_filename_("") {};

	void	addFeature(Feature& ftr);
	int		predict(const Sample& smpl);
	double	evaluateAccuracy(const SamplesHandler& test_samples_handler);

	void	set_threshold(double threshold);
	double	get_threshold() const;
	int		get_amount() const;

	void	set_model_filename(const std::string& model_filename);
	bool	saveModel();
	bool	loadModel();

	Feature& operator[](int id);
	const Feature& operator[](int id) const;
private:
	std::vector<Feature> v_best_features_;
	double threshold_;
	std::string model_filename_;
};