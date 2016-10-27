# Image-Stitching
Stitch Images using OpenCV in C++

##Project Description
This repository uses OpenCV to creat a panaroma-like version of videos.

The videos are captured from 3 video input cameras and are stitched frame by frame using the two methods discussed further.

## Algorithm
I have used two ways to perform image stitching. 

One uses the predefined Stitcher class <a href = "http://docs.opencv.org/2.4/modules/stitching/doc/stitching.html">OpenCV Stitcher Class Documentation.</a>
You just have to input the frames as a vector of images to the function stitcher.stitch() and it burps out the output stitched image.

Second method involves more steps which I will discuss here.

   First the frames from the 3 video input devices are captured. Then the pairwaise H matrix (homography matrix) is calculated. 

   The H matrix is 3*3 matrix.Homography relates the pixel co-ordinates in the two images.

   When applied to every pixel the new image is a warped version of the original image


   Let us assmue the 3 input frames are I1 , I2 and I3 in the order from left to right.(This assmuption is possible in my case since the camera positions are fixed and I have written the code keeping in mind the which camera will be the leftmost.)

   Now, we calculate the H matrix for image pairs (I1, I2), let us call it H12, and (I2, I3), let's call it H23. Since I1 and I3 will not have a lot of areas in common there is no point in calculating H matrix for these image pairs.

   For example, the images in the data folder :
   
   Image 1: 
   ![Alt text](https://github.com/Manasi94/Image-Stitching/blob/master/data/S1.jpg "Image1")

   Image 2: 
   ![Alt text](https://github.com/Manasi94/Image-Stitching/blob/master/data/S2.jpg "Image2")

   Image 3: 
   ![Alt text](https://github.com/Manasi94/Image-Stitching/blob/master/data/S3.jpg "Image3")

   Choose a image as reference image. Among the 3 images presented to us, the image I2 has the most in common with I1 and with I3 (since it is in the center). Hence we chose I2 as the reference image.This is also the reason why we do not compute H matrix for I1 and I3 image pairs, because we only compute the homography of images with the reference image, I2.

   Now using our H matrices we wrap the image pairs (I1,I2), now called I12, and (I2,I3), now called I23.

  Again calculate the H matrix for images I12 and I23 and wrap them up finally to get the ouptup I123. Tadaa!


##Implementation
I have chosen C++ language. 
Calculation of H matrix : 


1. Detecting the KeyPoints in an image using SURF.

    Speeded Up Robust Features.
    This detects the keypoints in the images.
    SURF is a local feature detector and descriptor. <a href = "http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_feature2d/py_surf_intro/py_surf_intro.html"> Introduction to SURF </a>

    To improve the feature detection you can change the variable value *minHessian*, which is the **Hessian Threshold**. 
  
    To increase the number of keypoints reduce the Hessian Threshold.

2. Calculating Descriptors :
   This computes and extracts the decriptors from the images using the keypoints from the previous step.

3. FLANN Matching :
   Fast Library for Approximate Nearest Neighbors  finds the best matches for local image features [3].

4. Filter, allowing only the good matches and find the correspoding keypoints.

5. Finds a perspective transformation between two planes using RANSAC

   RANdom SAmple Consensus (RANSAC) is a general parameter estimation approach designed to cope with a large
   proportion of outliers in the input data. 
   
   It takes the final keypoints from the two images as input and the method of model estimation. You can also change the method from RANSAC to LMEDS. The advantages and disadvantages of each method are discussed in [5]



##Errors and How they were fixed
1. Major error was the device capture index numbers. You can check these indices using VLC or other methods and modify these numbers in the code.

2. The order of images in first method is no concern. In second method however the order is very important as it can give you crappy output result.

3. In the second method after stitching the two image pairs, the result also had a black region. This region was hampering the further H matrix calculations and hence had to be removed. At first, I observed the black region occupied half of the image space and hence I decided to simply cut the part. However once i shifted from images to video ,I realized part of my image was also being chopped off. Hence I decided to use contours to solve this problem. The contours were sorted in descending order of size and the largest contours was selected and updated as the image space.
![Alt text](https://github.com/Manasi94/Image-Stitching/blob/master/Result/result234.jpg "Error Result")

##Conclusions
The second method works excellent with images.But it is however not producing excellent results with videos. The first method is giving decent results with videos, but is a little slow.
![Alt text](https://github.com/Manasi94/Image-Stitching/blob/master/Result/stitching_result.jpg "Result")

##Resources and References
1. <a href="http://people.scs.carleton.ca/~c_shu/Courses/comp4900d/notes/homography.pdf"> Dr. Gerhard Roth notes on homography </a>
2. <a href = "http://www.cs.toronto.edu/~jepson/csc2503/tutorials/homography.pdf"> Tutorial on 2D homographies at University of Toronto </a>
3. <a href = "http://www.cs.ubc.ca/~lowe/papers/09muja.pdf"> FAST APPROXIMATE NEAREST NEIGHBORS WITH AUTOMATIC ALGORITHM CONFIGURATION </a>
4. <a href = "http://www.cse.yorku.ca/~kosta/CompVis_Notes/ransac.pdf"> Overview of RANSAC Algorithm </a>
5. <a href = "http://www.loria.fr/~berger/Enseignement/Master2/Exposes/Choi.pdf"> Performance Evaluation of RANSAC Family </a>

##How to run this code
You need to change the name of input file and output file in CMakeLists.txt

Go to this directory (Image_Stitching) in your console and type "make".

After it succesfully  links and builds the target, execute the output file in the console. Example:

> ./img_stitch
