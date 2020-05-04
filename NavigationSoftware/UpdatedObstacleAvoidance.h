/*******************************************************************/
/**************  UpdatedObstacleAvoidance.h   **********************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#pragma once
#include "SensorsSharedMemAccess.h"
#include <vector>
const int ROBOT_MOVING_SECURE_WIDTH = 48;// 64;// 80; //1 meter
class UpdatedObstacleAvoidance
{
public:
	UpdatedObstacleAvoidance(int gWidth, int scale, SensorsSharedMemAccess* pSensorAccessor);
	~UpdatedObstacleAvoidance();
	enum {OBSTACLE_TYPE=0, WALL_TYPE};
	struct OBS_PIXEL
	{
		OBS_PIXEL() :x(0), y(0), width(0) {};
		OBS_PIXEL(int _x, int _y, int _w) :x(_x), y(_y), width(_w) {};
		int x;
		int y;
		int width;
	};
	struct
	{
		bool operator()(const OBS_PIXEL& lhs, const OBS_PIXEL& rhs)
		{
			return lhs.x < rhs.x;
		}
	} ColumnLess;
	struct
	{
		bool operator()(const OBS_PIXEL& lhs, const OBS_PIXEL& rhs)
		{
			return abs(lhs.x) < abs(rhs.x);
		}
	} AbsLess;
	struct
	{
		bool operator()(const OBS_PIXEL& lhs, const OBS_PIXEL& rhs)
		{
			if (lhs.x*rhs.x<0) // if 2 obstacles on 2 sides, high width is first
				return abs(lhs.width) > abs(rhs.width);
			else
				return abs(lhs.x) < abs(rhs.x);
		}
	} SafeLess;
	void setRobotPose(int x, int y, int robotAngle);
	void markObstalceOnMap();
	bool markObstalceOnMapInPath(int currAngle, int currX, int currY);
	bool checkPreviousObstacleStillExist(int currAngle, int currX, int currY, int currStatus);
	void clearObstacleData();
	void getSaftyMovingPoint();
	bool FindSecurePathInObstacleAvoidance(
		int& newXs, int& newYs, int& newXf, int& newYf);
	bool RepathPointsOnMap(int& xs, int& ys, int& xf, int& yf);
	void setTestPara(CDC* rangeDC, int testX, int testY);
	bool checkObstacleAppearOnPath(vector<AStar_Algo::coord> paths, int currAngle, int currX, int currY);
	void setGridWidth(int gWidth) { gridWidth = gWidth; }
	std::vector<OBS_PIXEL> securePoints;
	int m_obstacleExistInMap;
	bool m_obstacleCheckStart;
	bool m_repathFound;
	bool m_bCheckObstacleExistsIsDone;
	CDC* pdcRange;
	int m_nScrollDeltaX;
	int m_nScrollDeltaY;
	bool m_bNoStopRepathIsDoing;
	int m_currentMoveStep;
private:
	void markOneObstacle(int x, int y, int robotAngle, int obsWidth, int obsDepth=0);
	void markOneWall(int startX, int startY, int wallAngle, int wallLen);
	void safePointCoordinateTransfer(int itx, int ity,
		int& newXs, int& newYs, int& newXf, int& newYf,
		int& newXs1, int& newYs1, int& newXf1, int& newYf1,
		int& newXs2, int& newYs2, int& newXf2, int& newYf2,
		int& newXs3, int& newYs3, int& newXf3, int& newYf3);
	bool rePathObstacleCheck(int itx, int ity, int& newXs, int& newYs, int& newXf, int& newYf, bool mostEdge=false);
	void CoordinateTransfer(int angle, int a, int b, int xIn, int zIn, int& x, int& y);
	bool CheckObstacleIsWall(int x, int y, int robotAngle, int obsWitdh);
	void recordFrontObstacle();
	bool checkMostEdgeSafeSpace(int x0, int y0, int shift, int polarity, float radius, bool checkWide=false);
	void setNewRobotPose(int rotateAngle,int& newRobotStartX, int& newRobotStartY);
	void shiftRobotStartXY();
	int gridWidth;
	int m_absoluteAngle;
	int m_robotStartX;
	int m_robotStartY;
	int m_scaleValue;
	byte m_currFlag;
	SensorsSharedMemAccess* pSensorDataAccessor;
	CView* m_pView;
	std::vector<OBS_PIXEL> m_obsData;
	int m_front_obsX;
	OBS_PIXEL m_existingFrontObs;
};

