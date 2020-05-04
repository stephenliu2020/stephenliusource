/*******************************************************************/
/************** RobotLogFile.cpp ***********************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include "RobotLogFile.h"

CRobotLogFile::CRobotLogFile(string _fileName):fileName(_fileName)
{
}

void CRobotLogFile::saveStatusIntoFile(int x, int y)
{
	FILE* fp = fopen(fileName.c_str(), "w+");
	if (fp == NULL) return;
	fprintf(fp, "%d,%d\n", x, y);
	fclose(fp);
}

void CRobotLogFile::saveDoorRoutePointsIntoFile(int x, int y)
{
	FILE* fp;
	fp = fopen(fileName.c_str(), "a");
	if (fp == NULL) return;
	fprintf(fp, "%d,%d\n", x, y);
	fclose(fp);
}

void CRobotLogFile::saveVirtualWallIntoFile(int x, int y, int blkNum)
{
	m_virWallBlkNum.push_back(blkNum);
	FILE* fp;
	fp = fopen(fileName.c_str(), "a");
	if (fp == NULL) return;
	fprintf(fp, "%d,%d,%d\n", x, y, blkNum);
	fclose(fp);

}

void CRobotLogFile::writeLogInfoIntoFile(char* str, int len)
{
	m_fp = fopen(fileName.c_str(), "a");
	fwrite(str, len, 1, m_fp);
	fclose(m_fp);
}

void CRobotLogFile::saveChargerStationPosition()
{
	FILE* fp = fopen(fileName.c_str(), "w+");
	if (fp == NULL) return;
	float x = (float)(m_chargeStationPoint.x - m_bmWidth / 2) / m_scaleValue;
	float y = (float)(m_bmHeight / 2 - m_chargeStationPoint.y) / m_scaleValue;
	fprintf(fp, "%f,%f\n", x, y);
	fclose(fp);
}

bool CRobotLogFile::loadChargerStationPosition()
{
	FILE* fp = fopen(fileName.c_str(), "r");
	if (fp == NULL) return false;
	float x, y;
	if (fscanf(fp, "%f,%f\n", &x, &y) != EOF)
	{
		m_chargeStationPoint.x = m_bmWidth / 2 + x*m_scaleValue;
		m_chargeStationPoint.y = m_bmHeight / 2 - y*m_scaleValue;
		DrawChargerStation(&dcMemory, m_chargeStationPoint.x, m_chargeStationPoint.y, RGB(0, 0, 255), 16);
	}
	fclose(fp);
	return true;
}



