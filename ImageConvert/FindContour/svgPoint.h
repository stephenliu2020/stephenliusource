#pragma once

#define EPSILON 0.01
struct SvgPoint
{
	SvgPoint(double x = 0, double y = 0) : x(x), y(y) { }
	double x;
	double y;
	bool operator==(const SvgPoint& a)
	{
		return fabs(this->x - a.x) < EPSILON && fabs(this->y - a.y) < EPSILON;
	}

};



