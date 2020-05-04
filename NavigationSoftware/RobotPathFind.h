/*******************************************************************/
/**************       RobotPathFind.h   ****************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include <vector>
#include <set>
#include "AStar_Algo.h"
using namespace std;
class CRobotPathFind
{
public:
  bool isGridObstacle(int x, int y);
  bool isGridObstacle(int x, int y, int depth);
  bool twoPointLinesHasObstacle(int x1, int y1, int x2, int y2);
  bool twoPointLinesHasObstacle(int x1, int y1, int x2, int y2, int depth);
  void PathFind();
  bool PathFindUsingAStar(int startX, int startY, int endX, int endY,
	int preAngle, vector<AStar_Algo::coord>& mergedPaths);
  bool getClosestRoutePoint(int x, int y, int& closestX, int& closestY);
  int FindDirectLineNext(int x, int y, int initialPos);
  int FindDirectLineNext();
  void SecondPathAmend(vector<AStar_Algo::coord>& finalPaths);
  void FinalPathAmend();
  void AmendPath();
  bool isObstacleForPathAmendPoint(int x, int y, int amendSize,
	int xMultiplyFactor, int yMultiplyFactor);
  bool offsetToNewPosition(int x, int y, int amendSize,
	int xMultiplyFactor, int yMultiplyFactor, int& newX, int& newY);
  bool AmendPathPoint(int x0, int y0, int angle, int& newX, int& newY);
  void cleanRawAStarFinalPaths();
  void cleanFinalPaths();
private:
  AStar_Algo* m_astar;
  int m_destX;
  int m_destY;
  int m_prevX;
  int m_currX;
  int m_prevY;
  int m_currY;
  CPoint m_targetNavPoint;
  vector<CPoint> m_routePoints;
  vector<CPoint> m_tempRoutePoints;
  int m_routePointsNumBySetting;
  int dcRange;
}

