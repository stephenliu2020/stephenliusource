/*******************************************************************/
/****************    FindContour.cpp *******************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

// FindContour.cpp : 
// This code run under 
// This file contains the 'main' function. if run as executable file, add #define RUN_AS_EXECUTABLE
// if no #define RUN_AS_EXECUTABLE, DLL can be provide to other project 
// Other module can invoke generate_svg_from_png(char* inputPNGFileName)
//

#include "pch.h"
#include <opencv2/opencv.hpp>
#include<iostream>
#define _USE_MATH_DEFINES  
#include <math.h>  
#include <string>   
#include <opencv2/core/core.hpp>      
#include <opencv2/highgui/highgui.hpp>   
//#include<imgproc/imgproc.hpp>    
#include "svgPoint.h"
using namespace std;
using namespace cv;

void write_svg(int dimen_w, int dimen_h, std::vector<SvgPoint>& points, string outFileName);
Mat srcImage, grayImage, out_Canny;

vector<vector<Point>> g_vContours;
vector<Vec4i> g_vHierarchy;

RNG G_RNG(1234);

#ifdef RUN_AS_EXECUTABLE
void main() {
		const char* inputPNGFileName = "n023.png";
		Mat image = imread(inputPNGFileName);

		cout << "image size:" << image.rows << "," << image.cols << endl;;
		Mat grayImage;
		cvtColor(image, grayImage, CV_BGR2GRAY);
		namedWindow("grayImage");
		imshow("grayImage", grayImage);
		//make binary image   
		Mat binaryImage;
		threshold(grayImage, binaryImage, 200, 255, CV_THRESH_BINARY); // make binary image based on threshold in scalar 90~255 
		namedWindow("binaryImage");
		imshow("binaryImage", binaryImage);
		//reverse binary image   
		Mat reverseBinaryImage;
		bitwise_not(binaryImage, reverseBinaryImage);

		vector <vector<Point>>contours;
		findContours(reverseBinaryImage,
			contours,   //contour vector 
			CV_RETR_LIST, //CV_RETR_EXTERNAL,   //only draw contour external
			CV_CHAIN_APPROX_NONE);  //  keep all contour points

		Mat result(reverseBinaryImage.size(), CV_8U, Scalar(255));
		drawContours(result, contours,
			-1, //all contours  
			Scalar(0),  //color of contour drawing => black color
			2); // contour thickness
		Mat Contours(reverseBinaryImage.size(), CV_8UC1, Scalar(255));
		std::vector<SvgPoint> spoints;
		for (int i = 0; i < contours.size(); i++)
		{
			for (int j = 0; j < contours[i].size(); j++)
			{
				Point p = Point(contours[i][j].x, contours[i][j].y);
				//cout << "(" << contours[i][j].x << "," << contours[i][j].y << ") ";
				SvgPoint thePoint(contours[i][j].x, contours[i][j].y);
				if (std::find(spoints.begin(), spoints.end(), thePoint) == spoints.end())
					spoints.push_back(thePoint);
				Contours.at<uchar>(p) = 0;
			}
		}

		std::string input(inputPNGFileName);
		std::size_t pos = input.find(".");
		std::string out;
		if (pos > 0)
			out = input.substr(0, pos) + ".rcp";
		else
			out = "myinput.rcp";
		write_svg(image.cols, image.rows, spoints, out);
		namedWindow("contours");
		imshow("contours", reverseBinaryImage);

		namedWindow("ManualContours");
		imshow("ManualContours", Contours);

		waitKey(0);
	    return 0;
	}
#else
extern "C"
{
    __declspec(dllexport) void generate_svg_from_png(char* inputPNGFileName) {

		Mat image = imread(inputPNGFileName);

		Mat grayImage;
		cvtColor(image, grayImage, CV_BGR2GRAY);

		Mat binaryImage;
		threshold(grayImage, binaryImage, 200, 255, CV_THRESH_BINARY); // make binary image based on threshold in scalar 90~255 
		//reverse binary image   
		Mat reverseBinaryImage;
		bitwise_not(binaryImage, reverseBinaryImage);

		vector <vector<Point>>contours;
		findContours(reverseBinaryImage,
			contours,   //contour vector 
			CV_RETR_LIST, //CV_RETR_EXTERNAL,   //only draw contour external
			CV_CHAIN_APPROX_NONE);  //  keep all contour points

		Mat result(reverseBinaryImage.size(), CV_8U, Scalar(255));
		drawContours(result, contours,
			-1, //all contours  
			Scalar(0),  //color of contour drawing => black color
			2); // contour thickness
		Mat Contours(reverseBinaryImage.size(), CV_8UC1, Scalar(255));
		std::vector<SvgPoint> spoints;
		for (int i = 0; i < contours.size(); i++)
		{
			for (int j = 0; j < contours[i].size(); j++)
			{
				Point p = Point(contours[i][j].x, contours[i][j].y);
				//cout << "(" << contours[i][j].x << "," << contours[i][j].y << ") ";
				SvgPoint thePoint(contours[i][j].x, contours[i][j].y);
				if (std::find(spoints.begin(), spoints.end(), thePoint) == spoints.end())
					spoints.push_back(thePoint);
				Contours.at<uchar>(p) = 0;
			}
		}

		std::string input(inputPNGFileName);
		std::size_t pos = input.find(".");
		std::string out;
		if (pos > 0)
			out = input.substr(0, pos) + ".rcp";
		else
			out = "myinput.rcp";
		write_svg(image.cols, image.rows, spoints, out);
	}
}
#endif