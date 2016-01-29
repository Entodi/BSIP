#pragma once

#include "feature.h"

#include <string>

class StrongClassifier
{
public:
    StrongClassifier() : v_best_features_(std::vector<Feature>()), 
        v_train_accuracy_(std::vector<double>()),
        v_test_accuracy_(std::vector<double>()),
        threshold_(0.0), model_filename_("") {};

    void    reserveMemory(int num_classifiers);
    
    bool    addFeature(Feature& ftr);
    void    addAccuracy(double train_accuracy, double test_accuracy);
    
    int     predict(const Sample& smpl);
    
    double  evaluateAccuracy(const SamplesHandler& samples_handler);

    void    set_threshold(double threshold);
    double  get_threshold() const;
    int     get_amount() const;
    void    set_model_filename(const std::string& model_filename);

    bool    saveModel();
    bool    loadModel();

    Feature& operator[](int id);
    const Feature& operator[](int id) const;
private:
    std::vector<Feature> v_best_features_;
    std::vector<double> v_train_accuracy_;
    std::vector<double> v_test_accuracy_;
    double threshold_;
    std::string model_filename_;
};