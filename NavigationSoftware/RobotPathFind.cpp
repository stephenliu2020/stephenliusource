/*******************************************************************/
/**************       RobotPathFind.cpp ****************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include "RobotPathFind.h"
#include <mmsystem.h>//playsound

bool CRobotPathFind::isGridObstacle(int x, int y)
{
#if 1
	if (m_astar->isObstacle((int)round((float)x / m_astar->getGridWidth()),
		(int)round((float)y / m_astar->getGridWidth())))
		return true;
	else
		return false;

#else
	int gridX = (int)round((float)x / m_astar->getGridWidth());
	int gridY = (int)round((float)y / m_astar->getGridWidth());
	int shift[][2] = { {0,0},{0,1},{1,0},{-1,0},{0,-1},
	{-1,-1},{1,1},{-1,1},{1,-1} };
	for (int i = 0; i < 9; ++i)
	{
		if (m_astar->isObstacle(gridX+ shift[i][0], gridY+shift[i][1]))
			return true;
	}
	return false;
#endif
}

bool CRobotPathFind::isGridObstacle(int x, int y, int depth)
{
	int gridX = (int)round((float)x / m_astar->getGridWidth());
	int gridY = (int)round((float)y / m_astar->getGridWidth());

	for (int i = -depth; i <= depth; ++i)
	{
		for (int j = -depth; j <= depth; ++j)
		{
			if (m_astar->isObstacle(gridX+i, gridY+j))
				return true;
		}
	}
	return false;
}
bool CRobotPathFind::twoPointLinesHasObstacle(int x1, int y1, int x2, int y2)
{
	int iTag = 0;
	int dx, dy, tx, ty, inc1, inc2, d, curx, cury;

	if (isGridObstacle(x1, y1)) return true;

	if (x1 == x2 && y1 == y2)
		return true;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	if (dx<dy)
	{
		iTag = 1;
		Swap(x1, y1);
		Swap(x2, y2);
		Swap(dx, dy);
	}

	tx = (x2 - x1)>0 ? 1 : -1;
	ty = (y2 - y1)>0 ? 1 : -1;

	curx = x1;
	cury = y1;

	inc1 = 2 * dy;
	inc2 = 2 * (dy - dx);

	d = inc1 - dx;

	while (curx != x2)
	{
		curx += tx;
		if (d<0)
		{
			d += inc1;
		}
		else
		{
			cury += ty;
			d += inc2;
		}
		if (iTag)
		{
			if (isGridObstacle(cury, curx)) return true;
		}
		else
		{
			if (isGridObstacle(curx, cury)) return true;
		}
	}
	return false;
}

bool CRobotPathFind::twoPointLinesHasObstacle(int x1, int y1, int x2, int y2, int depth)
{
	int iTag = 0;
	int dx, dy, tx, ty, inc1, inc2, d, curx, cury;

	if (isGridObstacle(x1, y1, depth)) return true;

	if (x1 == x2 && y1 == y2)
		return true;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	if (dx<dy)
	{
		iTag = 1;
		Swap(x1, y1);
		Swap(x2, y2);
		Swap(dx, dy);
	}

	tx = (x2 - x1)>0 ? 1 : -1;
	ty = (y2 - y1)>0 ? 1 : -1;

	curx = x1;
	cury = y1;

	inc1 = 2 * dy;
	inc2 = 2 * (dy - dx);

	d = inc1 - dx;

	while (curx != x2)
	{
		curx += tx;
		if (d<0)
		{
			d += inc1;
		}
		else
		{
			cury += ty;
			d += inc2;
		}
		if (iTag)
		{
			if (isGridObstacle(cury, curx, depth)) return true;
		}
		else
		{
			if (isGridObstacle(curx, cury, depth)) return true;
		}
	}
	return false;
}
void CRobotPathFind::PathFind()
{
#ifdef ASTAR_ALGO1

	if (!m_bSetStartNav || !m_bSetTargetNav) return;
	bool passRoutePoint;
	// TODO: Add your command handler code here
	int closestRoutePointX, closestRoutePointY;
	if (findClosestRoutePoint(closestRoutePointX, closestRoutePointY) && m_isSmallMapSearchDone)
	{
		m_destX = closestRoutePointX;
		m_destY = closestRoutePointY;
		passRoutePoint = true;
	}
	else // route point is not found
	{
		m_destX = m_targetNavPoint.x;
		m_destY = m_targetNavPoint.y;
		passRoutePoint = false;
	}
	if (m_currX == m_destX&&m_currY == m_destY)
	{
		if (!(m_targetNavPoint.x == m_destX&&m_targetNavPoint.y == m_destY))
		{
			char strTest[MAX_STR_LEN];
			sprintf(strTest, "Start point(%d,%d) is the same with the end point(%d,%d) when target(%d,%d)\n", 
				m_currX, m_currY, m_destX, m_destY, m_targetNavPoint.x, m_targetNavPoint.y);
			writeLogInfoIntoFile(strTest, strlen(strTest));
			//AfxMessageBox(_T("Warning:Start point is the same with the end point"));
		}
		return;
	}

	if (m_currX >= 0 && m_currY >= 0 && m_destX >= 0 && m_destY >= 0)
	{
		m_astar->m_paths.clear();
		m_astar->m_mergedPaths.clear();

		m_astar->setStartAndTarget(m_currX / m_astar->getGridWidth(), m_currY / m_astar->getGridWidth(),
			m_destX / m_astar->getGridWidth(), m_destY / m_astar->getGridWidth());
		m_prevX = m_currX;
		m_prevY = m_currY;

		if (!twoPointLinesHasObstacle(m_currX, m_currY, m_destX, m_destY, 2)
			&& m_isSmallMapSearchDone && !m_bDoNotAmendPath)
		{
			//DrawLine(GetDC(), CPoint(m_currX- m_nScrollDeltaX, m_currY- m_nScrollDeltaY), 
			//CPoint(m_destX- m_nScrollDeltaX, m_destY- m_nScrollDeltaY), RGB(255, 165, 0));
			Udp_ClientSocket_tx_directLine(0xABE0, 0xA001, m_destX, m_destY);
			//m_bPathFind = false;
			if (!m_routePoints.empty() || passRoutePoint)
			{
				if (!m_routePointsNumBySetting && !passRoutePoint)
					;
				else
				{
					m_currX = m_destX;
					m_currY = m_destY;
					PathFind();
				}
			}

			// all is done
			if (!m_pathPlanIsDone)
			{
				Udp_Client_send_tx_packet(0x3BAD, 0x6001);
				//if (m_bIsEmergencyState)
				  //cleanObstacleOnMap(true);
			}
			return;
		}
		else
		{
			if (m_astar_smallmap&&!m_isSmallMapSearchDone
				/*&&m_routePoints.size()==0&& m_backRoutePoints.size()==0*/)
			{
				m_isSmallMapSearchDone = true;
				PathFindWithSmallMap();
				//if (m_routePoints.size() > 10) // ignore complicated case
					//m_routePoints.clear();
				if (m_bPathFind)
					StartPathFind();
				return;
			}
		}
		//char strTest[MAX_STR_LEN];
		//sprintf(strTest, "run...\n");
		//writeLogInfoIntoFile(strTest, strlen(strTest));
		m_astar->run();
		//char strTest[MAX_STR_LEN];
	    //sprintf(strTest, "Path len:%d test:%d\n", m_astar->m_paths.size(),test);
		//writeLogInfoIntoFile(strTest, strlen(strTest));
		if (m_astar->m_paths.size() == 0)
		{
			if (m_astar == m_astar_smallmap)
			{
				m_bPathFind = false;
				return;
			}
			//MessageBeep(0x00000010L);
			PlaySound(TEXT("nopath.wav"), NULL, SND_FILENAME);
			//GetDC()->DrawText(_T("Path Not Found"), -1, CRect(100, 930, 300, 950), DT_SINGLELINE);
#if 1
			if (m_bIsEmergencyState)
			{
				DrawLine(&dcRange, CPoint(m_realLocX - m_nScrollDeltaX, m_realLocY - m_nScrollDeltaY),
					CPoint(m_rePathXs - m_nScrollDeltaX, m_rePathYs - m_nScrollDeltaY), RGB(255, 255, 255));
				DrawLine(&dcRange, CPoint(m_rePathXs - m_nScrollDeltaX, m_rePathYs - m_nScrollDeltaY),
					CPoint(m_rePathXf - m_nScrollDeltaX, m_rePathYf - m_nScrollDeltaY), RGB(255, 255, 255));
			}
#endif
			Udp_Client_send_tx_packet(0x3BAD, 0x6001, true);//Stop firstly

			m_bPathFind = false;
			return;
		}

		AStar_Algo::pathIter it = m_astar->m_paths.begin();
		m_astar->m_mergedPaths.push_back(AStar_Algo::coord(it->x, it->y));
		AStar_Algo::pathIter prevIt = it;
		int preAngle = m_absoluteAngle;
		for (it = std::next(it, 1); it != m_astar->m_paths.end(); ++it)
		{
			//GetDC()->SetPixel(it->x * 8, it->y * 8, RGB(255, 0, 0)); 
			int drawX = it->x * m_astar->getGridWidth(), drawY = it->y * m_astar->getGridWidth();
			int prevX = prevIt->x * m_astar->getGridWidth(), prevY = prevIt->y * m_astar->getGridWidth();

			int angle = atan2(drawY - prevY, drawX - prevX) * 180 / M_PI;
			if (preAngle != angle)
			{
				// need rotation and direct line move
				if (std::find(m_astar->m_mergedPaths.begin(), m_astar->m_mergedPaths.end(), AStar_Algo::coord(prevIt->x, prevIt->y)) == m_astar->m_mergedPaths.end())
					m_astar->m_mergedPaths.push_back(AStar_Algo::coord(prevIt->x, prevIt->y));
			}

			prevIt = it;
			preAngle = angle;
		}
		//Invalidate();
	}
#elif defined ASTAR_ALGO2
	path_find(GetDC(), m_startNavPoint.x, m_startNavPoint.y, m_targetNavPoint.x, m_targetNavPoint.y);
	//Invalidate();
#endif
	if (std::find(m_astar->m_mergedPaths.begin(), m_astar->m_mergedPaths.end(),
		m_astar->m_paths.back()) == m_astar->m_mergedPaths.end())
	{
		m_astar->m_mergedPaths.push_back(m_astar->m_paths.back());
	}
	//char strTest[MAX_STR_LEN];
	//sprintf(strTest, "Merged path\n");
	//writeLogInfoIntoFile(strTest, strlen(strTest));
	if (m_currX / m_astar->getGridWidth() == m_astar->m_mergedPaths.begin()->x &&
		m_currY / m_astar->getGridWidth() == m_astar->m_mergedPaths.begin()->y)
		m_astar->m_mergedPaths.erase(m_astar->m_mergedPaths.begin());
	if (m_bDoNotAmendPath)
		Udp_ClientSocket_tx_curve(0xABE0, 0xA001, m_astar->m_paths);
	else
		Udp_ClientSocket_tx_curve(0xABE0, 0xA001, m_astar->m_mergedPaths);
	if (!m_routePoints.empty() || passRoutePoint)
	{
		m_currX = m_destX;
		m_currY = m_destY;
		PathFind();
	}
	// all is done
	if (!m_pathPlanIsDone)
	{
		Udp_Client_send_tx_packet(0x3BAD, 0x6001);
		//if (m_bIsEmergencyState)
			//cleanObstacleOnMap(true);
	}

	//m_bRobotFirstMove = false;
	//m_bPathGotAndExist = true;
/*
	if (m_rePathFinding || m_cloudPathFinding)
	{
		SendMessage(WM_COMMAND,
			MAKEWPARAM(ID_ROBOT_MOVING, BN_CLICKED),
			NULL);
		if (m_rePathFinding)
			m_rePathFinding = false;
		if (m_cloudPathFinding)
			m_cloudPathFinding = false;
	}
*/
}

bool CRobotPathFind::PathFindUsingAStar(int startX, int startY, int endX, int endY,
	int preAngle, vector<AStar_Algo::coord>& mergedPaths)
{
	int newX = -1, newY = -1;
	int grid = m_astar->getGridWidth();
	m_astar->m_paths.clear();
	mergedPaths.clear();
	m_astar->setStartAndTarget(startX / m_astar->getGridWidth(), startY / m_astar->getGridWidth(),
		endX / m_astar->getGridWidth(), endY / m_astar->getGridWidth());
	m_astar->run();
	if (m_astar->m_paths.size() == 0)
	{
		return false;
	}
	AStar_Algo::pathIter it = m_astar->m_paths.begin();
	AmendPathPoint(it->x*grid, it->y*grid, preAngle, newX, newY);
	mergedPaths.push_back(AStar_Algo::coord(newX / grid, newY / grid));
	//DrawPoint(GetDC(), newX - m_nScrollDeltaX, newY - m_nScrollDeltaY, RGB(128, 0, 0), 2);

	AStar_Algo::pathIter prevIt = it;
	for (it = std::next(it, 1); it != m_astar->m_paths.end(); ++it)
	{
		int drawX = it->x * m_astar->getGridWidth(), drawY = it->y * m_astar->getGridWidth();
		int prevX = prevIt->x * m_astar->getGridWidth(), prevY = prevIt->y * m_astar->getGridWidth();

		int angle = atan2(drawY - prevY, drawX - prevX) * 180 / M_PI;
		if (preAngle != angle)
		{
			if (std::find(mergedPaths.begin(), mergedPaths.end(), AStar_Algo::coord(prevIt->x, prevIt->y)) == mergedPaths.end())
			{
				AmendPathPoint(prevIt->x*grid, prevIt->y*grid, angle, newX, newY);
				mergedPaths.push_back(AStar_Algo::coord(newX / grid, newY / grid));
				//DrawPoint(GetDC(), newX - m_nScrollDeltaX, newY - m_nScrollDeltaY, RGB(128, 0, 0), 2);
			}
			//mergedPaths.push_back(AStar_Algo::coord(prevIt->x, prevIt->y));
		}
		else // only need amended point save into merged path
		{
			if (std::find(mergedPaths.begin(), mergedPaths.end(), AStar_Algo::coord(prevIt->x, prevIt->y)) == mergedPaths.end())
			{
				if (AmendPathPoint(prevIt->x*grid, prevIt->y*grid, angle, newX, newY))
				{
					mergedPaths.push_back(AStar_Algo::coord(newX / grid, newY / grid));
					//DrawPoint(GetDC(), newX - m_nScrollDeltaX, newY - m_nScrollDeltaY, RGB(128, 0, 0), 2);
				}
			}
		}

		prevIt = it;
		preAngle = angle;
	}
	if (std::find(mergedPaths.begin(), mergedPaths.end(),
		m_astar->m_paths.back()) == mergedPaths.end())
	{
		AmendPathPoint(m_astar->m_paths.back().x*grid, m_astar->m_paths.back().y*grid, preAngle, newX, newY);
		mergedPaths.push_back(AStar_Algo::coord(newX / grid, newY / grid));
		//DrawPoint(GetDC(), newX - m_nScrollDeltaX, newY - m_nScrollDeltaY, RGB(128, 0, 0), 2);
		//mergedPaths.push_back(m_astar->m_paths.back());
	}
	if (startX / m_astar->getGridWidth() == mergedPaths.begin()->x &&
		startY / m_astar->getGridWidth() == mergedPaths.begin()->y)
		mergedPaths.erase(mergedPaths.begin());
	return true;
}
bool CRobotPathFind::getClosestRoutePoint(int x, int y, int& closestX, int& closestY)
{
	double minDistance, distance;
	minDistance = sqrt((x - m_targetNavPoint.x)*(x - m_targetNavPoint.x)
		+ (y - m_targetNavPoint.y)*(y - m_targetNavPoint.y));
	bool hasRoutePoint = false;
	int pos = 0, closestPos;
	if (m_tempRoutePoints.empty()) return false;
	for (vector<CPoint>::iterator it = m_tempRoutePoints.begin();
		it != m_tempRoutePoints.end(); ++it, ++pos)
	{
		distance = sqrt((x - it->x)*(x - it->x)
			+ (y - it->y)*(y - it->y));
		if (distance < minDistance)
		{
			minDistance = distance;
			closestX = it->x;
			closestY = it->y;
			closestPos = pos;
			hasRoutePoint = true;
		}
	}
	if (hasRoutePoint&&minDistance <= m_scaleValue)
	{
		m_tempRoutePoints.erase(m_tempRoutePoints.begin() + closestPos);
	}
	return hasRoutePoint;
}

int CRobotPathFind::FindDirectLineNext(int x, int y, int initialPos)
{
	vector<AStar_Algo::coord>::reverse_iterator rit = m_astar->m_amendedPaths.rbegin();
	int pos;
	if (m_routePointsNumBySetting)
	{
		int closestX, closestY;
		getClosestRoutePoint(x, y, closestX, closestY);
		vector<CPoint>::iterator it;
		rit = std::find(m_astar->m_amendedPaths.rbegin(),
			m_astar->m_amendedPaths.rend(),
			AStar_Algo::coord(closestX, closestY));
		if (rit == m_astar->m_amendedPaths.rend())
		{
			rit = m_astar->m_amendedPaths.rbegin();
		}
		pos = m_astar->m_amendedPaths.rend() - rit - 1;
	}
	else
		pos = m_astar->m_amendedPaths.size() - 1;
	for (; rit != m_astar->m_amendedPaths.rend() && pos>(initialPos + 1); ++rit, --pos)
	{
		if (!twoPointLinesHasObstacle(x, y, rit->x, rit->y, 1))
		{
			return pos;
		}
	}
	return initialPos;
}

int CRobotPathFind::FindDirectLineNext()
{
	vector<AStar_Algo::coord>::reverse_iterator rit = m_finalPaths.rbegin();
	int pos = m_finalPaths.size() - 1;
	for (; rit != m_finalPaths.rend() && pos>0; ++rit, --pos)
	{
		if (!twoPointLinesHasObstacle(m_realLocX, m_realLocY, rit->x, rit->y, 1))
		{
			return pos;
		}
	}
	return 0;
}

void CRobotPathFind::SecondPathAmend(vector<AStar_Algo::coord>& finalPaths)
{
	int nextPos = 1;
	int x = m_robotStartX, y = m_robotStartY;
	int intialPos = 0;
	//if (m_astar->m_amendedPaths.size() <= 1) return;
	int newX = x, newY = y;
	if ((nextPos = FindDirectLineNext(x, y, 0)) > 0)
	{
		finalPaths.push_back(m_astar->m_amendedPaths[nextPos]);
		newX = m_astar->m_amendedPaths[nextPos].x;
		newY = m_astar->m_amendedPaths[nextPos].y;
		intialPos = nextPos;
	}
	else
	{
		finalPaths.push_back(m_astar->m_amendedPaths[0]);
		newX = m_astar->m_amendedPaths[0].x;
		newY = m_astar->m_amendedPaths[0].y;
		nextPos = 0;
	}
	DrawPoint(GetDC(), newX - m_nScrollDeltaX, newY - m_nScrollDeltaY, RGB(255, 0, 0), 2);
	DrawLine(&dcRange, CPoint(x - m_nScrollDeltaX, y - m_nScrollDeltaY),
		CPoint(newX - m_nScrollDeltaX, newY - m_nScrollDeltaY), RGB(255, 0, 0));
	x = newX; y = newY;
	if (m_astar->m_amendedPaths.size() <= (nextPos + 1)) return;
	vector<AStar_Algo::coord>::iterator it = m_astar->m_amendedPaths.begin() + nextPos;
	while (it != m_astar->m_amendedPaths.end())
	{
		if ((nextPos = FindDirectLineNext(it->x, it->y, intialPos)) > (intialPos + 1))
		{
			//finalPaths.erase(finalPaths.begin()+intialPos+1, finalPaths.begin()+nextPos);
			finalPaths.push_back(m_astar->m_amendedPaths[nextPos]);
			newX = m_astar->m_amendedPaths[nextPos].x;
			newY = m_astar->m_amendedPaths[nextPos].y;
			//DrawPoint(GetDC(), newX - m_nScrollDeltaX, newY - m_nScrollDeltaY, RGB(255, 0, 0), 2);
			DrawLine(&dcRange, CPoint(x - m_nScrollDeltaX, y - m_nScrollDeltaY),
				CPoint(newX - m_nScrollDeltaX, newY - m_nScrollDeltaY), RGB(255, 0, 0));

			it = m_astar->m_amendedPaths.begin() + nextPos;
			intialPos = nextPos;
		}
		else
		{
			finalPaths.push_back(m_astar->m_amendedPaths[intialPos + 1]);
			newX = m_astar->m_amendedPaths[intialPos + 1].x;
			newY = m_astar->m_amendedPaths[intialPos + 1].y;
			//DrawPoint(GetDC(), newX - m_nScrollDeltaX, newY - m_nScrollDeltaY, RGB(255, 0, 0), 2);
			DrawLine(&dcRange, CPoint(x - m_nScrollDeltaX, y - m_nScrollDeltaY),
				CPoint(newX - m_nScrollDeltaX, newY - m_nScrollDeltaY), RGB(255, 0, 0));

			it++;
			intialPos++;
		}
		x = newX;
		y = newY;
		if (x == m_astar->m_amendedPaths.back().x&&
			y == m_astar->m_amendedPaths.back().y) break;
	}

}

void CRobotPathFind::FinalPathAmend() {
	int size = m_finalPaths.size();
	if (size < 2) return;
	//x1,y1 is end point
	int x1= m_finalPaths[size - 1].x, y1= m_finalPaths[size - 1].y;
	int x2= m_finalPaths[size - 2].x, y2= m_finalPaths[size - 2].y;
	double x, y;
	if (m_obstacleAngle%180 == 0)
	{
		x = x2; 
		y = y1;
	}
	else if ((m_obstacleAngle+180) % 180 == 90)
	{
		x = x1;
		y = y2;
	}
	else
	{ 
		double k1 = tan((double)m_obstacleAngle*M_PI / 180.0);
		double k2 = tan((double)(m_obstacleAngle+60)*M_PI / 180.0);
		x = ((double)y2 - y1 + k1*x1 + x2 * k2) / (k1 + k2);
		y = k1*(x - x1) + y1;
	}
	x = (x + x2) / 2; y = (y + y2) / 2;
	if (twoPointLinesHasObstacle(x2, y2, round(x), round(y)))
	{
		//Invalidate();
		return;
	}
	vector<AStar_Algo::coord>::iterator it = m_finalPaths.end();
	m_finalPaths.insert(it - 1, AStar_Algo::coord((int)round(x), (int)round(y)));
	DrawLine(&dcRange, CPoint(x2 - m_nScrollDeltaX, y2 - m_nScrollDeltaY),
		CPoint((int)round(x) - m_nScrollDeltaX, (int)round(y) - m_nScrollDeltaY), RGB(255, 128, 0));
	DrawLine(&dcRange, CPoint((int)round(x) - m_nScrollDeltaX, (int)round(y) - m_nScrollDeltaY),
		CPoint(x1 - m_nScrollDeltaX, y1 - m_nScrollDeltaY), RGB(255, 128, 0));
	DrawLine(&dcRange, CPoint(x2 - m_nScrollDeltaX, y2 - m_nScrollDeltaY),
		CPoint(x1 - m_nScrollDeltaX, y1 - m_nScrollDeltaY), RGB(255, 255, 255));
	//Invalidate();
}

void CRobotPathFind::AmendPath()
{
	vector<int>::iterator itx = m_finalPathsX.begin();
	vector<int>::iterator ity = m_finalPathsY.begin();
	//vector<int>::iterator prevItx=itx, prevIty=ity;

	int prevPrevX, prevPrevY;
	prevPrevX = m_robotStartX;
	prevPrevY = m_robotStartY;
	int prevX = *itx, prevY = *ity;
	int drawX, drawY;
	int preAngle = m_absoluteAngle;
	for (itx = std::next(itx, 1), ity = std::next(ity, 1); itx != m_finalPathsX.end(); ++itx, ++ity)
	{
		drawX = *itx; drawY = *ity;

		int angle = atan2(drawY - prevY, drawX - prevX) * 180 / M_PI;
		int newX = -1, newY = -1;
		if (AmendPathPoint(prevX, prevY, angle, newX, newY))
		{
			//if (std::find(m_astar->m_amendedPaths.begin(), m_astar->m_amendedPaths.end(), AStar_Algo::coord(newX, newY)) == m_astar->m_amendedPaths.end())
			if (!twoPointLinesHasObstacle(prevPrevX, prevPrevY, newX, newY))
			{
				m_astar->m_amendedPaths.push_back(AStar_Algo::coord(newX, newY));
				//DrawPoint(GetDC(), newX - m_nScrollDeltaX, newY - m_nScrollDeltaY, RGB(128, 0, 0), 2);
				//DrawLine(GetDC(), CPoint(prevPrevX - m_nScrollDeltaX, prevPrevY - m_nScrollDeltaY),
					//CPoint(newX - m_nScrollDeltaX, newY - m_nScrollDeltaY), RGB(128, 0, 0));
				prevPrevX = newX;
				prevPrevY = newY;
			}
			else
			{
				vector<AStar_Algo::coord> theMergedPaths;
				if (PathFindUsingAStar(prevPrevX, prevPrevY, newX, newY, preAngle,
					theMergedPaths))
				{
					for (vector<AStar_Algo::coord>::iterator im = theMergedPaths.begin();
						im != theMergedPaths.end(); ++im)
						m_astar->m_amendedPaths.push_back(
							AStar_Algo::coord(im->x*m_astar->getGridWidth(),
								im->y*m_astar->getGridWidth()));
					prevPrevX = theMergedPaths.back().x*m_astar->getGridWidth();
					prevPrevY = theMergedPaths.back().y*m_astar->getGridWidth();
				}
				else
				{
					prevPrevX = newX;
					prevPrevY = newY;
				}
			}
		}
		else
		{
			//if (std::find(m_astar->m_amendedPaths.begin(), m_astar->m_amendedPaths.end(), AStar_Algo::coord(prevX, prevY)) == m_astar->m_amendedPaths.end())
			if (!twoPointLinesHasObstacle(prevPrevX, prevPrevY, prevX, prevY))
			{
				m_astar->m_amendedPaths.push_back(AStar_Algo::coord(prevX, prevY));
				prevPrevX = prevX;
				prevPrevY = prevY;
			}
			else
			{
				vector<AStar_Algo::coord> theMergedPaths;
				if (PathFindUsingAStar(prevPrevX, prevPrevY, prevX, prevY, preAngle,
					theMergedPaths))
				{
					for (vector<AStar_Algo::coord>::iterator im = theMergedPaths.begin();
						im != theMergedPaths.end(); ++im)
						m_astar->m_amendedPaths.push_back(
							AStar_Algo::coord(im->x*m_astar->getGridWidth(),
								im->y*m_astar->getGridWidth()));
					prevPrevX = theMergedPaths.back().x*m_astar->getGridWidth();
					prevPrevY = theMergedPaths.back().y*m_astar->getGridWidth();
				}
				else
				{
					prevPrevX = prevX;
					prevPrevY = prevY;
				}
			}
		}

		prevX = drawX;
		prevY = drawY;
		preAngle = angle;
	}
	m_astar->m_amendedPaths.push_back(AStar_Algo::coord(prevX, prevY));
	DrawLine(GetDC(), CPoint(prevPrevX - m_nScrollDeltaX, prevPrevY - m_nScrollDeltaY),
		CPoint(prevX - m_nScrollDeltaX, prevY - m_nScrollDeltaY), RGB(128, 0, 0));

}
//xMultiplyFactor and yMultiplyFactor is [-1,0,1]
bool CRobotPathFind::isObstacleForPathAmendPoint(int x, int y, int amendSize,
	int xMultiplyFactor, int yMultiplyFactor)
{
	for (int i = amendSize; i > 0; --i)
	{
		if (m_astar->isObstacle(x + i*xMultiplyFactor, y + i*yMultiplyFactor))
		{
			return true;
		}
	}
	return false;
}

bool CRobotPathFind::offsetToNewPosition(int x, int y, int amendSize,
	int xMultiplyFactor, int yMultiplyFactor, int& newX, int& newY)
{
	bool isAmended = false;
	for (int i = amendSize; i > 0; --i)
	{
		isAmended = true;
		for (int j = 2 * i; j > 0; --j)
		{
			if (m_astar->isObstacle(x + j*xMultiplyFactor, y + j*yMultiplyFactor))
			{
				isAmended = false;
				break;
			}
		}
		if (isAmended)
		{
			newX = x + i*xMultiplyFactor; newY = y + i*yMultiplyFactor;
			isAmended = true;
			return isAmended;
		}
	}
	return false;
}
#define ROBOT_BODY_RADIUM 40
bool CRobotPathFind::AmendPathPoint(int x0, int y0, int angle, int& newX, int& newY)
{
	if (m_routePointsNumBySetting)
	{
		vector<CPoint>::iterator rpIt;
		rpIt = std::find(m_tempRoutePoints.begin(),
			m_tempRoutePoints.end(), CPoint(x0, y0));
		if (rpIt != m_tempRoutePoints.end()) // if route point, do not amend
		{
			return false;
		}
	}
	bool isAmended = false;
	std::set<AStar_Algo::coord>::iterator it;
	int gridW = m_astar->getGridWidth();
	int x = x0 / gridW; // pixel value change to grid value
	int y = y0 / gridW;
	angle = (angle + 360) % 360;// make ange is in range of 0~360 
	int amendSize = m_scaleValue / 20;
	if (amendSize == 0) amendSize = 1;
	if ((angle > 338 || angle < 23) || (angle >= 158 && angle < 203)) // 0 or 180
	{
		if (isObstacleForPathAmendPoint(x, y, amendSize, 0, 1))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				0, -1, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, 0, -1))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				0, 1, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, -1, 0) && (angle > 338 || angle < 23))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				1, 0, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, 1, 0) && (angle >= 158 && angle < 203))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				-1, 0, newX, newY);
		}
	}
	else if ((angle >= 23 && angle < 68) || (angle >= 203 && angle < 248)) // 45 or 225
	{
		if (isObstacleForPathAmendPoint(x, y, amendSize, 1, -1))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				-1, 1, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, -1, 1))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				1, -1, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, 1, 1) && (angle >= 23 && angle < 68))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				-1, -1, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, -1, -1) && (angle >= 203 && angle < 248))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				1, 1, newX, newY);
		}
	}
	else if ((angle >= 68 && angle < 113) || (angle >= 248 && angle < 293)) // 90 or 270
	{
		if (isObstacleForPathAmendPoint(x, y, amendSize, -1, 0))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				1, 0, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, 1, 0))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				-1, 0, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, 0, 1) && (angle >= 68 && angle < 113)) //90
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				0, -1, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, 0, -1) && (angle >= 248 && angle < 293)) // 270
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				0, 1, newX, newY);
		}
	}
	else if ((angle >= 113 && angle < 158) || (angle >= 293 && angle < 338)) // 135 or 315
	{
		if (isObstacleForPathAmendPoint(x, y, amendSize, -1, -1))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				1, 1, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, 1, 1))
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				-1, -1, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, -1, 1) && (angle >= 113 && angle < 158)) // 135
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				1, -1, newX, newY);
		}
		else if (isObstacleForPathAmendPoint(x, y, amendSize, 1, -1) && (angle >= 293 && angle < 338)) // 315
		{
			isAmended = offsetToNewPosition(x, y, amendSize,
				-1, 1, newX, newY);
		}
	}
	if (isAmended)
	{
		newX = newX*gridW;
		newY = newY*gridW;
		return true;
	}
	else
	{
		// no update
		newX = x0;
		newY = y0;
		return false;
	}
}
void CRobotPathFind::cleanRawAStarFinalPaths()
{
	if (m_finalPathsX.empty()) return;

	//CPoint prevPoint(m_robotStartX - m_nScrollDeltaX, m_robotStartY - m_nScrollDeltaY);
	vector<int>::iterator itx = m_finalPathsX.begin();
	vector<int>::iterator ity = m_finalPathsY.begin();
	for (; itx != m_finalPathsX.end(); ++itx,++ity)
	{
		//CPoint currPoint(*itx - m_nScrollDeltaX, *ity - m_nScrollDeltaY);
		DrawPoint(&dcMemory, *itx - m_nScrollDeltaX, *ity - m_nScrollDeltaY, RGB(255, 255, 255), 2);

		//DrawLine(&dcRange, prevPoint, currPoint, RGB(255, 255, 255));
		//prevPoint = currPoint;
	}
	m_finalPathsX.clear();
	m_finalPathsY.clear();
}

void CRobotPathFind::cleanFinalPaths()
{
	if (m_finalPaths.empty()) return;

	CPoint prevPoint(m_robotStartX - m_nScrollDeltaX, m_robotStartY - m_nScrollDeltaY);
	vector<AStar_Algo::coord>::iterator it = m_finalPaths.begin();
	for (; it != m_finalPaths.end(); ++it)
	{
		CPoint currPoint(it->x - m_nScrollDeltaX, it->y - m_nScrollDeltaY);
		//DrawPoint(&dcMemory, it->x - m_nScrollDeltaX, it->y - m_nScrollDeltaY, RGB(255, 0, 0), 2); //test

		DrawLine(&dcRange, prevPoint, currPoint, RGB(255, 255, 255));
		prevPoint = currPoint;
	}
	m_finalPaths.clear();
}

