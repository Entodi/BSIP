#pragma once

#include "image.h"

class Sample
{
public:
    Sample(const Image& image, int label) :
        img_(image), label_(label), weight_(0.0) {};
    
    const Image& get_image() const;
    int     get_label() const;
    double  get_weight() const;
    void    set_weight(double weight);
private:
    Image img_;
    int label_;
    double weight_;
};