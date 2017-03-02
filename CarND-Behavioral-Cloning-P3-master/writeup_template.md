#**Behavioral Cloning** 

##Writeup Template

###You can use this file as a template for your writeup if you want to submit it as a markdown file, but feel free to use some other method and submit a pdf if you prefer.

---

**Behavioral Cloning Project**

The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train and validate the model with a training and validation set
* Test that the model successfully drives around track one without leaving the road
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./examples/placeholder.png "Model Visualization"
[image2]: ./examples/Actual.png "Image"
[image3]: ./examples/originalcenter.png "Center Image"
[image4]: ./examples/originalleft.png "Left Image"
[image5]: ./examples/originalright.png "Right Image"
[image6]: ./examples/crop.png "Cropped"
[image7]: ./examples/resize.png "re-sized"
[image8]: ./examples/Flip.png "Flipped Image"

## Rubric Points
###Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/432/view) individually and describe how I addressed each point in my implementation.  

---
###Files Submitted & Code Quality

####1. Submission includes all required files and can be used to run the simulator in autonomous mode

My project includes the following files:
* model.py containing the script to create and train the model
* drive.py for driving the car in autonomous mode
* model.h5 containing a trained convolution neural network 
* run1.mp4 containing Track1 driving video of my car.
* writeup_report.md or writeup_report.pdf summarizing the results

####2. Submission includes functional code
Using the Udacity provided simulator and my drive.py file, the car can be driven autonomously around the track by executing 
```sh
python drive.py model.h5
```

####3. Submission code is usable and readable

The model.py file contains the code for training and saving the convolution neural network. The file shows the pipeline I used for training and validating the model, and it contains comments to explain how the code works.

###Model Architecture and Training Strategy

####1. An appropriate model architecture has been employed
I had been using the Nvidia Model architecture for my Model
My Model Consiste of 5 Convlotional neural Network Layers and 5 Fully connected Layers.

The model includes RELU layers to introduce nonlinearity (code lines 33,38,43,48,53,61,65,69 and 73), and the data is normalized in the model using a Keras lambda layer (code line 29). 

####2. Attempts to reduce overfitting in the model

The model contains dropout layers in order to reduce overfitting (model.py lines 72). 

The model was trained and validated on different data sets to ensure that the model was not overfitting. The model was tested by running it through the simulator and ensuring that the vehicle could stay on the track.

####3. Model parameter tuning

The model used an adam optimizer, so the learning rate was not tuned manually (model.py line 82).

####4. Appropriate training data

Training data was chosen to keep the vehicle driving on the road. I used a combination of center lane driving, recovering from the left and right sides of the road ... 

###Model Architecture and Training Strategy

####1. Solution Design Approach

The overall strategy for deriving a model architecture was to drive the car in a track by reading the images of the track

My first step was to use a convolution neural network model similar to the Nvidia. I thought this model might be appropriate because itis having 5 Convolotuinal Layers which will get most cheracterstics of the image.

In order to gauge how well the model was working, I split my image and steering angle data into a training and validation set. I found that my first model had a low mean squared error on the training set but a high mean squared error on the validation set. This implied that the model was overfitting. 

To combat the overfitting, I modified the model so that our model will not go for overfitting which effects our car to run in track with out going out of the road.

Then I used an adam optimizer, so the learning rate was not tuned manually (model.py line 82).   

The final step was to run the simulator to see how well the car was driving around track one. There were a few spots where the vehicle fell off the track, to improve the driving behavior in these cases, I have been using Center image, left image and right image to make it smooth. The Angle of Left and right image is choosen from center image angle on average +-0.25. Then Data Augementation like Cropping and resizing the image, fiping the image and normalizing the image provides good behaviour of predicting car to drive on proper road.

At the end of the process, the vehicle is able to drive autonomously around the track without leaving the road.

####2. Final Model Architecture

The final model architecture consisted of a convolution neural network with the following layers and layer sizes.

My Model Consiste of 5 Convlotional neural Network Layers
Conv Layer 1: 24x5x5
Conv Layer 2: 36x5x5
Conv Layer 3: 48x5x5
Conv Layer 4: 64x3x3
Conv Layer 5: 64x3x3 (model.py lines 32-54)

Flattening the Conv Layer 5 output(model.py line 57)

Five Fully Connected Layers
Fully Connected Layer 1 : Dense-1164
Fully Connected Layer 2 : Dense-100
Fully Connected Layer 3 : Dense-50
Fully Connected Layer 4 : Dense-10
Fully Connected Layer 5 : Dense-1  (model.py lines 60-80)

Dropout Function with 0.5 (model.py line 72)

The model includes RELU layers to introduce nonlinearity (code lines 33,38,43,48,53,61,65,69 and 73), and the data is normalized in the model using a Keras lambda layer (code line 29). 

Here is a visualization of the architecture (note: visualizing the architecture is optional according to the project rubric)

![alt text][image1]

####3. Creation of the Training Set & Training Process

To capture good driving behavior, I first recorded two laps on track one using center lane driving. Here is an example image of center lane driving:

![alt text][image2]

I then recorded the vehicle recovering from the left side and right sides of the road back to center so that the vehicle would learn to swift steering properly These images show what a recovery looks like starting from ... :

![alt text][image3]
![alt text][image4]
![alt text][image5]

Then I repeated this process on track two in order to get more data points.

To augment the data set, 
1) Firstly I cropped the image top and bottom of the image with dimesions(64:130)(model.py code line 93) and Re-sized the image to 64x64x3 (model.py code line 94)

![alt text][image6]
![alt text][image7]

2) I also flipped images and angles thinking that this would give better results so our model predicts the right turn and left turn properly if it drives in Anti-lock direction also(model,py code lines 101-105). For example, here is an image that has then been flipped:


![alt text][image8]


After the collection process, I had almost 8036 number of data points. I am preprocessing this data using generator function with batch size.

I finally randomly shuffled the data set and put 20% of the data into a validation set. 

I used this training data for training the model. The validation set helped determine if the model was over or under fitting. The ideal number of epochs was 9 as evidenced by the loss of 0.017. So it is preciting very Good. I used an adam optimizer so that manually training the learning rate wasn't necessary.
