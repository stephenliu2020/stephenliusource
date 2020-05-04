/*******************************************************************/
/**************  RobotMoveController.h  ****************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

class CRobotMoveController
{
public:
  void HandleCurveLine();
  void HandleCurveSegmentsInBigCurveStep();
  void sendDirectLineMessage();
  void HandleDirectLineTimer();
  void sendCurveMessage();
  void HandleCurveLineTimer();
  void sendRotationMessage();
  void HandleRotationTimer();
  void HandleRouteTraceTimer();
  void sendSplitDirectLineMove();
  void sendSplitCurveLineMove(double movingDistance);
  void sendSplitRotationMove(UINT rotationAngle);
  void calculateThreeSegments(int startA, double wholeDistance, double& direct1stSegment, double& arc2ndSegmentRadius, double& arc3rdSegmentRadius);
  void OnRobotMoving(long int wpara, long int lpara);
  void OnTimer(UINT_PTR nIDEvent);
private:
  void * pDC;
  float m_absoluteAngle;
  float m_gaussPrevAngle;
  int m_kinectWallShiftX;
  int m_kinectWallShiftY;
  float m_gauss_wholeDistance;
  float m_direct1stSegmentDistance;
  int m_gauss_startM;
  int m_direct1stSegmentDistance;
  int m_gauss_startX;
  int m_gauss_startY;
  int m_segmentType;
  bool m_1stSegmentDirectIsDone;
  bool m_2ndSegmentCurveIsDone;
}

