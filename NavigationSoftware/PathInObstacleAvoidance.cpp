/*******************************************************************/
/**************  PathInObstacleAvoidance.cpp  **********************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include "PathInObstacleAvoidance.h"
void CPathInObstacleAvoidance::markOneBlockForUltrasonic(int gridX, int gridY, int i)
{
	COLORREF color;
	if (i == 8)
		color = RGB(255, 165, 0);
	else
		color = RGB(160 - 20 * i, 160 - 20 * i, 128);
	if (!m_astar->isObstacle(gridX, gridY))
	{
		m_astar->setObstacle(gridX, gridY);
		int blkPosX = gridX * m_astar->getGridWidth();
		int blkPosY = gridY * m_astar->getGridWidth();
		if (!m_bSetExtraVirtualWall) // We do not draw it for virtual wall
			DrawArea(&dcRange/*GetDC()*/, blkPosX, blkPosY, color, m_astar->getGridWidth() / 2); //255, 0, 0	
		if (m_bSetExtraVirtualWall)
		{
			m_virWalls.push_back(CPoint(blkPosX, blkPosY));
			saveVirtualWallIntoFile(blkPosX, blkPosY, m_wallBlkNum);
			m_wallBlkNum++;
		}
		else
			m_blocks.push_back(CPoint(blkPosX, blkPosY));
	}
}
// max distance is 3, otherwise is endlesss return -1
void CPathInObstacleAvoidance::markExtraGridForFindingWidePath(int gridX1, int gridY1, 
	int gridX2, int gridY2, int angle)
{
	int len, newX1, newY1, newX2, newY2;
	int prevX1, prevY1, prevX2, prevY2;
	for (len = 1; len <= 4; ++len)
	{
		angle = (angle + 360) % 360;
		if ((angle > 45 && angle < 135) ||
			(angle > 225 && angle < 315)) //90, 270
		{
			if (gridY1 < gridY2)
			{
				GetPoint(gridX1, gridY1, max(angle % 360, (angle + 180) % 360), len, newX1, newY1);
				GetPoint(gridX2, gridY2, min(angle % 360, (angle + 180) % 360), len, newX2, newY2);
			}
			else
			{
				GetPoint(gridX1, gridY1, min(angle % 360, (angle + 180) % 360), len, newX1, newY1);
				GetPoint(gridX2, gridY2, max(angle % 360, (angle + 180) % 360), len, newX2, newY2);
			}
		}
		else // 0,180
		{
			if (gridX1 < gridX2)
			{
				GetPoint(gridX1, gridY1, max(angle%360, (angle + 180)%360), len, newX1, newY1);
				GetPoint(gridX2, gridY2, min(angle % 360, (angle + 180) % 360), len, newX2, newY2);
			}
			else
			{
				GetPoint(gridX1, gridY1, min(angle % 360, (angle + 180) % 360), len, newX1, newY1);
				GetPoint(gridX2, gridY2, max(angle % 360, (angle + 180) % 360), len, newX2, newY2);
			}
		}

		if (m_astar->isObstacle(newX1, newY1)
			&& m_astar->isObstacle(newX2, newY2))
		{
			if (len == 1) 
				continue; // SetGrid in BresenhamLine already set obstacle for extra one
			else
				return;
		}
		else if (m_astar->isObstacle(newX1, newY1)
			&& !m_astar->isObstacle(newX2, newY2))

		{
			BresenhamLine(gridX1, gridY1, newX1, newY1, 8);
			return;
		}
		else if (!m_astar->isObstacle(newX1, newY1)
			&& m_astar->isObstacle(newX2, newY2))
		{
			BresenhamLine(gridX2, gridY2, newX2, newY2, 8);
			return;
		}
		else
		{
			continue;
		}
	}
}

void CPathInObstacleAvoidance::markObstacleForOneUltraSensor(int x0, int y0, int pixelDistance, char detectAngle, int radium)
{
	/*double unitAngle = m_astar->getGridWidth() * 360 / (2 * M_PI*pixelDistance);
	int halfParts = (int)round(22.5 / unitAngle /2);
	// since robot body, it will occupy extra space (half body as 40cm)
	int extraParts = 40 * m_scaleValue / 100 / m_astar->getGridWidth();
	halfParts += extraParts;*/
	int bodyLength;
	if (pixelDistance <= 20 * m_scaleValue / 100) // if within 20cm, make obstacle wide
	{
		pixelDistance = 20 * m_scaleValue / 100;
		bodyLength = 80 * m_scaleValue / 100; // stephen change 110 to 120 on Jun 23
	}
	else //if (pixelDistance <= 50 * m_scaleValue / 100) // if within 20cm, make obstacle wide
	{
		pixelDistance = 50 * m_scaleValue / 100;
		//if (detectAngle<22)
			bodyLength = 110 * m_scaleValue / 100; // stephen change 110 to 120 on Jun 23
		//else
			//bodyLength = 130/*120*/ * m_scaleValue / 100; // stephen change 110 to 120 on Jun 23
	}
	/*
	else // tan(22.5)=0.414
	{
		if (i==0)
			bodyLength = (2*0.414*pixelDistance+79)* m_scaleValue / 100;
		else
			bodyLength = 200* m_scaleValue / 100;

	}*/
	int x, y, x1, y1, x2, y2, gridX, gridY, gridX1, gridY1, gridX2, gridY2;
	int gridWidth = m_astar->getGridWidth();
	GetPoint(x0, y0, m_absoluteAngle + detectAngle, pixelDistance, x, y);
#if 1
	GetPoint((int)round((float)x/ gridWidth), (int)round((float)y / gridWidth), m_absoluteAngle + detectAngle + 90, bodyLength / (2*gridWidth), gridX1, gridY1);
	GetPoint((int)round((float)x / gridWidth), (int)round((float)y / gridWidth), m_absoluteAngle + detectAngle - 90, bodyLength / (2*gridWidth), gridX2, gridY2);
#else
	GetPoint(x, y, m_absoluteAngle + 45 * i + 90, bodyLength / 2, x1, y1);
	GetPoint(x, y, m_absoluteAngle + 45 * i - 90, bodyLength / 2, x2, y2);
	gridX = (int)round((float)x/ m_astar->getGridWidth());
	gridY = (int)round((float)y/ m_astar->getGridWidth());
	gridX1 = (int)round((float)x1 / m_astar->getGridWidth());
	gridY1 = (int)round((float)y1 / m_astar->getGridWidth());
	gridX2 = (int)round((float)x2 / m_astar->getGridWidth());
	gridY2 = (int)round((float)y2 / m_astar->getGridWidth());
#endif
	BresenhamLine(gridX1, gridY1, gridX2, gridY2, 0);
	m_obstacleAngle = (int)round((float)atan2(gridY2 - gridY1, gridX2 - gridX1) * 180 / M_PI);
	markExtraGridForFindingWidePath(gridX1, gridY1, gridX2, gridY2, m_obstacleAngle);

}

bool CPathInObstacleAvoidance::RepathPointsOnMap(int x0, int y0,
	int& xs, int& ys, int& xf, int& yf)
{
	int pixelDistance = m_scaleValue;
	int x3, y3, x4, y4, x5, y5,x11,y11,x22,y22;
	GetPoint(x0, y0, m_absoluteAngle + 90, 100 * m_scaleValue / 100, xs, ys);
	GetPoint(x0, y0, m_absoluteAngle+90, 120 * m_scaleValue / 100, x3, y3); // change to arc
	if (!twoPointLinesHasObstacle(x0, y0, x3, y3))
	{
		GetPoint(xs, ys, m_absoluteAngle, 140 * m_scaleValue / 100, xf, yf);
		GetPoint((xs+xf)/2, (ys+yf)/2, m_absoluteAngle-90, 30 * m_scaleValue / 100, x11, y11); // new test
		GetPoint(xf, yf, m_absoluteAngle + 90, 15 * m_scaleValue / 100, x4, y4);
		GetPoint(xf, yf, m_absoluteAngle - 90, 30 * m_scaleValue / 100, x22, y22); // new test
		GetPoint(xf, yf, m_absoluteAngle, 50 * m_scaleValue / 100, x5, y5);
		if (!twoPointLinesHasObstacle(xs, ys, x5, y5) &&
			!twoPointLinesHasObstacle(xs, ys, x11, y11) && // new test
			!twoPointLinesHasObstacle(xf, yf, x4, y4) &&
			!twoPointLinesHasObstacle(xf, yf, x22, y22)) // new test
		{
			m_curveRadius = 1.0; // counterclockwise
			DrawLine(&dcRange, CPoint(x0 - m_nScrollDeltaX, y0 - m_nScrollDeltaY),
				CPoint(xs - m_nScrollDeltaX, ys - m_nScrollDeltaY), RGB(255, 128, 0));
			DrawLine(&dcRange, CPoint(xs - m_nScrollDeltaX, ys - m_nScrollDeltaY),
				CPoint(xf - m_nScrollDeltaX, yf - m_nScrollDeltaY), RGB(255, 128, 0));

			return true;
		}
	}
	else
	{
		GetPoint(x0, y0, m_absoluteAngle - 90, 100 * m_scaleValue / 100, xs, ys);
		GetPoint(x0, y0, m_absoluteAngle-90, 120 * m_scaleValue / 100, x3, y3);
		if (!twoPointLinesHasObstacle(x0, y0, x3, y3))
		{
			GetPoint(xs, ys, m_absoluteAngle, 140 * m_scaleValue / 100, xf, yf);
			GetPoint((xs + xf) / 2, (ys + yf) / 2, m_absoluteAngle + 90, 30 * m_scaleValue / 100, x11, y11); // new test
			GetPoint(xf, yf, m_absoluteAngle - 90, 15 * m_scaleValue / 100, x4, y4);
			GetPoint(xf, yf, m_absoluteAngle + 90, 30 * m_scaleValue / 100, x22, y22); // new test
			GetPoint(xf, yf, m_absoluteAngle, 50 * m_scaleValue / 100, x5, y5);

			if (!twoPointLinesHasObstacle(xs, ys, x5, y5) &&
				!twoPointLinesHasObstacle(xs, ys, x11, y11) && // new test
				!twoPointLinesHasObstacle(xf, yf, x4, y4) &&
				!twoPointLinesHasObstacle(xf, yf, x22, y22))// new test
			{
				m_curveRadius = -1.0; // counterclockwise
				DrawLine(&dcRange, CPoint(x0 - m_nScrollDeltaX, y0 - m_nScrollDeltaY),
					CPoint(xs - m_nScrollDeltaX, ys - m_nScrollDeltaY), RGB(255, 128, 0));
				DrawLine(&dcRange, CPoint(xs - m_nScrollDeltaX, ys - m_nScrollDeltaY),
					CPoint(xf - m_nScrollDeltaX, yf - m_nScrollDeltaY), RGB(255, 128, 0));

				return true;
			}

		}
	}
	return false;
}

void CPathInObstacleAvoidance::markRetangleOnMap()
{
	int x, y,x1,y1,x0,y0,x2,y2;
	int height = 150 * m_scaleValue / 100;
	GetPoint(m_robotStartX, m_robotStartY, m_absoluteAngle, 100 * m_scaleValue / 100, x, y);
	GetPoint(x, y, m_absoluteAngle - 90, 25 * m_scaleValue / 100, x1, y1);
	GetPoint(m_robotStartX, m_robotStartY, m_absoluteAngle, 50 * m_scaleValue / 100, x0, y0);
	GetPoint(x0, y0, m_absoluteAngle + 90, 25 * m_scaleValue / 100, x2, y2);
	//BresCircle(gridX, gridY, height/2);
	int gridX1 = round(x1 / m_gridWidh), gridY1 = round(y1 / m_gridWidh);
	int gridX2 = round(x2 / m_gridWidh), gridY2 = round(y2 / m_gridWidh);
	Rectangle(gridX1, gridY1, gridX2, gridY2);
}

void CPathInObstacleAvoidance::cleanObstacleOnMap(bool keepObstacleShow)
{
	while (!m_blocks.empty())
	{
		CPoint currBlock = m_blocks.back();
		m_blocks.pop_back();
		DrawArea(&dcRange/*GetDC()*/, currBlock.x/*-m_nScrollDeltaX*/, currBlock.y/*-m_nScrollDeltaY*/, RGB(255, 255, 255), m_astar->getGridWidth() / 2);
		//if (keepObstacleShow)
			//DrawArea(GetDC(), currBlock.x-m_nScrollDeltaX, currBlock.y-m_nScrollDeltaY, RGB(255, 255, 128), m_astar->getGridWidth() / 2);

		m_astar->clearObstacle(currBlock.x / m_astar->getGridWidth(), currBlock.y / m_astar->getGridWidth());
	}
}

#ifdef USIR_OBSTACLEAVOIDANCE
void CPathInObstacleAvoidance::markUSIRObstalceOnMap()
{
	int robotX, robotY, x0, y0;
	int radium = 33 * m_scaleValue / 100;
	//char strTest[MAX_STR_LEN];
	//sprintf(strTest, "markObstalceOnMap dista:%d\n", m_detectDistance);
	//writeLogInfoIntoFile(strTest, strlen(strTest));

	//GetPoint(m_robotStartX, m_robotStartY, m_absoluteAngle + 180, radium, x0, y0);
	int x, y, gridX, gridY;
	int pixelDistance = m_detectDistance* m_scaleValue / 100;
	markObstacleForOneUltraSensor(m_robotStartX, m_robotStartY, pixelDistance, m_detectAngle, radium);
	Invalidate();
}

void CPathInObstacleAvoidance::cleanUSIRObstalceOnMap()
{
	while (!m_blocks.empty())
	{
		CPoint currBlock = m_blocks.back();
		m_blocks.pop_back();
		DrawArea(&dcRange/*GetDC()*/, currBlock.x/*-m_nScrollDeltaX*/, currBlock.y/*-m_nScrollDeltaY*/, RGB(255, 255, 255), m_astar->getGridWidth() / 2);

		m_astar->clearObstacle(currBlock.x / m_astar->getGridWidth(), currBlock.y / m_astar->getGridWidth());
	}
}
#endif

#ifdef REALSENSE_OBSTACLEAVOIDANCE
Mat CPathInObstacleAvoidance::rotate(Mat src, float angle) {
	Mat dst(src.cols * 2, src.rows * 2, CV_8UC1, Scalar(255));
	src.copyTo(dst(Rect(src.cols / 2, 0, src.cols, src.rows)));

	Point2f pt(src.cols, src.rows);
	Mat r = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(dst, dst, r, Size(src.cols * 2, src.rows * 2), 1, 0, Scalar(255));

	return dst;
}

// Convert array into image
// @params data: the array delivered via UDP
// @params angle: the direction the robot is facing
// @params pxPerMeter: the desired pixels per meter
// @return: a map image with the robot's current pose located at the center of the image
Mat CPathInObstacleAvoidance::decompressView(unsigned short data[16], float angle, float pxPerMeter) {
	Mat output(2.857 * pxPerMeter, 2.50 * pxPerMeter, CV_8UC1, Scalar(255));

	//Size of input data grid
	short width = 16, height = 16;

	short xShift = output.cols / width;
	short yShift = output.rows / height;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			bool bit = (data[j] >> i) & 1;

			if (bit) {
				rectangle(output, cv::Point(xShift * i, yShift * j), cv::Point(xShift * (i + 1), yShift * (j + 1)), Scalar(0), -1);
			}
		}
	}
	//flip(output, output, -1);
	//flip(output, output, 1);
	//Apply rotation
	output = rotate(output, angle);

	return output;
}


void CPathInObstacleAvoidance::markRealSenseObstalceOnMap()
{

#define m_matrixWidth 16
#define m_matrixSize 16*16
	int width, height, widthBytes;

	//Convert matrix map into matrix 
	//m_currX, m_currY, m_absoluteAngle 
	// m_realSenseData	=> pixels
	int x = (m_absoluteAngle + 90 + 360) % 360;
	if (x > 180) {
		x -= 360;
	}
	x = -x;
	Mat binaryMat/*grayscaleMat*/ = decompressView(m_realSenseData, x, m_scaleValue);
	//Binary image
	/*cv::Mat binaryMat(grayscaleMat.size(), grayscaleMat.type());

	//Apply thresholding
	cv::threshold(grayscaleMat, binaryMat, 100, 255, cv::THRESH_BINARY);*/
	//imshow("result_mat", binaryMat);
	width = binaryMat.cols;
	height = binaryMat.rows;
	widthBytes = width / 8;
	int startX = m_robotStartX - width / 2;
	int startY = m_robotStartY - height / 2;
	int cnt = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			if (binaryMat.channels() == 1)
			{
				if ((int)(binaryMat.at<uchar>(i, j)) != 0 && (int)(binaryMat.at<uchar>(i, j)) != 255)
				{
					cnt++;
				}
				else if ((int)(binaryMat.at<uchar>(i, j)) == 255)
				{

				}
				else if ((int)(binaryMat.at<uchar>(i, j)) == 0)
				{
					int x = j + startX;
					int y = i + startY;
					//GetDC()->SetPixel(x, y, RGB(0, 0, 200));// 119, 125, 128));
					int gridX = (int)round((float)x / m_astar->getGridWidth());
					int gridY = (int)round((float)y / m_astar->getGridWidth());
					if (!m_astar->isObstacle(gridX, gridY))
					{
						m_astar->setObstacle(gridX, gridY);
						int blkPosX = gridX * m_astar->getGridWidth();
						int blkPosY = gridY * m_astar->getGridWidth();
						//DrawArea(GetDC(), blkPosX-m_nScrollDeltaX, blkPosY-m_nScrollDeltaY, RGB(119, 125, 128), m_astar->getGridWidth() / 2); //255, 0, 0
						DrawArea(&dcRange, blkPosX, blkPosY, RGB(255, 255, 128), m_astar->getGridWidth() / 2); //255, 0, 0

						m_blocks.push_back(CPoint(blkPosX, blkPosY));
						//addExtraBlockForObstacleDetection(gridX, gridY);
					}
				}
			}
			else
			{
				AfxMessageBox(_T("Warning: It is not 1 channel"));
				std::cout << "It is not 1 channel" << std::endl;
			}
		}
	}
	if (cnt == 0)
	{
		std::cout << "Black and white image" << std::endl;
	}


#if 0
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < widthBytes; j++)
		{
			for (int k = 7; k >= 0; k--)
			{
				if ((bytes[i*widthBytes + j] & (1 << k)) == 1)
				{
					int x = j * 8 + abs(k - 7) + startX;
					int y = i + startY;
					// Draw cloud point map for test
					GetDC()->SetPixel(x, y, RGB(119, 125, 128));
					int gridX = (int)round((float)x / m_astar->getGridWidth());
					int gridY = (int)round((float)y / m_astar->getGridWidth());
					if (!m_astar->isObstacle(gridX, gridY))
					{
						m_astar->setObstacle(gridX, gridY);
						int blkPosX = gridX * m_astar->getGridWidth();
						int blkPosY = gridY * m_astar->getGridWidth();
						DrawPoint(pDC, blkPosX, blkPosY, RGB(119, 125, 128), 2); //255, 0, 0
						m_blocks.push_back(CPoint(blkPosX, blkPosY));
					}
				}
			}
		}
	}
#endif
}
#endif