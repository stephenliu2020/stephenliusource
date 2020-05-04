/*******************************************************************/
/**************       RobotUtility.cpp  ****************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include "RobotUtility.h"
void CRobotUtility::Swap(int& a, int& b)
{
	int temp;
	temp = a;
	a = b;
	b = temp;
}

float CRobotUtility::calculateDistance(const float& x1, 
	const float& y1, const float& x2, const float& y2)
{
	float diffY = y1 - y2;
	float diffX = x1 - x2;
	return sqrt((diffY * diffY) + (diffX * diffX));
}

float CRobotUtility::float_swap(float value)
{
	union v {
		float       f;
		unsigned int    i;
	};

	union v val;
	unsigned int temp;

	val.f = value;
	temp = htonl(val.i);

	return *(float*)&temp;
}

int CRobotUtility::FindLastIndexOf(const CString& s1, const CString& s2)
{
	int found = -1;
	int next_pos = 0;
	for (;;)
	{
		next_pos = s1.Find(s2, next_pos);
		if (next_pos == -1)
			return found;

		found = next_pos;
	};
}

float CRobotUtility::mag(float x, float y)
{
	return sqrt(x *x + y * y);
}
//return sqrt(a[0] * *2 + a[1] * *2)

//# get the unit vector of a vector
void CRobotUtility::unit(float& x, float& y)  // a/norm(a)
{
	float m = mag(x,y);
	x = x / m;
	y= y / m;
}

void CRobotUtility::rotate_vector(float& ux, float& uy, float theta)
{
	float sin_theta = sin(theta);
	float cos_theta = cos(theta);

	float _ux = ux * cos_theta - uy * sin_theta;
	float _uy = ux * sin_theta + uy * cos_theta;
	ux = _ux;
	uy = _uy;
}

float CRobotUtility::calculateDifferenceBetweenAngles(float firstAngle, float secondAngle)
{
	float difference = secondAngle - firstAngle;
	while (difference < -180) difference += 360;
	while (difference > 180) difference -= 360;
	return difference;
}
