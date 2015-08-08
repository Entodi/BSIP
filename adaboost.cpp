#include "adaboost.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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

        if (best_feature.get_error() > 0.5)
            break;

		AdaBoost::updateWeights(train_samples_handler, best_feature);

		strong_classifier.addFeature(best_feature);
		double train_accuracy = strong_classifier.evaluateAccuracy(train_samples_handler);
		double test_accuracy = strong_classifier.evaluateAccuracy(test_samples_handler);
		strong_classifier.addAccuracy(train_accuracy, test_accuracy);

		std::cout << "\nTrained " << t + 1 << " weak classifiers." <<
			"\nAccuracy on train set:\t" << train_accuracy <<
			"\nAccuracy on test set:\t" << test_accuracy << '\n';
		
		// Save model
		if ((t != 0) && ((t + 1) % save_period == 0))
		{
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

void AdaBoost::generateFeatures(int height, int width)
{
	int memory_need = 2 * Feature::kNumFeaturesTypes * height * width * (height * width - 1);
	v_features_.reserve(memory_need);

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
						for (int ftrType = 0; ftrType < Feature::kNumFeaturesTypes; ftrType++)
						{
							Feature ftr;
							ftr.set_pixels(first_pixel, second_pixel);
							ftr.set_feature_type(static_cast<FeatureType>(ftrType));
							v_features_.push_back(ftr);

							// Inverse of ftr
							Feature i_ftr;
							i_ftr.set_pixels(first_pixel, second_pixel);
							i_ftr.set_feature_type(static_cast<FeatureType>(ftrType));
							i_ftr.set_inverse_parity(true);
							v_features_.push_back(i_ftr);
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
    int second_pixel_change_period = 10 * 
        (train_samples_handler.get_height() * train_samples_handler.get_width() - 1);
	int first_pixel_value;
	int second_pixel_value;

	// Compute error for every feature
	for (int i = 0; i < num_samples; i++)
	{
        const Image* image = &train_samples_handler[i].get_image();
        int label = train_samples_handler[i].get_label();

		// No need to compute inverse feature and they lying sequentially in the vector. 
		// This why j's step 2.
		for (int j = 0; j < num_features; j += 2)
		{
			// Because every 10 features have same first pixels
			if (j % 10 == 0)
				first_pixel_value =
				static_cast<int>(image->get_pixel_value(v_features_[j].get_first_pixel()));
			// Because every 'second_pixel_change_period' features have same second pixels
			if (j % second_pixel_change_period == 0)
				second_pixel_value =
				static_cast<int>(image->get_pixel_value(v_features_[j].get_second_pixel()));

			if ((!v_features_[j].is_taken()) && (!v_features_[j + 1].is_taken()))
			{
				int answer = v_features_[j].computeFeature(first_pixel_value, second_pixel_value);

				if (answer == label)
				{
					v_features_[j].subFromError(train_samples_handler[i].get_weight());
				}
				else
				{
					v_features_[j + 1].subFromError(train_samples_handler[i].get_weight());
				}
			}
		}
	}

	// Finding best feature
    int index = 0;
    for (int i = 0; i < num_features; i++)
	{
		if (!v_features_[i].is_taken())
		{
            if (v_features_[i] < best_feature)
            {
                best_feature = v_features_[i];
                index = i;
            }
		}
        v_features_[i].set_error(1.0);
	}
    v_features_[index].took();
    best_feature.computeBetaAndLogBeta(); 
}



bool AdaBoost::adaboostRandomPart(SamplesHandler& train_samples_handler,
	const SamplesHandler& test_samples_handler,
	int num_classifiers,
	StrongClassifier& strong_classifier,
	double percent_classifiers,
	int save_period)
{
	train_samples_handler.initWeights();

	generateFeatures(train_samples_handler.get_height(), train_samples_handler.get_width());
	int num_features_to_select_from = static_cast<int>(rint(v_features_.size() 
		* percent_classifiers));
	srand(time(NULL));

	for (int t = 0; t < num_classifiers; t++)
	{
		Feature best_feature;

		train_samples_handler.normalizeWeights();

		searchFeatureRandomPart(train_samples_handler, best_feature, num_features_to_select_from);

		if (best_feature.get_error() > 0.5)
			break;

		AdaBoost::updateWeights(train_samples_handler, best_feature);
		strong_classifier.addFeature(best_feature);
		double train_accuracy = strong_classifier.evaluateAccuracy(train_samples_handler);
		double test_accuracy = strong_classifier.evaluateAccuracy(test_samples_handler);
		strong_classifier.addAccuracy(train_accuracy, test_accuracy);

		std::cout << "\nTrained " << t + 1 << " weak classifiers." <<
			"\nAccuracy on train set:\t" << train_accuracy <<
			"\nAccuracy on test set:\t" << test_accuracy << '\n';

		// Save and show current accuracy
		if ((t != 0) && ((t + 1) % save_period == 0))
		{
			if (strong_classifier.saveModel() == false)
				return false;
		}
	}

	return true;
}


void AdaBoost::searchFeatureRandomPart(const SamplesHandler& train_samples_handler,
	Feature& best_feature, int num_features_to_select_from)
{
	int take_index;
	int num_features = static_cast<int>(v_features_.size());

	for (int i = 0; i < num_features_to_select_from;)
	{
		int rnd_index = rand() % num_features;

		if (v_features_[rnd_index].is_taken())
			continue;
		else
		{
			v_features_[rnd_index].computeError(train_samples_handler);

			if (v_features_[rnd_index] < best_feature)
			{
				best_feature = v_features_[rnd_index];
				take_index = rnd_index;
			}

			i++;
		}
	}

	v_features_[take_index].took();
}
