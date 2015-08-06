#pragma once

#include <vector>
#include <queue>
#include <functional>

#include "samples_handler.h"
#include "strong_classifier.h"

class AdaBoost
{
public:
	bool adaboost(SamplesHandler& train_samples_handler,
		const SamplesHandler& test_samples_handler,
		int num_classifiers,
		StrongClassifier& strong_calssifier_,
		int save_period);
private:
	void updateWeights(SamplesHandler& train_samples_handler, 
		Feature& best_feature);
	void generateFeatures(int height, int width);
	void searchFeature(const SamplesHandler& train_samples_handler, 
		Feature& best_feature);

	void backupWeights(const SamplesHandler& samples_handler);

	std::vector<Feature> v_features_;
};
