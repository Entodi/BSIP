#include "samples_handler.h"

#include <iostream>

bool SamplesHandler::addSample(const Sample& smpl)
{
    if (!((smpl.get_image().get_height() == height_) 
        && (smpl.get_image().get_width() == width_)))
    {
        std::cout << "Can't add sample because width and height don't match.";
        return false;
    }

    if (smpl.get_label() == 1)
        num_males_++;
    else if (smpl.get_label() == 0)
        num_females_++;
    else
    {
        std::cout << "Miss labeled sample " << v_samples_.size() + 1 << '.';
        return false;
    }
    v_samples_.push_back(smpl);
    return true;
}

void SamplesHandler::initWeights()
{
    double femaleWeight = 0.5 / static_cast<double>(num_females_);
    double maleWeight = 0.5 / static_cast<double>(num_males_);

    double samples_amount = static_cast<int>(v_samples_.size());
    for (int i = 0; i < samples_amount; i++)
    {
        if (v_samples_[i].get_label() == 0)
            v_samples_[i].set_weight(femaleWeight);
        else 
            v_samples_[i].set_weight(maleWeight);
    }
}

void SamplesHandler::normalizeWeights()
{
    double weightSum = 0;
    double samples_amount = static_cast<int>(v_samples_.size());
    for (int i = 0; i < samples_amount; i++)
        weightSum += v_samples_[i].get_weight();

    for (int i = 0; i < samples_amount; i++)
        v_samples_[i].set_weight(v_samples_[i].get_weight() / weightSum);
}

void SamplesHandler::set_sample_size(int height, int width)
{
    height_ = height;
    width_ = width;
}

int SamplesHandler::get_height() const
{
    return static_cast<int>(height_);
}

int SamplesHandler::get_width() const
{
    return static_cast<int>(width_);
}

int SamplesHandler::get_amount() const
{
    return static_cast<int>(v_samples_.size());
}

int SamplesHandler::get_males() const
{
    return num_males_;
}

int SamplesHandler::get_females() const
{
    return num_females_;
}

void SamplesHandler::reserveMemoryForSamples(int samples_amount)
{
    return v_samples_.reserve(samples_amount);
}

void SamplesHandler::clear()
{
    num_females_ = 0;
    num_males_ = 0;
    v_samples_.clear();
}

Sample& SamplesHandler::operator[](int id)
{
    return v_samples_[id];
}

const Sample& SamplesHandler::operator[](int id) const
{
    return v_samples_[id];
}