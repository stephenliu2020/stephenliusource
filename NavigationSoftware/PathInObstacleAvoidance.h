/*******************************************************************/
/**************  PathInObstacleAvoidance.h    **********************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include <vector>
#include <set>
#include "AStar_Algo.h"
#include <opencv2/opencv.hpp>
using namespace cv;
class CPathInObstacleAvoidance
{
public:
  CPathInObstacleAvoidance();
  void markOneBlockForUltrasonic(int gridX, int gridY, int i);
  void markExtraGridForFindingWidePath(int gridX1, int gridY1, 
	int gridX2, int gridY2, int angle);
  void markObstacleForOneUltraSensor(int x0, int y0, int pixelDistance, char detectAngle, int radium);
  bool RepathPointsOnMap(int x0, int y0,
	int& xs, int& ys, int& xf, int& yf);
  void markRetangleOnMap();
  void cleanObstacleOnMap(bool keepObstacleShow);
  void markUSIRObstalceOnMap();
  void cleanUSIRObstalceOnMap();
  Mat rotate(Mat src, float angle);
  Mat decompressView(unsigned short data[16], float angle, float pxPerMeter);
  void markRealSenseObstalceOnMap();
private:
  AStar_Algo* m_astar;
  bool m_bSetExtraVirtualWall;
  int m_wallBlkNum;
  vector<CPoint> m_blocks;
  int m_scaleValue;
  vector<CPoint> m_virWalls;
  int m_absoluteAngle;
  int dcRange;
  int* pDC;
}

