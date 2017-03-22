##Writeup Template
###You can use this file as a template for your writeup if you want to submit it as a markdown file, but feel free to use some other method and submit a pdf if you prefer.

---

**Advanced Lane Finding Project**

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

[//]: # (Image References)

[image1]: ./output_images/undistorted_image.png "Undistorted"
[image2]: ./output_images/Undistored_lanes.jpg "Road Transformed"
[image3]: ./test_images/s_bin5.jpg "Binary Example"
[image4]: ./output_images/bird_view_lanes.jpg "Warp Example"
[image5]: ./output_images/color_fit_lines.jpg "Fit Visual"
[image6]: ./output_images/Finaloutput.jpg "Output"
[video1]: ./project_output_final.mp4 "Video"

## [Rubric](https://review.udacity.com/#!/rubrics/571/view) Points
###Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---
###Writeup / README

####1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  You can submit your writeup as markdown or pdf.  [Here](https://github.com/udacity/CarND-Advanced-Lane-Lines/blob/master/writeup_template.md) is a template writeup for this project you can use as a guide and a starting point.  

You're reading it!
###Camera Calibration

####1. Briefly state how you computed the camera matrix and distortion coefficients. Provide an example of a distortion corrected calibration image.

The code for this step is contained in the first code cell of the IPython notebook located in "./P4.ipynb" (or the function img_calibration()). 

I start by preparing "object points", which will be the (x, y, z) coordinates of the chessboard corners in the world. Here I am assuming the chessboard is fixed on the (x, y) plane at z=0, such that the object points are the same for each calibration image.  Thus, `objp` is just a replicated array of coordinates, and `objpoints` will be appended with a copy of it every time I successfully detect all chessboard corners in a test image.  `imgpoints` will be appended with the (x, y) pixel position of each of the corners in the image plane with each successful chessboard detection. 

I then used the output `objpoints` and `imgpoints` to compute the camera calibration and distortion coefficients using the `cv2.calibrateCamera()` function.  I applied this distortion correction to the test image using the `cv2.undistort()` function and obtained this result: 

![alt text][image1]

###Pipeline (single images)

####1. Provide an example of a distortion-corrected image.
To demonstrate this step, I will describe how I apply the distortion correction to one of the test images like this one:
![alt text][image2]
####2. Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image.  Provide an example of a binary image result.
I used a combination of color and gradient thresholds to generate a binary image (thresholding step functions are abs_sobel_thresh(), mag_thresh(), color_threshold() and dir_threshold() i.e., 6, 7, 8, 9th code cell in P4.ipynb).  Here's an example of my output for this step.  (note: this is not actually from one of the test images)

![alt text][image3]

####3. Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.

The code for my perspective transform includes a function called `warper()`, which appears in lines the function perspective_transform() in the file `P4.ipynb`  i.e., 4rd code cell of the IPython notebook.  The `warper()` function takes as inputs an image (`img`), as well as source (`src`) and destination (`dst`) points.  I chose the hardcode the source and destination points in the following manner:

```
top=70
bottom=370
height, width = image.shape[0:2]
output_size = height/2

 src = np.float32([[(width/2) - top, height*0.65], 
	[(width/2) + top, height*0.65], 
	[(width/2) + bottom, height-50], 
	[(width/2) - bottom, height-50]])
 dst = np.float32([[(width/2) - output_size, (height/2) - output_size], 
	[(width/2) + output_size, (height/2) - output_size], 
	[(width/2) + output_size, (height/2) + output_size], 
	[(width/2) - output_size, (height/2) + output_size]])
    

```
This resulted in the following source and destination points:


| Source        | Destination   | 
|:-------------:|:-------------:| 
| 570, 468      | 280, 0        | 
| 710, 468      | 1000, 0       |
| 1010, 670     | 1000, 720     |
| 270, 670      | 280, 720      |


I verified that my perspective transform was working as expected by drawing the `src` and `dst` points onto a test image and its warped counterpart to verify that the lines appear parallel in the warped image.

![alt text][image4]

####4. Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial?

Then I did some other stuff and fit my lane lines with a 2nd order polynomial kinda like this:

![alt text][image5]

####5. Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.

I did this in measure_curvature() function in `P4.ipynb` file.

####6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.

I implemented this step in my code in `P4.ipynb` in the function `drawing_lane_detected()`.  Here is an example of my result on a test image:

![alt text][image6]

---

###Pipeline (video)

####1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (wobbly lines are ok but no catastrophic failures that would cause the car to drive off the road!).

Here's a [link to my video result](./project_video.mp4)

---

###Discussion

####1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

Here I'll talk about the approach I took, what techniques I used, what worked and why, where the pipeline might fail and how I might improve it if I were going to pursue this project further. 

To Execute this project(Advance Lane Detection) intilaly i processed the Camera images Calibration then Undisorting the image and after that make a view of bird eye image to see the lines parallel at any type of curve this is been done by using perceptive transform. After Perceptive transform Gradient and color threshold is been applied using sobel operator to identify the lane edges properly and after that 2nd degree polynomial fit is used for sliding wimdow technique to get exact left and right lanes and also measured the radius of curvature so that lane detection will be accurate, smooth at curves.
Finally the oputut can be visualized the project_output_final.mp4 file.

where the pipeline might fail..?
Pipeleine may behave differently for night camera with rainfall images and also if there no proper lane markings it may behave differntly. This can be tuned up for further enchaces like night images detection.

I might imporve this project if we go further..?
Yes definately, along with the lane detection i can provide steering angle for the curve and also deep learning technique can be used for more enhancements like road without lane markings or semi lane markings. 

