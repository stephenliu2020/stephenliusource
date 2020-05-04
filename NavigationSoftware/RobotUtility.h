/*******************************************************************/
/**************       RobotUtility.h    ****************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

class CRobotUtility
{
public:
  void Swap(int& a, int& b);
  float calculateDistance(const float& x1, 
	const float& y1, const float& x2, const float& y2);
  float CRobotUtility::float_swap(float value);
  int FindLastIndexOf(const CString& s1, const CString& s2);
  float CRobotUtility::mag(float x, float y);
  void unit(float& x, float& y);
  void rotate_vector(float& ux, float& uy, float theta);
  float calculateDifferenceBetweenAngles(float firstAngle, float secondAngle);
}

