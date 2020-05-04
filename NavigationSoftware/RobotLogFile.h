/*******************************************************************/
/************** RobotLogFile.h ***********************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

class CRobotLogFile
{
public:
  CRobotLogFile(string _fileName);
  void saveStatusIntoFile(int x, int y);
  void saveDoorRoutePointsIntoFile(int x, int y);
  void saveVirtualWallIntoFile(int x, int y, int blkNum);
  void saveChargerStationPosition();
  bool loadChargerStationPosition();
private:
  string fileName;
}

