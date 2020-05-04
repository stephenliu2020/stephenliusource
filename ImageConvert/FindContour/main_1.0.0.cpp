/*******************************************************************/
/****************    main_1.0.0.cpp  *******************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include "pch.h"
#include "simple_svg_1.0.0.hpp"
#include <vector>
#include <algorithm>
#include <string>
#define _USE_MATH_DEFINES  
#include <math.h>  
using namespace svg;
using namespace std;

SvgPoint curr;
double minDis =  5.0;
double maxDis =  45.0;
int mergeCompareAngle = 20;

extern "C"
{
	__declspec(dllexport) void set_step_value(double step)
	{
		minDis = step;
		maxDis = 50.0 + step - 10.0;
		if (step < 8)
		{
			mergeCompareAngle = 20;
		}
		else 
			mergeCompareAngle = 10;
	}
}
SvgPoint operator-(SvgPoint a, SvgPoint b)
{
	SvgPoint p;
	p.x = a.x - b.x;
	p.y = a.y - b.y;
	return p;
}

static double distance(SvgPoint a)
{
	double diffxa = a.x - curr.x;
	double diffya = a.y - curr.y;
	return sqrt(diffxa*diffxa + diffya * diffya);
}

static bool distance_compare(SvgPoint a, SvgPoint b)
{
	return distance(a) == distance(b);
}

bool judge_2points_side(SvgPoint& A, SvgPoint& B)
{
	SvgPoint v1 = A - curr, v2 = B - curr;
	int a = atan2(v2.y, v2.x) * 180 / M_PI;
	int b = atan2(v1.y, v1.x) * 180 / M_PI;
	int d = abs(a - b) % 360;
	int r = d > 180 ? 360 - d : d;
	double disDiff = distance(B) / distance(A);
	return (disDiff > 1.5f&&disDiff < 2.3f&& r < 20);
	//calculate sign 
	//int sign = (a - b >= 0 && a - b <= 180) || (a - b <= -180 && a - b >= -360) ? 1 : -1;
	//r *= sign;

}

bool calculate_diff(SvgPoint& A, SvgPoint& B, SvgPoint& C)
{
	SvgPoint v1 = B - A, v2 = C - B;
	int a = atan2(v2.y, v2.x) * 180 / M_PI;
	int b = atan2(v1.y, v1.x) * 180 / M_PI;
	int d = abs(a - b) % 360;
	int r = d > 180 ? 360 - d : d;

	//calculate sign 
	//int sign = (a - b >= 0 && a - b <= 180) || (a - b <= -180 && a - b >= -360) ? 1 : -1;
	//r *= sign;
	return (r < mergeCompareAngle);
}

SvgPoint find_start_point(vector<SvgPoint> points)
{
	if (points.size()<3)
		return *points.begin();
	std::vector < SvgPoint>::iterator first, second;
	//vector<SvgPoint> points = original;
	std::vector < SvgPoint>::iterator it = points.begin(), start= points.begin();
	for (int i=0;i<points.size();i++)
	{
		curr = points[i];
		points.erase(points.begin()+i);
		first = std::min_element(points.begin(), points.end(), distance_compare);
		SvgPoint theFirst = *first;
		int pos = first - points.begin();
		points.erase(first);
		second = std::min_element(points.begin(), points.end(), distance_compare);
		SvgPoint theSecond = *second;
		points.insert(points.begin()+pos, theFirst);
		if (judge_2points_side(theFirst, theSecond))
			return curr;

	}
	return *points.begin();
}

void merge_directline_in_points(vector<SvgPoint>& points)
{
	int size = points.size();
	if (size < 3)
		return;
	vector<SvgPoint>::iterator pt = points.begin();

	while (pt + 2 < points.end())
	{
		if (!calculate_diff(*pt, *(pt + 1), *(pt + 2)))
		{
			//*newPolyline << *(pt + 1);
			pt++;
		}
		else
			points.erase(pt + 1);
	}
}

void merge_directline_in_polyline(Polyline* polyline, Polyline* newPolyline)
{
	int size = polyline->points.size();
	if (size < 3)
	{
       if (size == 2)
		{
			*newPolyline << *polyline->points.begin();
			*newPolyline << SvgPoint((polyline->points.begin()->x+ polyline->points.back().x)/2.0f, (polyline->points.begin()->y + polyline->points.back().y) / 2.0f);
		}
		return;
	}

	vector<SvgPoint>::iterator pt = polyline->points.begin();
	*newPolyline << *pt;

	while (pt+2 < polyline->points.end())
	{
		if (!calculate_diff(*pt, *(pt + 1), *(pt + 2)))
		{
			*newPolyline << *(pt + 1);
			pt++;
		}
		else
			polyline->points.erase(pt + 1);
	}
	if (std::find(newPolyline->points.begin(), newPolyline->points.end(), polyline->points.back())== newPolyline->points.end())
		*newPolyline << polyline->points.back();
	/*if (newPolyline->points.size() == 2)
	{
		*newPolyline << SvgPoint((newPolyline->points.begin()->x + newPolyline->points.back().x) / 2.0f, (newPolyline->points.begin()->y + newPolyline->points.back().y) / 2.0f);
	}*/
	return;
}

void write_svg(int dimen_w, int dimen_h, std::vector<SvgPoint>& points, std::string outFileName)
{
	//merge_directline_in_points(points); // merge directline firstly
	Dimensions dimensions(dimen_w, dimen_h);// 100, 100);

	Document doc(outFileName, Layout(dimensions, Layout::TopLeft));// Layout::BottomLeft));

	Polyline* currPolyline = new Polyline(Stroke(.5, Color::Black));
	vector<Polyline*> polylines;
	polylines.push_back(currPolyline);

	std::vector < SvgPoint>::iterator result;
	curr = *points.begin();
	SvgPoint startPoint = find_start_point(points);// curr;
	*currPolyline << curr;
	//points.erase(points.begin());
	while (points.size())
	{
		result = std::min_element(points.begin(), points.end(), distance_compare);
		double theDis = distance(*result);
		if (theDis > minDis)
		{
			curr = *result;
			if (theDis < maxDis)
			{
				*currPolyline << curr;
			}
			else
			{
				if (distance(startPoint) <= minDis)
				{
					*currPolyline << curr;
					*currPolyline << startPoint;
				}
				else
				{
					currPolyline = new Polyline(Stroke(.5, Color::Black));
					polylines.push_back(currPolyline);
					startPoint = find_start_point(points);
				}

			}
		}
		points.erase(result);
	}
	if (distance(startPoint) <= minDis*2.0)
	  *currPolyline << startPoint;

	for (vector<Polyline*>::iterator it = polylines.begin(); it != polylines.end(); it++)
	{
		Polyline* newPolyline = new Polyline(Stroke(.5, Color::Black));
		merge_directline_in_polyline(*it, newPolyline);
		doc << *newPolyline;
	}


	//doc << (Polyline(Stroke(.5, Color::Black)) << SvgPoint(0, 5) << SvgPoint(20, 33) << SvgPoint(60, 56));
	doc.save();

	for (vector<Polyline*>::iterator it = polylines.begin(); it != polylines.end(); it++)
		delete *it;

}
