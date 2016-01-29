# Introduction
It is implementation of training module in C++ from article Boosting Sex Identification Performance http://www.cs.cmu.edu/~har/iaai2005.pdf

# Datasets
Datasets consist of images with 3 different sizes of man and woman face images.
* 16x16 - about 1136 images
* 20x20 - about 4505 images
* 24x24 - about 1139 images

file: xxsamples00.vec
Where xx equal to 16, 20 or 24

* 1 byte - width of dataset (w)
* 1 byte - height of dataset (h)

And images to the end of the file 

* w*h bytes - image (first row - last row of image)
* 1 byte - gender (1 for man and 255 for woman)


All samples are normalized to mean = 128 and variance = 64.

# Usage
To train model:
*.exe --train 1 --data_filename "16samples00.vec" --percent_trainset 0.8 --num_classifiers 20 --save_period 10

To evaluate accuracy of trained model:
*.exe --data_filename "16samples00.vec" --model_filename "model.bin" --percent_trainset 0.8

--train <1 or 0>
1 - to train model
0 - to evaluate accuracy of trained model (default)

--data_filename <filename> 
File with images in *.vec format

--model_filename <filename> 
Filename of trained model to evaluate accuracy

--percent_trainset <percent>
Percent of data as trainset should be in [0.5; 1],
to evaluate accuracy [0; 1] (default - 0.0 (0 %)

--percent_classifiers <percent>
Percent of classifiers to observe in every iterations,
should be from 0.01 to 1. (default - 0.8 (80%))

--num_classifiers <amount> 
Amount of classifiers to train. Should be from 1 to 1000.

--save_period <period> 
Save trained classifiers every 'period' iterations (default - 100)

--balanced <1 or 0>
Set flag to 1 if you want balanced train data (default - 0)

# Model file
file: model_HxW_NC_PTS_B_PC.bin
* H, W - height and width of training samples
* NC - number of weak classifiers
* PTS - persent of dataset for training
* B - balanced or unbalanced train set
* PC - percent of weak classifiers to choose random from all classifiers on every step

model_HxW_NC_PTS_B_PC.bin
* 4 byte - Int - number of weak classifiers

Number of weak classifiers times

* 4 byte - Int - first pixel row
* 4 byte - Int - first pixel column
* 4 byte - Int - second pixel row
* 4 byte - Int - second pixel column
* 8 byte - Double - Log(1/betta)
* 4 byte - Int - feature type of weak classifier
* 4 byte - Int - feature parity of weak classifier
* 8 byte - Double - accuracy on train set after adding this weak classifier
* 8 byte - Double - accuracy on test set after adding this weak classifier