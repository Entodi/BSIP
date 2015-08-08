#pragma once

#include "samples_handler.h"
#include <vector>

enum FeatureType
{
	greater_type,
	within5_type,
	within10_type,
	within25_type,
	within50_type,
	no_type,
};

class Feature
{
public:
	static const int kNumFeaturesTypes = 5;

	Feature() : 
		ftr_type_(no_type), 
		first_pixel_(Pixel()), 
		second_pixel_(Pixel()), 
		inverse_parity_(0), 
		error_(1.0), 
		beta_(0), 
		log_beta_(0), 
		is_taken_flag_(false) {};

	void		set_pixels(const Pixel& first_pixel, const Pixel& second_pixel);
	void		set_inverse_parity(int inverse_parity);
	void		set_feature_type(FeatureType ftr_type);
    void        set_error(double error);
    void        set_beta(double beta);
	void		set_log_beta(double log_beta);
	int			get_inverse_parity() const;
	double		get_error() const;
	double		get_beta() const;
	double		get_log_beta() const;
	Pixel		get_first_pixel() const;
	Pixel		get_second_pixel() const;
	FeatureType get_feature_type() const;

	void		took();
	bool		is_taken() const;
	
    void        subFromError(double weight);

	void		computeError(const SamplesHandler& samples_handler);	
    void        computeBetaAndLogBeta();
	int			computeFeature(const Sample& smpl);
    int         computeFeature(int first_pixel_value, int second_pixel_value);
	
	void		show();

	bool		operator<(const Feature& ftr) const;
private:
	int			get_asnwer_sample(const Sample& smpl, int type_value);
    int			get_asnwer_pixel(int first, int second, int type_value);

	FeatureType ftr_type_;
	Pixel first_pixel_;
	Pixel second_pixel_;
	int inverse_parity_;
	double error_;
	double beta_;
	double log_beta_;
	bool is_taken_flag_;
};