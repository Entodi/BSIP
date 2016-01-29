#pragma once

#include "sample.h"

#include <vector>

class SamplesHandler
{
public:
    SamplesHandler() : num_females_(0), num_males_(0), 
        height_(0), width_(0), v_samples_(std::vector<Sample>()) {};
    bool addSample(const Sample& smpl);
    void initWeights();
    void normalizeWeights();

    void set_sample_size(int height, int width);
    int get_width() const;
    int get_height() const;
    int get_amount() const;
    int get_females() const;
    int get_males() const;
    void reserveMemoryForSamples(int samples_amount);
    void clear();
    
    Sample& operator[](int id);
    const Sample& operator[](int id) const;
private:
    int num_females_;
    int num_males_;
    uint8_t height_;
    uint8_t width_;
    std::vector<Sample> v_samples_;
};
