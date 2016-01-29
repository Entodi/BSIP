#include "training_module.h"

#include <fstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <sstream>

#include "adaboost.h"

typedef unsigned char uint8_t;

bool TrainingModule::init(int argc, char *argv[])
{
    if (config_.load(argc, argv) == false)
        return false;

    std::cout << "Initializing data.\n";
    if (initData(config_.get_data_filename()) == false)
        return false;
    
    std::cout << "Splitting data to train and test sets.\n";
    if (config_.get_balanced_flag() == true)
    {
        if (splitDataBalanced(config_.get_percent_trainset()) == false)
            return false;
    }
    else
    {
        if (splitData(config_.get_percent_trainset()) == false)
            return false;
    }

    samples_handler_.clear();

    std::stringstream model_filename;
    model_filename << "model_" << 
        train_samples_handler_.get_height() << 'x' <<
        train_samples_handler_.get_width() << '_' <<
        config_.get_num_classifiers() << '_' << 
        config_.get_percent_trainset() << '_' << 
        config_.get_balanced_flag() << '_' << 
        config_.get_percent_classifiers() << ".bin";
    strong_classifier_.set_model_filename(model_filename.str());
    strong_classifier_.reserveMemory(config_.get_num_classifiers());

    return true;
}

bool TrainingModule::run()
{
    if (config_.get_train_flag())
    {
        std::cout << "Training...\n";
        if (trainModel() == false)
            return false;
        evaluateAccuracyOnTestSet();
        if (saveModel(config_.get_model_filename()) == false)
            return false;
    }
    else
    {
        std::cout << "Evaluating Accuracy...\n";
        if (loadModel(config_.get_model_filename()) == false)
            return false;
        evaluateAccuracyOnTestSet();
    }

    return true;
}

bool TrainingModule::trainModel()
{
    AdaBoost ab;
    if (config_.get_percent_classifiers() == 1)
    {
        if (ab.adaboost(train_samples_handler_, test_samples_handler_,
                config_.get_num_classifiers(), strong_classifier_, 
                config_.get_save_period()) == false)
            return false;
    }
    else
    {
        ab.adaboostRandomPart(train_samples_handler_, test_samples_handler_,
            config_.get_num_classifiers(), strong_classifier_, 
            config_.get_percent_classifiers(),
            config_.get_save_period());
    }
    return true;
}

bool TrainingModule::initData(const std::string& data_filename)
{
    std::ifstream ifs;
    ifs.open(data_filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (!ifs.is_open())
    {
        std::cout << "Error: opening file " << data_filename << '.';
        return false;
    }

    int width;
    int height;
    char c;

    if (!ifs.get(c))
    {
        std::cout << "Error: reading width.";
        return false;
    }
    width = static_cast<int>(static_cast<uint8_t>(c));

    if (!ifs.get(c))
    {
        std::cout << "Error: reading height.";
        return false;
    }
    height = static_cast<int>(static_cast<uint8_t>(c));

    int buf_size = height * width + 1;
    char* buffer = new char[buf_size];

    samples_handler_.set_sample_size(height, width);
    train_samples_handler_.set_sample_size(height, width);
    test_samples_handler_.set_sample_size(height, width);

    while (ifs.good())
    {
        ifs.read(buffer, buf_size);

        if (buf_size == ifs.gcount())
        {
            if (addPatch(buffer, height, width) == false)
                return false;
        }
        else if (ifs.eof())
            break;
        else 
        {
            std::cout << "Error: reading patches.";
            return false;
        }
    }

    delete[] buffer;
    ifs.close();
    return true;
}

bool TrainingModule::splitData(double percent_train)
{
    int sample_amount = samples_handler_.get_amount();
    int train_samples_amount = 
        static_cast<int>(floor(static_cast<double>(sample_amount) * percent_train));

    train_samples_handler_.reserveMemoryForSamples(train_samples_amount);
    for (int i = 0; i < train_samples_amount; i++)
        if (train_samples_handler_.addSample(samples_handler_[i]) == false)
            return false;

    int test_samples_amount = sample_amount - train_samples_amount;
    test_samples_handler_.reserveMemoryForSamples(test_samples_amount);
    for (int i = train_samples_amount; i < sample_amount; i++)
        if (test_samples_handler_.addSample(samples_handler_[i]) == false)
            return false;

    std::cout << "Loaded " << sample_amount <<
        " samples\n\ttrain samples:\t" << train_samples_amount <<
        "\n\ttest samples:\t" << test_samples_amount <<
        "\n\theight:\t" << train_samples_handler_.get_height() << 
        "\n\twidth:\t" << train_samples_handler_.get_width() << '\n';

    return true;
}

bool TrainingModule::splitDataBalanced(double percent_train)
{

    int num_females = samples_handler_.get_females();
    int num_males = samples_handler_.get_males();

    int num_females_to_train =
        static_cast<int>(floor(static_cast<double>(num_females)* percent_train));
    int num_males_to_train =
        static_cast<int>(floor(static_cast<double>(num_males)* percent_train));

    if (num_females_to_train > num_males_to_train)
        num_females_to_train = num_males_to_train;
    else
        num_males_to_train = num_females_to_train;

    int samples_amount = samples_handler_.get_amount();
    for (int i = 0; i < samples_amount; i++)
    {
        if ((samples_handler_[i].get_label() == 1) && (num_males_to_train != 0))
        {
            if (train_samples_handler_.addSample(samples_handler_[i]) == false)
                return false;
            num_males_to_train--;
        }
        else if ((samples_handler_[i].get_label() == 0) && (num_females_to_train != 0))
        {
            if (train_samples_handler_.addSample(samples_handler_[i]) == false)
                return false;
            num_females_to_train--;
        }
        else
        {
            if (test_samples_handler_.addSample(samples_handler_[i]) == false)
                return false;
        }
    }

    std::cout << "Loaded " << samples_amount <<
        " samples\n\ttrain samples:\t" << train_samples_handler_.get_amount() <<
        "\n\ttest samples:\t" << test_samples_handler_.get_amount() <<
        "\n\theight:\t" << train_samples_handler_.get_height() <<
        "\n\twidth:\t" << train_samples_handler_.get_width() << '\n';

    return true;
}

bool TrainingModule::addPatch(const char* patch, int height, int width)
{
    std::vector<uint8_t> v_img(height*width);

    for (int row = height - 1; row >= 0; row--)
        for (int col = 0; col < width; col++)
            v_img[col + row * width] = static_cast<uint8_t>(*patch++);

    int label = (static_cast<uint8_t>(*patch) == 255) ? 0 : 1;

    Image img(v_img, height, width);
    if (samples_handler_.addSample(Sample(img, label)) == false)
        return false;

    return true;
}

bool TrainingModule::saveModel(const std::string& filename)
{
    if (strong_classifier_.saveModel() == false)
        return false;
    return true;
}

bool TrainingModule::loadModel(const std::string& filename)
{
    strong_classifier_ = StrongClassifier();
    strong_classifier_.set_model_filename(config_.get_model_filename());
    if (strong_classifier_.loadModel() == false)
        return false;
    return true;
}

double TrainingModule::evaluateAccuracyOnTestSet()
{
    double accuracy = strong_classifier_.evaluateAccuracy(test_samples_handler_);
    std::cout << "\nOverall accuracy: " << accuracy << '\n';
    return accuracy;
}