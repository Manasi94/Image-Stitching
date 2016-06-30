#include <stdio.h>
#include <iostream> 

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <unistd.h>
using namespace cv;
 
void readme()
{
	printf("This project takes input from 3 video streams and stitches the videos, frame by frame.");
}


Mat calculate_h_matrix(Mat image1, Mat image2, Mat gray_image1, Mat gray_image2)
{
	

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 200;
	SurfFeatureDetector detector( minHessian );
	std::vector< KeyPoint > keypoints_object, keypoints_scene;
	detector.detect( gray_image1, keypoints_object );
	detector.detect( gray_image2, keypoints_scene );

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor; 
	Mat descriptors_object, descriptors_scene;
	extractor.compute( gray_image1, keypoints_object, descriptors_object );
	extractor.compute( gray_image2, keypoints_scene, descriptors_scene );

	//-- Step 3: Matching descriptor vectors using FLANN matcher

	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match( descriptors_object, descriptors_scene, matches );
	matcher.match( descriptors_object, descriptors_scene, matches );


	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints 
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ 
	 double dist = matches[i].distance;
	 if( dist < min_dist ) min_dist = dist;
	 if( dist > max_dist ) max_dist = dist;
	 }
	 
	printf("-- Max dist: %f \n", max_dist );
	printf("-- Min dist: %f \n", min_dist );


	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;
	cv::Mat result;
	// cv::Mat result23;
	cv::Mat H;
	// cv::Mat H23;
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ 
	  if( matches[i].distance < 3*min_dist )
	  { good_matches.push_back( matches[i]); }
	}
	std::vector< Point2f > obj;
	std::vector< Point2f > scene;
	  
	for( int i = 0; i < good_matches.size(); i++ )
	{
	    //-- Get the keypoints from the good matches
	  obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
	  scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	}
	 

	// Find the Homography Matrix for img 1 and img2
	H = findHomography( obj, scene, CV_RANSAC );
	return H;
}


Mat stitch_image(Mat image1, Mat image2, Mat H)
{

	cv::Mat result;
	// cv::Mat result23;
	warpPerspective(image1,result,H,cv::Size(image1.cols+image2.cols,image1.rows));
	cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
	image2.copyTo(half);


	// cv::resize(result,result, Size(image1.cols,image1.rows),INTER_LINEAR);
	
	// cv::imshow("Result", result);

	// Mat ycrcb;

 //    cvtColor(result,ycrcb,CV_BGR2YCrCb);

 //    vector<Mat> channels;
 //    split(ycrcb,channels);

 //    equalizeHist(channels[0], channels[0]);

 //    Mat dst;
 //    merge(channels,ycrcb);

 //    cvtColor(ycrcb,dst,CV_YCrCb2BGR);

	// cv::imshow("Hist_Equalized_Result", dst);
	// cv::resize(dst,dst,image1.size());
	// cv::imwrite("./Result/Result.jpg", dst);
	// // cv::imwrite("./data/cam_left.jpg", image1);
	// // cv::imwrite("./data/cam_right.jpg", image2);
	// waitKey(0);
	return result;
}


// Mat hist_equalization()
// {
// cvtColor(img2, img_gray2, COLOR_BGR2GRAY);
// Mat ycrcb;
// cvtColor(result,ycrcb,CV_BGR2YCrCb);
// vector<Mat> channels;
// split(ycrcb,channels);
// equalizeHist(channels[0], channels[0]);
// Mat dst;
// merge(channels,ycrcb);
// cvtColor(ycrcb,img,CV_YCrCb2BGR);

// }

int main( int argc, char** argv )
{
	readme();
 // if( argc != 4 )
 // { readme(); return -1; }
Mat gray_image1;
Mat gray_image2;
Mat gray_image3;
Mat gray_image4;
Mat img, img2;
Mat gray_img;
Mat result;
Mat img_gray, img_gray2, img_gray3, img_gray4,img_gray5;
Mat img3, img4, img5, img6;

// Load the images
VideoCapture cap1(1); 
VideoCapture cap2(2); 
VideoCapture cap3(3);


for(;;)
{

	Mat image1;
	cap1 >> image1; // get a new frame from camera
	cvtColor(image1, gray_image1, COLOR_BGR2GRAY);

	Mat image2;
	cap2 >> image2; // get a new frame from camera
	cvtColor(image2, gray_image2, COLOR_BGR2GRAY);

	Mat image3;
	cap3 >> image3; // get a new frame from camera
	cvtColor(image3, gray_image3, COLOR_BGR2GRAY);

	imshow("first image",image1);
	cv::imwrite("./data/Image1.jpg", image1);
	
	imshow("second image",image2);
	cv::imwrite("./data/Image2.jpg", image2);
	
	imshow("third image",image3);
	cv::imwrite("./data/Image3.jpg", image3);
	
	if( !gray_image1.data || !gray_image2.data )
	 { std::cout<< " --(!) Error reading images " << std::endl; return -1; }
	



	// Mat image1 = imread("../Image_stitching/data/S1.jpg");
	// cvtColor(image1, gray_image1, COLOR_BGR2GRAY);

	// Mat image2 =imread("../Image_stitching/data/S2.jpg");
	// cvtColor(image2, gray_image2, COLOR_BGR2GRAY);

	// Mat image3 = imread("../Image_stitching/data/S3.jpg");
	// cvtColor(image3, gray_image3, COLOR_BGR2GRAY);

	// Mat image4 = imread("../Image_stitching/data/S5.jpg");
	// cvtColor(image4, gray_image4, COLOR_BGR2GRAY);

	// imshow("first image",image1);
	// imshow("second image",image2);
	// imshow("third image",image3);
	// imshow("fourth image", image4);

	// if( !gray_image1.data || !gray_image2.data )
	// 	{ std::cout<< " --(!) Error reading images " << std::endl; return -1; }




	Mat H12 = calculate_h_matrix(image2,image1, gray_image2, gray_image1);
	// Mat H13 = calculate_h_matrix(image1,image3, gray_image1, gray_image3);
	Mat H23 = calculate_h_matrix(image3,image2, gray_image3, gray_image2);
	// Mat H34 = calculate_h_matrix(image3,image4, gray_image3, gray_image4);



	/*The main logic is to chose a central image which is common to the 3 images. 
	In this code, there are 3 images, in the order: Image1, Image2 and Image 3 respectively. 
	The Image2 is thus comman to all three image and thus we chose this as the central image 
	and calculate the homography matrices of other images with respect to this image*/

	//Stitch Image 2 and Image 3 and saved in img
	img = stitch_image(image3,image2,H23);
	cvtColor(img, img_gray, COLOR_BGR2GRAY);

	// //Finding the largest contour i.e remove the black region from image
	threshold(img_gray, img_gray,25, 255,THRESH_BINARY); //Threshold the gray
	vector<vector<Point> > contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
    findContours( img_gray, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
    int largest_area = 0;
    int largest_contour_index = 0;
    Rect bounding_rect;

    for( int i = 0; i< contours.size(); i++ ) // iterate through each contour. 
    {
       double a=contourArea( contours[i],false);  //  Find the area of contour
       if(a>largest_area){
       largest_area=a;
       largest_contour_index=i;                //Store the index of largest contour
       bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
       }
   
    }

	// Scalar color( 255,255,255);
  	img = img(Rect(bounding_rect.x, bounding_rect.y, bounding_rect.width, bounding_rect.height));
 
 



	//Stitch Image 1 and Image 2 and saved in img2
	img2 = stitch_image(image2, image1, H12);
	cvtColor(img2, img_gray2, COLOR_BGR2GRAY);

	//Finding the largest contour i.e remove the black region from image
	threshold(img_gray2, img_gray2,25, 255,THRESH_BINARY); //Threshold the gray
	vector<vector<Point> > contours2; // Vector for storing contour
    vector<Vec4i> hierarchy2;
    findContours( img_gray2, contours2, hierarchy2,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
    int largest_area2 = 0;
    int largest_contour_index2 = 0;
    Rect bounding_rect2;

    for( int i = 0; i< contours2.size(); i++ ) // iterate through each contour. 
    {
       double a=contourArea( contours2[i],false);  //  Find the area of contour
       if(a>largest_area2){
       largest_area2=a;
       largest_contour_index2=i;                //Store the index of largest contour
       bounding_rect2=boundingRect(contours2[i]); // Find the bounding rectangle for biggest contour
       }
   
    }

	img2 = img2(Rect(bounding_rect2.x, bounding_rect2.y, bounding_rect2.width, bounding_rect2.height));

	// //Stitch Image 3 and Image 4 and saved in img3
	// img3 = stitch_image(image3, image4, H34);
	// img3 = img3(Rect(0,0,(img3.cols/2),(img3.rows)));
	// cvtColor(img3, img_gray3, COLOR_BGR2GRAY);


	//Show img
	cv::imshow("Hist_Equalized_Result of Image 2 and Image 3", img);
	cv::imwrite("./Result/Result23.jpg", img);
	// waitKey(0);

	//Show img2
	cv::imshow("Hist_Equalized_Result of Image 1 and Image 2", img2);
	cv::imwrite("./Result/Result12.jpg", img2);
	// waitKey(0);

	// //Show img3
	// // cv::resize(img2,img2,image1.size());
	// cv::imshow("Hist_Equalized_Result of Image 3 and Image 4", img3);
	// cv::imwrite("./Result/Result34.jpg", img3);
	// waitKey(0);


	// Stitch (Image 1 and Image 2) and (Image 2 and Image 3)
	Mat H123 = calculate_h_matrix(img,img2, img_gray, img_gray2);
	img4 = stitch_image(img,img2, H123);
	// img4 = img4(Rect(0,0,(img4.cols*7/10),(img4.rows)));
	// cvtColor(img4, img_gray4, COLOR_BGR2GRAY);

	// //Stitch (Image 2 and Image 3) and (Image 3 and Image 4)
	// Mat H234 = calculate_h_matrix(img3,img, img_gray3, img_gray);
	// img5 = stitch_image(img3,img, H234);
	// img5 = img5(Rect(0,0,(img5.cols*3/4),(img5.rows)));
	// cvtColor(img5, img_gray5, COLOR_BGR2GRAY);

	// //Stitch (Image 1 and Image 2 and Image 3) and (Image 2 and Image 3 and Image 4)
	// Mat H1234 = calculate_h_matrix(img5,img4, img_gray5, img_gray4);
	// img6 = stitch_image(img5,img4, H1234);

	cv::imshow("Hist_Equalized_Result of Image 1 and Image 2 and Image 3", img4);
	cv::imwrite("./Result/Result123.jpg", img4);
	// waitKey(0);

	// cv::imshow("Hist_Equalized_Result of Image 2 and Image 3 and Image 4" , img5);
	// cv::imwrite("./Result/Result234.jpg", img5);
	// waitKey(0);

	// cv::imshow("Hist_Equalized_Result of Image 1 and Image 2 and Image 3 and Image 4" , img6);
	// cv::imwrite("./Result/Result1234.jpg", img6);
	// waitKey(0);
	if(waitKey(30) >= 0) break;
	else usleep(2000000);

}

}