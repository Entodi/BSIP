#pragma once

#include <vector>
#include <string>

#include "samples_handler.h"
#include "strong_classifier.h"
#include "configuration.h"

class TrainingModule
{
public:
	TrainingModule() {};
	bool	init(int argc, char *argv[]);
	bool	run();
private:
	bool	trainModel();
	bool	reevaluateModel();
	double	evaluateAccuracyOnTestSet();
	bool	loadModel(const std::string& filename);
	bool	saveModel(const std::string& filename);
	bool	initData(const std::string& data_filename);
	bool	splitData(double percent_train);
	
	bool	splitDataBalanced(double percent_train);
	bool	addPatch(const char* patch, int height, int width);

	Configuration config_;
	SamplesHandler samples_handler_;
	SamplesHandler train_samples_handler_;
	SamplesHandler test_samples_handler_;
	StrongClassifier strong_classifier_;

	TrainingModule(const TrainingModule&) {};
	void operator = (const TrainingModule&) {};
};