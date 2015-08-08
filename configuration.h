#pragma once

#include <string>

class Configuration
{
public:
	Configuration() : data_filename_(""), model_filename_(""),
		percent_trainset_(0.0), percent_classifiers_(1.0), num_classifiers_(0), balanced_flag_(0), 
		train_flag_(0), save_period_(100) {};

	bool load(int argc, char *argv[]);

	const std::string& get_data_filename() const;
	const std::string& get_model_filename() const;
	double	get_percent_trainset()	const;
	double	get_percent_classifiers() const;
	int		get_num_classifiers() const;
	bool	get_balanced_flag() const;
	bool	get_train_flag() const;
	int		get_save_period() const;

private:
	std::string data_filename_;
	std::string model_filename_;
	double 	percent_trainset_;
	double	percent_classifiers_;
	int 	num_classifiers_;
	bool	balanced_flag_;
	bool	train_flag_;
	int		save_period_;

	bool validateParameters();
	void showParams();

	Configuration(const Configuration&){};
	void operator = (const Configuration&){};
};