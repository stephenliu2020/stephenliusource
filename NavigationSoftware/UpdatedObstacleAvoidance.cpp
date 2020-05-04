/*******************************************************************/
/**************  UpdatedObstacleAvoidance.cpp **********************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include "UpdatedObstacleAvoidance.h"
//#define _USE_MATH_DEFINES 
//#include <math.h>

#define TEST_67DEGREE_ROTATION
UpdatedObstacleAvoidance::UpdatedObstacleAvoidance
(int gWidth, int scale, SensorsSharedMemAccess* pSensorAccessor)
	:gridWidth(gWidth), m_scaleValue(scale), m_obstacleExistInMap(0),
	pSensorDataAccessor(pSensorAccessor), m_currFlag(0), m_obstacleCheckStart(false),
	m_repathFound(false), m_bCheckObstacleExistsIsDone(false), m_existingFrontObs(-1, -1, -1),
	m_currentMoveStep(0), m_bNoStopRepathIsDoing(false)
{
	CFrameWnd* pFrameWnd = (CFrameWnd*)AfxGetMainWnd();
	m_pView = pFrameWnd->GetActiveView();
}


UpdatedObstacleAvoidance::~UpdatedObstacleAvoidance()
{
}

void UpdatedObstacleAvoidance::setRobotPose(int x, int y, int robotAngle)
{
	m_absoluteAngle = robotAngle;
	m_robotStartX = x;
	m_robotStartY = y;
}
void UpdatedObstacleAvoidance::CoordinateTransfer(int angle, int a, int b, int xIn, int zIn, int& x, int& y)
{
	double mySin = sin(angle*M_PI / 180), myCos = cos(angle*M_PI / 180),
		myTan = tan(angle*M_PI / 180);

	angle = (angle + 360 * 2) % 360;
	double x1, y1;
	if (angle == 90 || angle == 270)
	{
		x1 = zIn*mySin;
		y1 = xIn;
	}
	else
	{
		x1 = xIn*myCos + zIn*mySin;
		y1 = zIn / myCos - x1*myTan;
	}

	x = a + x1;
	y = b + y1;
}

/*
void UpdatedObstacleAvoidance::getSaftyMovingPoint()
{
	if (m_obsData.size() == 0) return;
	else if (m_obsData.size() == 1)
	{
		securePoints.push_back(OBS_PIXEL(m_obsData[0].x - m_obsData[0].width / 2 - ROBOT_MOVING_SECURE_WIDTH / 2, m_obsData[0].y, 0));
		securePoints.push_back(OBS_PIXEL(m_obsData[0].x + m_obsData[0].width / 2 + ROBOT_MOVING_SECURE_WIDTH / 2, m_obsData[0].y, 0));
	}
	else
	{
		std::sort(m_obsData.begin(), m_obsData.end(), ColumnLess);
		std::vector<OBS_PIXEL>::iterator it = m_obsData.begin();
		securePoints.push_back(OBS_PIXEL(it->x - it->width / 2 - ROBOT_MOVING_SECURE_WIDTH / 2, it->y, 0));
		std::vector<OBS_PIXEL>::iterator prevIt = it;
		for (it = std::next(it, 1); it != m_obsData.end(); ++it)
		{
			if (abs((it->x - it->width / 2) - (prevIt->x + prevIt->width / 2))>ROBOT_MOVING_SECURE_WIDTH)
			{
				int safePoint;
				bool closeCurrentObstacle = true;
				if (it->x - it->width / 2 <= 0)
				{
					closeCurrentObstacle = true;
				}
				else
				{
					if (prevIt->x + prevIt->width / 2 >= 0)
						closeCurrentObstacle = false;
					else
					{
						if (abs(it->x - it->width / 2) <= abs(prevIt->x + prevIt->width / 2))
							closeCurrentObstacle = true;
						else
							closeCurrentObstacle = false;
					}
				}
				if (closeCurrentObstacle)
					safePoint = it->x - it->width / 2 - ROBOT_MOVING_SECURE_WIDTH / 2;
				else
					safePoint = prevIt->x + prevIt->width / 2 + ROBOT_MOVING_SECURE_WIDTH / 2;
				//safePoint = (it->x - it->width / 2 + prevIt->x + prevIt->width / 2) / 2;
				int y = it->y > prevIt->y ? it->y : prevIt->y;
				securePoints.push_back(OBS_PIXEL(safePoint, y, 0));
			}
			prevIt = it;
		}
		it = m_obsData.end() - 1;
		securePoints.push_back(OBS_PIXEL(it->x + it->width / 2 + ROBOT_MOVING_SECURE_WIDTH / 2, it->y, 0));
	}
	// First and last one are the edge, need extension check
	//if (securePoints.size()>2) // Sort [2nd, ... end-1) for abs check first
	std::sort(securePoints.begin(), securePoints.end(), AbsLess);

}
*/

void UpdatedObstacleAvoidance::getSaftyMovingPoint()
{
	//m_radius_need_adjusted = false;
	if (m_obsData.size() == 0) return;
	else if (m_obsData.size() == 1)
	{
		securePoints.push_back(OBS_PIXEL(m_obsData[0].x - m_obsData[0].width / 2 - ROBOT_MOVING_SECURE_WIDTH / 2, m_obsData[0].y, 0));
		securePoints.push_back(OBS_PIXEL(m_obsData[0].x + m_obsData[0].width / 2 + ROBOT_MOVING_SECURE_WIDTH / 2, m_obsData[0].y, 0));
	}
	else
	{
		std::sort(m_obsData.begin(), m_obsData.end(), ColumnLess);
		std::vector<OBS_PIXEL>::iterator it = m_obsData.begin();
		securePoints.push_back(OBS_PIXEL(it->x - it->width / 2 - ROBOT_MOVING_SECURE_WIDTH / 2, it->y, 0));
		std::vector<OBS_PIXEL>::iterator prevIt = it;
		for (it = std::next(it, 1); it != m_obsData.end(); ++it)
		{
			int prevRightEdge = prevIt->x + prevIt->width / 2;
			int currLeftEdge = it->x - it->width / 2;
			int obstDist = abs(currLeftEdge - prevRightEdge);
			if (obstDist>ROBOT_MOVING_SECURE_WIDTH)
			{
				int safePoint;
				if (obstDist < ROBOT_MOVING_SECURE_WIDTH * 2)
				{
					if (prevRightEdge < 0 && currLeftEdge > 0)
					{
						if (abs(prevRightEdge) > abs(currLeftEdge))
							safePoint = prevRightEdge + ROBOT_MOVING_SECURE_WIDTH / 2;
						else
							safePoint = currLeftEdge - ROBOT_MOVING_SECURE_WIDTH / 2;
						//m_radius_need_adjusted = true;
						//m_adjusted_safePoint = safePoint;
					}
					else
						safePoint = (currLeftEdge + prevRightEdge) / 2;
				}
				else
				{
					if (currLeftEdge <= 0)
						safePoint = currLeftEdge - ROBOT_MOVING_SECURE_WIDTH;
					else if (prevRightEdge >= 0)
						safePoint = prevRightEdge + ROBOT_MOVING_SECURE_WIDTH;
					else //prevRightEdge < 0 && currLeftEdge > 0
					{
						if (abs(prevRightEdge)<abs(currLeftEdge))
							safePoint = prevRightEdge + ROBOT_MOVING_SECURE_WIDTH;
						else
							safePoint = currLeftEdge - ROBOT_MOVING_SECURE_WIDTH;
					}
				}
				int y = it->y > prevIt->y ? it->y : prevIt->y;
				securePoints.push_back(OBS_PIXEL(safePoint, y, obstDist));
			}
			prevIt = it;
		}
		it = m_obsData.end() - 1;
		securePoints.push_back(OBS_PIXEL(it->x + it->width / 2 + ROBOT_MOVING_SECURE_WIDTH / 2, it->y, 0));
	}
	// First and last one are the edge, need extension check
	//if (securePoints.size()>2) // Sort [2nd, ... end-1) for abs check first
	std::sort(securePoints.begin(), securePoints.end(), SafeLess);

}

void UpdatedObstacleAvoidance::setNewRobotPose(int rotateAngle, int& newRobotStartX, int& newRobotStartY)
{
	int xf, yf;

	double r = 0.2;
	double mySin = sin(rotateAngle * M_PI / 180), myCos = cos(rotateAngle * M_PI / 180);

	xf = m_scaleValue * r*sin(rotateAngle * M_PI / 180);
	yf = m_scaleValue * (-r+r*cos(rotateAngle * M_PI / 180));
	////////////////
	if (xf > 0)
	{
		CoordinateTransfer(-m_absoluteAngle - 90 /*+ (90- rotateAngle)*/, m_robotStartX, m_robotStartY,
			xf, -yf,
			newRobotStartX, newRobotStartY);
	}
	else
	{
		CoordinateTransfer(-m_absoluteAngle - 90/* - (90 - rotateAngle)*/, m_robotStartX, m_robotStartY,
			xf, -yf,
			newRobotStartX, newRobotStartY);
	}
}

void UpdatedObstacleAvoidance::safePointCoordinateTransfer(int itx, int ity,
	int& newXs, int& newYs, int& newXf, int& newYf, // for 90 degree
	int& newXs1, int& newYs1, int& newXf1, int& newYf1, // for 55 degree
	int& newXs2, int& newYs2, int& newXf2, int& newYf2, // for 70 degree
	int& newXs3, int& newYs3, int& newXf3, int& newYf3) // for 35 degree
{
	int xs, ys, xf, yf;
	xs = itx;
	ys = 0;
	xf = itx;
	yf = ity;
	if (yf >= m_scaleValue * 1.5)
		yf = m_scaleValue * 1.5;
	int newAngle = -m_absoluteAngle - 90;
	CoordinateTransfer(newAngle, m_robotStartX, m_robotStartY,
		xs, -ys,
		newXs, newYs);
	CoordinateTransfer(newAngle, m_robotStartX, m_robotStartY,
		xf, -yf,
		newXf, newYf);
	////////////////
	if (xs > 0)
	{
		CoordinateTransfer(-m_absoluteAngle - 90 + 35, m_robotStartX, m_robotStartY,
			xs, -ys,
			newXs1, newYs1);
		CoordinateTransfer(-m_absoluteAngle - 90 + 35, m_robotStartX, m_robotStartY,
			xf, -yf,
			newXf1, newYf1);
#ifdef TEST_67DEGREE_ROTATION
		CoordinateTransfer(-m_absoluteAngle - 90 + 20, m_robotStartX, m_robotStartY,
			xs, -ys,
			newXs2, newYs2);
		CoordinateTransfer(-m_absoluteAngle - 90 + 20, m_robotStartX, m_robotStartY,
			xf, -yf,
			newXf2, newYf2);
#endif
		CoordinateTransfer(-m_absoluteAngle - 90 + 55, m_robotStartX, m_robotStartY,
			xs, -ys,
			newXs3, newYs3);
		CoordinateTransfer(-m_absoluteAngle - 90 + 55, m_robotStartX, m_robotStartY,
			xf, -yf,
			newXf3, newYf3);
	}
	else
	{
		CoordinateTransfer(-m_absoluteAngle - 90 - 35, m_robotStartX, m_robotStartY,
			xs, -ys,
			newXs1, newYs1);
		CoordinateTransfer(-m_absoluteAngle - 90 - 35, m_robotStartX, m_robotStartY,
			xf, -yf,
			newXf1, newYf1);
#ifdef TEST_67DEGREE_ROTATION
		CoordinateTransfer(-m_absoluteAngle - 90 - 20, m_robotStartX, m_robotStartY,
			xs, -ys,
			newXs2, newYs2);
		CoordinateTransfer(-m_absoluteAngle - 90 - 20, m_robotStartX, m_robotStartY,
			xf, -yf,
			newXf2, newYf2);
#endif
		CoordinateTransfer(-m_absoluteAngle - 90 - 55, m_robotStartX, m_robotStartY,
			xs, -ys,
			newXs3, newYs3);
		CoordinateTransfer(-m_absoluteAngle - 90 - 55, m_robotStartX, m_robotStartY,
			xf, -yf,
			newXf3, newYf3);
	}

}
/*
bool UpdatedObstacleAvoidance::FindSecurePathInObstacleAvoidance(
int& newXs, int& newYs, int& newXf, int& newYf)
{
if (securePoints.size() == 0) return false;
std::vector<UpdatedObstacleAvoidance::OBS_PIXEL>::iterator it = securePoints.begin();
if (securePoints.size() <= 2)
{
for (it = std::next(it, 1); it != securePoints.end() - 1; ++it)
{
safePointCoordinateTransfer(it, newXs, newYs, newXf, newYf);
if (!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(m_robotStartX, m_robotStartY, newXs, newYs) &&
!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(newXs, newYs, newXf, newYf))
{
return true;
}
}
}

it = securePoints.begin(); // leftmost one, need check extra area
safePointCoordinateTransfer(it, newXs, newYs, newXf, newYf);
int x1, y1, x2, y2;
CAutoNavPathPlanView::GetPoint(newXs, newYs, m_absoluteAngle - 90, 100 * m_scaleValue / 100, x1, y1);
CAutoNavPathPlanView::GetPoint(newXf, newYf, m_absoluteAngle, 40 * m_scaleValue / 100, x2, y2);
if (!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(m_robotStartX, m_robotStartY, x1, y1) &&
!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(newXs, newYs, x2, y2))
{
return true;
}
it = securePoints.end()-1; // rightmost one, need check extra area
safePointCoordinateTransfer(it, newXs, newYs, newXf, newYf);
CAutoNavPathPlanView::GetPoint(newXs, newYs, m_absoluteAngle + 90, 100 * m_scaleValue / 100, x1, y1);
CAutoNavPathPlanView::GetPoint(newXf, newYf, m_absoluteAngle, 40 * m_scaleValue / 100, x2, y2);
if (!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(m_robotStartX, m_robotStartY, x1, y1) &&
!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(newXs, newYs, x2, y2))
{
return true;
}
return false;
}
*/
void UpdatedObstacleAvoidance::setTestPara(CDC* rangeDC, int testX, int testY)
{
	pdcRange = rangeDC;
	m_nScrollDeltaX = testX;
	m_nScrollDeltaY = testY;
}

bool UpdatedObstacleAvoidance::rePathObstacleCheck(int itx, int ity,
	int& newXs, int& newYs, int& newXf, int& newYf, bool mostEdge)
{
	int newXs1, newYs1, newXf1, newYf1; // for 55
	int newXs2, newYs2, newXf2, newYf2; // for 70
	int newXs3, newYs3, newXf3, newYf3; // for 35
	int threadshold = 1.0*(float)m_scaleValue;
//	if (abs(itx) < threadshold)
	//	itx = itx < 0 ? -threadshold : threadshold;
	safePointCoordinateTransfer(itx, ity, newXs, newYs, newXf, newYf, newXs1, newYs1, newXf1, newYf1,
		newXs2, newYs2, newXf2, newYf2, newXs3, newYs3, newXf3, newYf3);	int x1, y1, x2, y2, x0, y0;
	int shiftAngle = -90;
	int shiftAngle1, shiftAngle2; // for 50 degree
	int shiftAngle3, shiftAngle4; // for 70 degree
	int shiftAngle5, shiftAngle6; // for 35 degree
	int polarity;
	int newRobotStartX,  newRobotStartY;
	float radius = (((CAutoNavPathPlanView*)m_pView)->calculateDistance(m_robotStartX, m_robotStartY, newXs, newYs) /*- fabs(m_front_obsX)*/) / m_scaleValue;
	radius = fabs(itx )/ m_scaleValue;

	if (itx < 0)
	{
		shiftAngle = -90;
		shiftAngle1 = -55;
		shiftAngle2 = 35;
		shiftAngle3 = -70;
		shiftAngle4 = 20;
		shiftAngle5 = -35;
		shiftAngle6 = 55;
		polarity = 1;
		((CAutoNavPathPlanView*)m_pView)->m_curveRadius = -radius ;
	}
	else
	{
		shiftAngle = 90;
		shiftAngle1 = 55;
		shiftAngle2 = -35;
		shiftAngle3 = 70;
		shiftAngle4 = -20;
		shiftAngle5 = 35;
		shiftAngle6 = -55;
		polarity = -1;
		((CAutoNavPathPlanView*)m_pView)->m_curveRadius = radius; // counterclockwise
	}
	bool check1, check2, check3;
	float absRadius;
	((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(m_robotStartX - m_nScrollDeltaX, m_robotStartY - m_nScrollDeltaY),
		CPoint(m_robotStartX + 2 - m_nScrollDeltaX, m_robotStartY + 2 - m_nScrollDeltaY), RGB(125, 0, 125));
	// 35 degree
	setNewRobotPose(shiftAngle5, newRobotStartX, newRobotStartY);
	((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(newRobotStartX - m_nScrollDeltaX, newRobotStartY - m_nScrollDeltaY),
		CPoint(newRobotStartX + 2 - m_nScrollDeltaX, newRobotStartY + 2 - m_nScrollDeltaY), RGB(0, 255, 255));

	CAutoNavPathPlanView::GetPoint(newRobotStartX, newRobotStartY, m_absoluteAngle + shiftAngle6, fabs(radius)* m_scaleValue, x0, y0);
	if (//!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(m_robotStartX, m_robotStartY, newXs1, newYs1) &&
		checkMostEdgeSafeSpace(x0, y0, shiftAngle6, polarity, fabs(radius) + 0.35, mostEdge) &&
		checkMostEdgeSafeSpace(x0, y0, shiftAngle6, polarity, fabs(radius) - 0.20) &&
		checkMostEdgeSafeSpace(x0, y0, shiftAngle6, polarity, fabs(radius)))
	{
		newXs = newXs3; newYs = newYs3;
		newXf = newXf3; newYf = newYf3;
		m_repathFound = true;
		return true;
	}
	// 55 degree
	setNewRobotPose(shiftAngle1, newRobotStartX, newRobotStartY);
	((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(newRobotStartX - m_nScrollDeltaX, newRobotStartY - m_nScrollDeltaY),
		CPoint(newRobotStartX + 2 - m_nScrollDeltaX, newRobotStartY + 2 - m_nScrollDeltaY), RGB(0, 255, 255));
	//((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(m_robotStartX - m_nScrollDeltaX, m_robotStartY - m_nScrollDeltaY),
		//CPoint(newXs1  - m_nScrollDeltaX, newYs1  - m_nScrollDeltaY), RGB(0, 0, 255));
	CAutoNavPathPlanView::GetPoint(newRobotStartX, newRobotStartY, m_absoluteAngle + shiftAngle2, fabs(radius)* m_scaleValue, x0, y0);
	if (//!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(m_robotStartX, m_robotStartY, newXs1, newYs1) &&
		(check1=checkMostEdgeSafeSpace(x0, y0, shiftAngle2, polarity, fabs(radius) + 0.35, mostEdge)) &&
		(check2= checkMostEdgeSafeSpace(x0, y0, shiftAngle2, polarity, fabs(radius) - 0.3))&&
		(check3=checkMostEdgeSafeSpace(x0, y0, shiftAngle2, polarity, fabs(radius) )))
	{
		newXs = newXs1; newYs = newYs1;
		newXf = newXf1; newYf = newYf1;
		m_repathFound = true;
		return true;
	}

#ifdef TEST_67DEGREE_ROTATION
	setNewRobotPose(shiftAngle3, newRobotStartX, newRobotStartY);
	((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(newRobotStartX - m_nScrollDeltaX, newRobotStartY - m_nScrollDeltaY),
		CPoint(newRobotStartX + 2 - m_nScrollDeltaX, newRobotStartY + 2 - m_nScrollDeltaY), RGB(0, 255, 255));
	//((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(m_robotStartX - m_nScrollDeltaX, m_robotStartY - m_nScrollDeltaY),
		//CPoint(newXs2 - m_nScrollDeltaX, newYs2 - m_nScrollDeltaY), RGB(0, 0, 255));
	CAutoNavPathPlanView::GetPoint(newRobotStartX, newRobotStartY, m_absoluteAngle + shiftAngle4, fabs(radius)* m_scaleValue, x0, y0);
	if (//!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(m_robotStartX, m_robotStartY, newXs2, newYs2) &&
		(check1 = checkMostEdgeSafeSpace(x0, y0, shiftAngle4, polarity, fabs(radius) + 0.35, mostEdge)) &&
		(check2 = checkMostEdgeSafeSpace(x0, y0, shiftAngle4, polarity, fabs(radius) - 0.3))&&
		(check3 = checkMostEdgeSafeSpace(x0, y0, shiftAngle4, polarity, fabs(radius) )))
	{
		newXs = newXs2; newYs = newYs2;
		newXf = newXf2; newYf = newYf2;
		m_repathFound = true;
		return true;
	}
#endif

	setNewRobotPose(shiftAngle, newRobotStartX,  newRobotStartY);
	((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(newRobotStartX - m_nScrollDeltaX, newRobotStartY - m_nScrollDeltaY),
		CPoint(newRobotStartX+2 - m_nScrollDeltaX, newRobotStartY+2 - m_nScrollDeltaY), RGB(0, 255, 255));
	//((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(m_robotStartX - m_nScrollDeltaX, m_robotStartY - m_nScrollDeltaY),
		//CPoint(newXs - m_nScrollDeltaX, newYs - m_nScrollDeltaY), RGB(0, 0, 255));
	CAutoNavPathPlanView::GetPoint(newRobotStartX, newRobotStartY, m_absoluteAngle, fabs(radius)* m_scaleValue, x0, y0);
	if (//!((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(m_robotStartX, m_robotStartY, newXs, newYs) &&
		(check1=checkMostEdgeSafeSpace(x0, y0, 0, polarity, fabs(radius) + 0.35, mostEdge)) &&
		(check2=checkMostEdgeSafeSpace(x0, y0, 0, polarity, fabs(radius) - 0.3)) &&
		(check3=checkMostEdgeSafeSpace(x0, y0, 0, polarity, fabs(radius) )))
	{
		m_repathFound = true;
		return true;
	}

	return false;
}

bool UpdatedObstacleAvoidance::checkMostEdgeSafeSpace( int x0,int y0,int shift, int polarity, float radius, bool checkWide)
{
	int x, y,prevX,prevY;
	bool checkResult;
	((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(x0 - m_nScrollDeltaX, y0 - m_nScrollDeltaY),
		CPoint(x0+2 - m_nScrollDeltaX, y0+2 - m_nScrollDeltaY), RGB(0, 0, 255));
	CAutoNavPathPlanView::GetPoint(x0, y0, m_absoluteAngle + shift + 180, radius* m_scaleValue, prevX, prevY);

	for (int angle = 5; angle <= 80; angle = angle + 5)
	{
		CAutoNavPathPlanView::GetPoint(x0, y0, m_absoluteAngle +shift+180 + polarity*angle, radius* m_scaleValue, x, y);
		if (checkWide)
			checkResult=((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(prevX, prevY, x, y, 8/gridWidth);
		else
			checkResult=((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(prevX, prevY, x, y);
		if (checkResult)
		{
			((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(prevX - m_nScrollDeltaX, prevY - m_nScrollDeltaY),
				CPoint(x - m_nScrollDeltaX, y - m_nScrollDeltaY), RGB(255, 0, 0));

			return false;
		}
		((CAutoNavPathPlanView*)m_pView)->DrawLine(pdcRange, CPoint(prevX - m_nScrollDeltaX, prevY - m_nScrollDeltaY),
			CPoint(x - m_nScrollDeltaX, y - m_nScrollDeltaY), RGB(0, 255, 0));

		prevX=x;
		prevY=y;
	}

	m_repathFound = true;
	return true;
}

bool UpdatedObstacleAvoidance::FindSecurePathInObstacleAvoidance(
	int& newXs, int& newYs, int& newXf, int& newYf)
{
	//((CAutoNavPathPlanView*)m_pView)->setAStar(true);
	if (securePoints.size() == 0) return false;
	std::vector<UpdatedObstacleAvoidance::OBS_PIXEL>::iterator it = securePoints.begin();
	bool mostEdge;
	for (int idx = 0; it != securePoints.end(); ++it, ++idx)
	{
		if (it->width==0)
			mostEdge = true;
		else
			mostEdge = false;
		int itx, ity;
		itx = it->x;
		ity = it->y + 0.5*m_scaleValue;
		if (rePathObstacleCheck(itx, ity, newXs, newYs, newXf, newYf, mostEdge))
		{
			//((CAutoNavPathPlanView*)m_pView)->setAStar(false);
			return true;
		}

	}
	//((CAutoNavPathPlanView*)m_pView)->setAStar(false);
	return false;
}
#if 1
bool UpdatedObstacleAvoidance::RepathPointsOnMap(
	int& newXs, int& newYs, int& newXf, int& newYf)
{
	//((CAutoNavPathPlanView*)m_pView)->setAStar(true);
	int itx = 1.0*(float)m_scaleValue, ity = itx;
	for (int idx = 0; idx < 2; idx++)
	{
		if (idx == 1)
			itx = -itx;
		if (rePathObstacleCheck(itx, ity, newXs, newYs, newXf, newYf, true))
		{
			//((CAutoNavPathPlanView*)m_pView)->setAStar(false);
			return true;
		}
	}
	//((CAutoNavPathPlanView*)m_pView)->setAStar(false);
	return false;
}
#else
float UpdatedObstacleAvoidance::RepathPointsOnMap(
	int& xs, int& ys, int& xf, int& yf)
{
	bool rtValue = false;

	int minradius = 8;// (1.2 - 0.4)*(float)(m_scaleValue / gridWidth);
	int maxradius = (1.2 + 0.4)*(float)m_scaleValue / gridWidth;
#if 0
	float angle[8] = { 22.5,157.5,45,135,67.5,112.5,90,90 };
	float turnAnle[8] = { -22.5, 22.5, -45,45,-67.5,67.5,-90,90 };
	int quater[8] = { 1,2,1,2,1,2,1,2 };
#else
	float angle[4] = { 45,135 };// 90, 90
	float turnAngle[4] = { 45,-45 };// -90, 90 };
	int quater[4] = { 45,-45 };// 90, -90 };// { 1, 2 };
#endif
	for (int i = 0; i < 4; i++)
	{
		double mySin = sin(angle[i] * M_PI / 180), myCos = cos(angle[i] * M_PI / 180);
		int x0 = (int)round(-myCos*1.2*(float)m_scaleValue); // original circuit center
		int y0 = (int)round(mySin*1.2*(float)m_scaleValue);
		int newX0, newY0;
		CoordinateTransfer(-m_absoluteAngle - 90, m_robotStartX, m_robotStartY,
			x0, -y0,
			newX0, newY0);
		bool findObstacle = false;
		for (int radius = minradius; radius <= maxradius; radius++)
		{
			m_obstacleCheckStart = true;  // start to check obstacle
			m_obstacleExistInMap = 0;
			// bug is here, should make coordinate convert
			((CAutoNavPathPlanView*)m_pView)->BresCircle(
				(int)round((float)newX0 / gridWidth),
				(int)round((float)newY0 / gridWidth),
				radius,
				quater[i]);
			if (m_obstacleExistInMap == 1)
				findObstacle = true;
			if (findObstacle)
				break;
		}
		if (!findObstacle) // it is the free space which robot wants
		{
			int xs0 = (int)round((float)(mySin*1.2*(float)m_scaleValue));
			int ys0 = (int)round((float)(myCos*1.2*(float)m_scaleValue));
			CoordinateTransfer(-m_absoluteAngle - 90, m_robotStartX, m_robotStartY,
				xs0, -ys0,
				xs, ys);
			CAutoNavPathPlanView::GetPoint(xs, ys, m_absoluteAngle - turnAngle[i], 120 * m_scaleValue / 100, xf, yf);
			if (((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(m_robotStartX, m_robotStartY, xs, ys) ||
				((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(xs, ys, xf, yf))
				continue;
			m_obstacleExistInMap = 0;
			m_obstacleCheckStart = false;
			m_repathFound = true;
			return angle[i];
		}
	}
	m_obstacleExistInMap = 0;
	m_obstacleCheckStart = false;
	return 0;
}
#endif
bool UpdatedObstacleAvoidance::CheckObstacleIsWall(int x, int y, int robotAngle, int obsWitdh)
{
	/*int x1, y1, x2, y2;

	CAutoNavPathPlanView::GetPoint(x, y , robotAngle + 90, obsWitdh / 2, x1, y1);
	CAutoNavPathPlanView::GetPoint(x, y, robotAngle - 90, obsWitdh / 2, x2, y2);

	if (((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(x1, y1, x2, y2))
	return true;
	else
	return false;*/
	bool rtValue = false;
	m_obstacleCheckStart = true;  // start to check obstacle
	m_obstacleExistInMap = 0;
	//if obstacle exists in the range of radium of 25cm, then it indicates it already exists in the wall
	((CAutoNavPathPlanView*)m_pView)->BresCircle((int)round((float)x / gridWidth), (int)round((float)y / gridWidth), m_scaleValue / (8 * gridWidth));
	rtValue = (m_obstacleExistInMap == 1);
	m_obstacleExistInMap = 0;
	m_obstacleCheckStart = false;
	return rtValue;
}

void UpdatedObstacleAvoidance::markObstalceOnMap()
{
	byte recFlag = pSensorDataAccessor->CheckDataFlag();
	//((CAutoNavPathPlanView*)m_pView)->setAStar(true);
	//if ((recFlag != m_currFlag) && (recFlag!=0)) // for test on on Oct 26, 2017
	{
		m_currFlag = recFlag;
		for (int i = 0; i < pSensorDataAccessor->pSensorData->head.nObst; i++)
		{
			if (pSensorDataAccessor->pSensorData->obs[i].x > 4.0 ||
				pSensorDataAccessor->pSensorData->obs[i].y > 4.0 ||
				pSensorDataAccessor->pSensorData->obs[i].width > 3.0) // stephen temporary add for test on Nov3, if depth added, it will be removed
				continue;
			int obsX = (int)round(pSensorDataAccessor->pSensorData->obs[i].x*m_scaleValue);
			int obsY = (int)round(pSensorDataAccessor->pSensorData->obs[i].y*m_scaleValue);
			int obsWidth = (int)round(pSensorDataAccessor->pSensorData->obs[i].width*m_scaleValue);
			int obsAngle = (int)round(atan2(pSensorDataAccessor->pSensorData->obs[i].y,
				pSensorDataAccessor->pSensorData->obs[i].x) * 180 / M_PI);

			int newX = 0, newY = 0, newAngle = 0;
			newAngle = -m_absoluteAngle - 90;
			CoordinateTransfer(newAngle, m_robotStartX, m_robotStartY,
				obsX, -obsY,
				newX, newY);
			//markOneObstacle(newX, newY, m_absoluteAngle +90 - obsAngle, obsWidth); //for test
			m_obsData.push_back(OBS_PIXEL(obsX, obsY, obsWidth /*+ m_scaleValue/4*/));
			if (CheckObstacleIsWall(newX, newY, obsAngle, obsWidth))// ignore the obstacle exists in the map
			{
				markOneObstacle(newX, newY, m_absoluteAngle/*+ obsAngle*/, m_scaleValue / 8);
				continue;
			}
			m_obstacleCheckStart = false;
			if (obsWidth<m_scaleValue)
				((CAutoNavPathPlanView*)m_pView)->BresCircle((int)round((float)newX / gridWidth), (int)round((float)newY / gridWidth), obsWidth / (2 * gridWidth));
			// only mark bar on the direct front of robot
			//if (fabs((float)obsX) < 1.5*(float)m_scaleValue / 2 && (float)obsY < 2.0*(float)m_scaleValue / 2)
				//markOneObstacle(newX, newY, m_absoluteAngle/*+ obsAngle*/, obsWidth + m_scaleValue / 4);
		}
		recordFrontObstacle();
		//((CAutoNavPathPlanView*)m_pView)->setAStar(false);
#if 0 // wall will be used later
		for (int i = 0; i < pSensorDataAccessor->pSensorData->head.nWall; i++)
		{
			int startX = round(pSensorDataAccessor->pSensorData->wall[i].xStart*m_scaleValue);
			int startY = round(pSensorDataAccessor->pSensorData->wall[i].yStart*m_scaleValue);
			int wallAngle = (int)round(atan2(pSensorDataAccessor->pSensorData->wall[i].yDirection,
				pSensorDataAccessor->pSensorData->wall[i].xDirection) * 180 / M_PI);
			int wallLen = round(pSensorDataAccessor->pSensorData->wall[i].length*m_scaleValue);

			int newX = 0, newY = 0, newAngle = 0;
			newAngle = -m_absoluteAngle - 90;
			CoordinateTransfer(newAngle, m_robotStartX, m_robotStartY,
				startX, -startY,
				newX, newY);
			markOneWall(newX, newY, m_absoluteAngle - wallAngle, wallLen);
		}
#endif
	}
}

void UpdatedObstacleAvoidance::markOneObstacle(int x, int y, int robotAngle, int obsWitdh, int obsDepth)
{
	int gridX1, gridY1, gridX2, gridY2;

	CAutoNavPathPlanView::GetPoint((int)round((float)x / gridWidth), (int)round((float)y / gridWidth), robotAngle + 90, obsWitdh / (2 * gridWidth), gridX1, gridY1);
	CAutoNavPathPlanView::GetPoint((int)round((float)x / gridWidth), (int)round((float)y / gridWidth), robotAngle - 90, obsWitdh / (2 * gridWidth), gridX2, gridY2);

	if (obsDepth)
	{
		int gridX3, gridY3, gridX4, gridY4;
		CAutoNavPathPlanView::GetPoint(gridX1, gridY1, robotAngle - 180, obsDepth / gridWidth, gridX3, gridY3);
		CAutoNavPathPlanView::GetPoint(gridX2, gridY2, robotAngle, obsDepth / gridWidth, gridX4, gridY4);
		((CAutoNavPathPlanView*)m_pView)->BresenhamLine(gridX3, gridY3, gridX4, gridY4, 3);
	}
	else
		((CAutoNavPathPlanView*)m_pView)->BresenhamLine(gridX1, gridY1, gridX2, gridY2, 2);
	//int obstacleAngle = (int)round((float)atan2(gridY2 - gridY1, gridX2 - gridX1) * 180 / M_PI);
	//((CAutoNavPathPlanView*)m_pView)->markExtraGridForFindingWidePath(gridX1, gridY1, gridX2, gridY2, obstacleAngle);
}

void UpdatedObstacleAvoidance::markOneWall(int startX, int startY, int wallAngle, int wallLen)
{
	int gridStartX = (int)round((float)startX / gridWidth);
	int gridStartY = (int)round((float)startY / gridWidth);
	int gridEndX, gridEndY;

	CAutoNavPathPlanView::GetPoint(gridStartX, gridStartY, wallAngle, (int)round((float)wallLen / gridWidth), gridEndX, gridEndY);

	((CAutoNavPathPlanView*)m_pView)->BresenhamLine(gridStartX, gridStartY, gridEndX, gridEndY, WALL_TYPE);

}

void UpdatedObstacleAvoidance::clearObstacleData()
{
	m_obsData.clear();
	securePoints.clear();
}

void UpdatedObstacleAvoidance::shiftRobotStartXY()
{
	int newAngle = -m_absoluteAngle - 90;
	int newX, newY;
	CoordinateTransfer(newAngle, m_robotStartX, m_robotStartY,
		0, 0,
		newX, newY);
	CoordinateTransfer(newAngle, m_robotStartX, m_robotStartY,
		m_front_obsX, 0,
		newX, newY);
	m_robotStartX = newX;
	m_robotStartY = newY;
}

void UpdatedObstacleAvoidance::recordFrontObstacle()
{
	if (m_obsData.size() == 0)
	{
		m_existingFrontObs.x = m_existingFrontObs.y = -1;
		m_front_obsX = 0;
		return;
	}
	std::sort(m_obsData.begin(), m_obsData.end(), AbsLess);
	m_front_obsX = m_obsData.begin()->x;
	//shiftRobotStartXY();
	int newX = 0, newY = 0, newAngle = 0;
	newAngle = -m_absoluteAngle - 90;
	CoordinateTransfer(newAngle, m_robotStartX, m_robotStartY,
		m_obsData[0].x, -m_obsData[0].y,
		newX, newY);
	m_existingFrontObs = OBS_PIXEL(newX, newY, m_obsData[0].width);
}
bool UpdatedObstacleAvoidance::checkPreviousObstacleStillExist(int currAngle, int currX, int currY, int currStatus)
{
	return true; // ignore obstacle disappear in re-path for dynamic OA test
	const int ROTATION_MOVE_START_STATUS = 2;
	if (m_existingFrontObs.x == -1 && m_existingFrontObs.y == -1)
		return true; // initial value, obstacle still exist by default
	if (m_bCheckObstacleExistsIsDone) // already checked in one re-path procedure, then we say obstacle still exists and skip new re-path
		return true;
	// need further test
	if (abs(currAngle - m_absoluteAngle) > 50)
		return true;
	for (int i = 0; i < pSensorDataAccessor->pSensorData->head.nObst; i++)
	{
		if (pSensorDataAccessor->pSensorData->obs[i].x > 2.0 ||
			pSensorDataAccessor->pSensorData->obs[i].y > 2.0 ||
			pSensorDataAccessor->pSensorData->obs[i].width > 2.0) // stephen temporary add for test on Nov3, if depth added, it will be removed
			continue;
		int obsX = (int)round(pSensorDataAccessor->pSensorData->obs[i].x*m_scaleValue);
		int obsY = (int)round(pSensorDataAccessor->pSensorData->obs[i].y*m_scaleValue);
		int obsWidth = (int)round(pSensorDataAccessor->pSensorData->obs[i].width*m_scaleValue);
		int obsAngle = (int)round(atan2(pSensorDataAccessor->pSensorData->obs[i].y,
			pSensorDataAccessor->pSensorData->obs[i].x) * 180 / M_PI);

		if (currStatus == ROTATION_MOVE_START_STATUS)
		{
			//currX = m_robotStartX;
			//currY = m_robotStartY;
		}
		int newX = 0, newY = 0;
		CoordinateTransfer(-currAngle - 90, currX, currY,
			obsX, -obsY,
			newX, newY);

		if (((CAutoNavPathPlanView*)m_pView)->calculateDistance(newX, newY,
			m_existingFrontObs.x, m_existingFrontObs.y) <= 0.5 * (float)m_scaleValue)
			return true;
	}
	m_bCheckObstacleExistsIsDone = true;
	return false;
}

bool UpdatedObstacleAvoidance::markObstalceOnMapInPath(int currAngle, int currX, int currY)
{
	bool rePathTrigger = false;
	for (int i = 0; i < pSensorDataAccessor->pSensorData->head.nObst; i++)
	{
		if (fabs(pSensorDataAccessor->pSensorData->obs[i].x) > 4.0 ||
			pSensorDataAccessor->pSensorData->obs[i].y > 4.0 ||
			pSensorDataAccessor->pSensorData->obs[i].width > 3.0) // stephen temporary add for test on Nov3, if depth added, it will be removed
			continue;
		if (fabs(pSensorDataAccessor->pSensorData->obs[i].x) <= 1.1 &&
			//pSensorDataAccessor->pSensorData->obs[i].x >= 0.8 &&
			pSensorDataAccessor->pSensorData->obs[i].y <= 2.0 &&
			pSensorDataAccessor->pSensorData->obs[i].y >= 0.8)
			//pSensorDataAccessor->pSensorData->obs[i].width <= 1.0) // stephen temporary add for test on Nov3, if depth added, it will be removed
			rePathTrigger = true;

		int obsX = (int)round(pSensorDataAccessor->pSensorData->obs[i].x*m_scaleValue);
		int obsY = (int)round(pSensorDataAccessor->pSensorData->obs[i].y*m_scaleValue);
		int obsWidth;
		if (fabs((float)obsX) < 1.5*(float)m_scaleValue / 2)
		    obsWidth = (int)round(pSensorDataAccessor->pSensorData->obs[i].width*m_scaleValue);
		else
			obsWidth = (int)round(pSensorDataAccessor->pSensorData->obs[i].width*0.9*m_scaleValue);
		int obsAngle = (int)round(atan2(pSensorDataAccessor->pSensorData->obs[i].y,
			pSensorDataAccessor->pSensorData->obs[i].x) * 180 / M_PI);

		int newX = 0, newY = 0, newAngle = 0;
		newAngle = -currAngle - 90;
		CoordinateTransfer(newAngle, currX, currY,
			obsX, -obsY,
			newX, newY);

		m_obsData.push_back(OBS_PIXEL(obsX, obsY, obsWidth /*+ m_scaleValue/4*/));
		if (CheckObstacleIsWall(newX, newY, obsAngle, obsWidth))// ignore the obstacle exists in the map
		{
			markOneObstacle(newX, newY, currAngle, m_scaleValue / 8);
			continue;
		}
		if (obsWidth<m_scaleValue)
			((CAutoNavPathPlanView*)m_pView)->BresCircle((int)round((float)newX / gridWidth), (int)round((float)newY / gridWidth), obsWidth / (2 * gridWidth));
		// only mark bar on the direct front of robot
		if (fabs((float)obsX) < 1.5*(float)m_scaleValue / 2 && (float)obsY < 2.0*(float)m_scaleValue / 2)
			markOneObstacle(newX, newY, currAngle, obsWidth + m_scaleValue / 4);
	}
	if (m_obsData.size() > 0)
	{
		//recordFrontObstacleInPath(currAngle, currX, currY);
		return rePathTrigger;
	}
	else
		return false;

}

bool UpdatedObstacleAvoidance::checkObstacleAppearOnPath(vector<AStar_Algo::coord> paths, int currAngle, int currX, int currY)
{
	return false;
	if (paths.size() == 0)
		return false;
	size_t pathSize = paths.size();
	if (m_currentMoveStep >= pathSize)
		return false;
	if (m_bNoStopRepathIsDoing)
		return false;

	((CAutoNavPathPlanView*)m_pView)->cleanObstacleOnMap();
	clearObstacleData();
	if (!markObstalceOnMapInPath(currAngle, currX, currY))
		return false;
	if (((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(currX, currY, paths[m_currentMoveStep].x, paths[m_currentMoveStep].y, 0))
	{
		//m_bLongDistanceRepath = true;
		m_bCheckObstacleExistsIsDone = false;
		return true;
	}
	/*for (size_t i = m_currentMoveStep; i < pathSize - 1; ++i)
	{
		if (((CAutoNavPathPlanView*)m_pView)->twoPointLinesHasObstacle(paths[i].x, paths[i].y, paths[i + 1].x, paths[i + 1].y, 0))
		{
			//m_bLongDistanceRepath = true;
			m_bCheckObstacleExistsIsDone = false;
			return true;
		}
	}*/
	return false;
}

