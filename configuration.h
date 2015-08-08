#pragma once

#include <string>

class Configuration
{
public:
	Configuration() : data_filename_(""), model_filename_(""),
		percent_train_(0.0), num_classifiers_(0), balanced_flag_(0), 
		train_flag_(0), save_period_(100) {};

	bool load(int argc, char *argv[]);

	const std::string& get_data_filename() const;
	const std::string& get_model_filename() const;
	double	get_percent_train()	const;
	int		get_num_classifiers() const;
	bool	get_balanced_flag() const;
	bool	get_train_flag() const;
	int		get_save_period() const;

private:
	std::string data_filename_;
	std::string model_filename_;
	double 	percent_train_;
	int 	num_classifiers_;
	bool	balanced_flag_;
	bool	train_flag_;
	int		save_period_;

	bool validateParameters();
	void showParams();

	Configuration(const Configuration&) = delete;
	void operator = (const Configuration&) = delete;
};