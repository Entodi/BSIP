#include "adaboost.h"

#include <iostream>
#include <fstream>

bool AdaBoost::adaboost(SamplesHandler& train_samples_handler,
	const SamplesHandler& test_samples_handler,
	int num_classifiers,
	StrongClassifier& strong_classifier,
	int save_period)
{
	train_samples_handler.initWeights();
	generateFeatures(train_samples_handler.get_height(), train_samples_handler.get_width());

	for (int t = 0; t < num_classifiers; t++)
	{
		Feature best_feature;
		train_samples_handler.normalizeWeights();
		searchFeature(train_samples_handler, best_feature);
		AdaBoost::updateWeights(train_samples_handler, best_feature);
		strong_classifier.addFeature(best_feature);
		
		if ((t != 0) && ((t + 1) % save_period == 0))
		{
			std::cout << "\nTrained " << t + 1 << " weak classifiers." <<
				"\nAccuracy on train set:\t" << strong_classifier.evaluateAccuracy(train_samples_handler) << 
				"\nAccuracy on test set:\t" << strong_classifier.evaluateAccuracy(test_samples_handler) << "\n";
			if (strong_classifier.saveModel() == false)
				return false;
		}
	}

	return true;
}

void AdaBoost::updateWeights(SamplesHandler& train_samples_handler, Feature& best_feature)
{
	double beta = best_feature.get_beta();

	for (int i = 0; i < train_samples_handler.get_amount(); i++)
	{
		int answer = best_feature.computeFeature(train_samples_handler[i]);

		if (answer == train_samples_handler[i].get_label())
		{
			double new_weight = train_samples_handler[i].get_weight() * beta;
			train_samples_handler[i].set_weight(new_weight);
		}
	}
}

void AdaBoost::backupWeights(const SamplesHandler& samples_handler)
{
	std::ofstream ofs;
	ofs.open("weight_backup.txt", std::ofstream::out | std::ofstream::app);

	int num_samples = samples_handler.get_amount();

	for (int i = 0; i < num_samples; i++)
	{
		ofs << samples_handler[i].get_weight() << ' ';
	}

	ofs << '\n';
	ofs.close();
}

void AdaBoost::generateFeatures(int height, int width)
{
	int memory_need = 2 * Feature::kNumFeaturesTypes * height * width * (height * width - 1);
	v_features_.reserve(memory_need);
	Feature ftr;

	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			Pixel second_pixel(r, c);
			for (int x = 0; x < height; x++)
			{
				for (int y = 0; y < width; y++)
				{
					if (!((r == x) && (c == y)))
					{
						Pixel first_pixel(x, y);
						ftr.set_pixels(first_pixel, second_pixel);
						for (int ftrType = 0; ftrType < Feature::kNumFeaturesTypes; ftrType++)
						{
							ftr.set_feature_type(static_cast<FeatureType>(ftrType));
							v_features_.push_back(ftr);

							ftr.set_inverse_parity(true);
							v_features_.push_back(ftr);
						}
					}
				}
			}
		}
	}
}

void AdaBoost::searchFeature(const SamplesHandler& train_samples_handler, 
	Feature& best_feature)
{
	int num_features = static_cast<int>(v_features_.size());
	int num_samples = train_samples_handler.get_amount();
	int index = 0;

	for (int i = 0; i < num_features; i++)
	{
		if (!v_features_[i].is_taken())
		{
			v_features_[i].computeError(train_samples_handler);
			if (v_features_[i] < best_feature)
			{
				best_feature = v_features_[i];
				index = i;
			}
		}
	}
}