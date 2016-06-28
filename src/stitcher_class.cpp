#include <stdio.h>  
#include "opencv2/opencv.hpp" 
#include "opencv2/stitching/stitcher.hpp"
#include <stdio.h>
#include <iostream> 
#include <unistd.h>

#ifdef _DEBUG  
#pragma comment(lib, "opencv_core246d.lib")   
#pragma comment(lib, "opencv_imgproc246d.lib")   //MAT processing  
#pragma comment(lib, "opencv_highgui246d.lib")  
#pragma comment(lib, "opencv_stitching246d.lib");

#else  
#pragma comment(lib, "opencv_core246.lib")  
#pragma comment(lib, "opencv_imgproc246.lib")  
#pragma comment(lib, "opencv_highgui246.lib")  
#pragma comment(lib, "opencv_stitching246.lib");
#endif  

using namespace cv;  
using namespace std;


int main()  
{

// Load the images
VideoCapture cap1(1); 
VideoCapture cap2(2); 
VideoCapture cap3(3);

for(;;)
{
	Mat image1;
	cap1 >> image1; // get a new frame from camera

	Mat image2;
	cap2 >> image2; // get a new frame from camera
	
	Mat image3;
	cap3 >> image3; // get a new frame from camera
	
	imshow("first image",image1);
	cv::imwrite("./data/Image1.jpg", image1);
	
	imshow("second image",image2);
	cv::imwrite("./data/Image2.jpg", image2);
	
	imshow("third image",image3);
	cv::imwrite("./data/Image3.jpg", image3);
	
	if( !image1.data || !image2.data || !image3.data )
	 { std::cout<< " --(!) Error reading images " << std::endl; return -1; }
	

	vector< Mat > vImg;
	Mat rImg;

	vImg.push_back( image1 );
	vImg.push_back( image2 );
	vImg.push_back( image3 );
	  

	Stitcher stitcher = Stitcher::createDefault();


	unsigned long AAtime=0, BBtime=0; //check processing time
	AAtime = getTickCount(); //check processing time

	Stitcher::Status status = stitcher.stitch(vImg, rImg);

	BBtime = getTickCount(); //check processing time 
	printf("%.2lf sec \n",  (BBtime - AAtime)/getTickFrequency() ); //check processing time

	if (Stitcher::OK == status) 
	{
	  imshow("Result",rImg);
	  cv::imwrite("./Result/stitching_result.jpg", rImg);
	}
	else
	  printf("Stitching fail.");

	if(waitKey(30) >= 0) break;
	else usleep(2000000);
	// waitKey(0);

}
}  