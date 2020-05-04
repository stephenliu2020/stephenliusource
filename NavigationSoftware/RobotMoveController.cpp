/*******************************************************************/
/************** RobotMoveController.cpp ****************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include "RobotMoveController.h"
#include "myThreadArgument.h"
#include <string>
#define _USE_MATH_DEFINES 
#include <math.h>

void CRobotMoveController::HandleCurveLine()
{
	int startX = m_currX + m_kinectWallShiftX;
	int startY = m_currY + m_kinectWallShiftY;
	int startA = abs((int)m_absoluteAngle - (int)m_gaussPrevAngle)/*m_sensorAbsAngle*/;
	double wholeDistance = m_distance * 100 / m_scaleValue; // pixel convert to centimeter
	m_gauss_wholeDistance = wholeDistance*0.75;
	m_direct1stSegmentDistance = wholeDistance / (4 * cos(startA*M_PI / 180.0));
	m_gauss_startM = tan(startA*M_PI / 180.0);
	GetPoint(startX * 100 / m_scaleValue, startY * 100 / m_scaleValue,
		m_gauss_startM, m_direct1stSegmentDistance,
		m_gauss_startX, m_gauss_startY);
	m_segmentType = SEGMENT_IN_CURVELINE_TYPE;
	sendSplitDirectLineMove();
}

void CRobotMoveController::HandleCurveSegmentsInBigCurveStep()
{
	char strTest[MAX_STR_LEN];
	if (m_1stSegmentDirectIsDone)
	{
		m_1stSegmentDirectIsDone = false;
		m_2ndSegmentCurveIsDone = false; // direct segment is done and go for 2nd segment curve
		sprintf(strTest, "Start to handle 2nd segment curve\n");
		writeLogInfoIntoFile(strTest, strlen(strTest));
		sendSplitCurveLineMove(m_gauss_wholeDistance);
	}
	else // Gaussian curve segment is done and go for next big step move 
	{
		RobotMoves(m_it, GetDC()); // stephen test
	}
}

void CRobotMoveController::sendDirectLineMessage()
{
	UINT directDistance = m_distance * 100 / m_scaleValue; // pixel convert to cm
	if (m_segmentType == SEGMENT_IN_CURVELINE_TYPE)
		directDistance = m_direct1stSegmentDistance;
	short directSpeed = htons(m_autoNavSpeedFromSetting);
	UINT splitDistance = directDistance / m_splitParts;
	m_dataExtracted = false;
	int expectedDistance = splitDistance*m_splitPartCount;
	int revisedDistance = expectedDistance - (int)m_measuredDistance & 0xffff;
	m_splitPartCount++;
	char strTest[MAX_STR_LEN];
	sprintf(strTest, "TimerProc:Direct Line Split %d distance:%d m:%d onTimer\n", m_splitPartCount, splitDistance, m_measuredDistance);
	writeLogInfoIntoFile(strTest, strlen(strTest));

	SendMessage(WM_MOTORMOVINGMSG, DIRECT_LINE_MOVE_REQUEST, directSpeed << 16 | htons(splitDistance/*+ revisedDistance*/)); // test afterwards for revised distance

	if (splitDistance == 0)
	{
		m_splitParts = directDistance;
		splitDistance = 1;
	}
	double speed = abs(m_autoNavSpeedFromSetting*1.06); // cm/s
	UINT timerInterval = splitDistance/*+ revisedDistance*/ * 1000 / speed; // milliseconds

	m_measuredDistance = 0;

	m_nRobotTimer = SetTimer(IDT_TIMER_DIRECT, timerInterval, NULL); //1
	memset(strTest, 0, MAX_STR_LEN);
	sprintf(strTest, "**Set Line timer:%d parts:%d\n", timerInterval, m_splitParts);
	writeLogInfoIntoFile(strTest, strlen(strTest));
}

void CRobotMoveController::HandleDirectLineTimer()
{

	if (m_splitPartCount < m_splitParts)
	{
		/*char strTest[MAX_STR_LEN];
		sprintf(strTest, "TimerProc:send 0x3005 onTimer of DirectLine\n");
		writeLogInfoIntoFile(strTest, strlen(strTest));
		com_tx_packet(0x3005, NULL);*/
		sendDirectLineMessage();
	}
	else
	{
		KillTimer(m_nRobotTimer);
		m_splitPartCount = 0;
		char strTest[MAX_STR_LEN];
		if (m_segmentType == SEGMENT_IN_CURVELINE_TYPE)
		{
			sprintf(strTest, "LineTimer:HandleCurveSegment after directLine:distance:%d\n", m_measuredDistance & 0xffff);
			writeLogInfoIntoFile(strTest, strlen(strTest));
			m_1stSegmentDirectIsDone = true;
			HandleCurveSegmentsInBigCurveStep();
		}
		else
		{
			sprintf(strTest, "LineTimer:next move after directLine:distance:%d\n", m_measuredDistance & 0xffff);
			writeLogInfoIntoFile(strTest, strlen(strTest));
			RobotMoves(m_it, GetDC());
		}
	}
}

// return slope of diffAngle =>tan(diffAngle)
double CRobotMoveController::getRevisdedAngleFromMeasuredDistance(double& revisedX, double& revisedY)
{
	double measuredDistance; // protocol need change to one time distance
	if (m_prevArc.radius < 0)
		measuredDistance = (m_measuredDistance >> 16) & 0xffff;
	else // turn right, right wheel is internal wheel 
		measuredDistance = m_measuredDistance & 0xffff;
	UINT arcAngle = measuredDistance * 360 / (2 * M_PI * abs(m_prevArc.radius));
	double m1 = -1 / m_prevArc.startM, m2;
	double tanA = tan(arcAngle* M_PI / 180.0);
	m2 = (m1 - tanA) / (1 + tanA*m1);
	GetPoint(m_prevArc.x0, m_prevArc.y0, m2, m_prevArc.radius, revisedX, revisedY);
	double m_point2 = -1 / m2;
	double m_expectedPoint2 = m_prevArc.expectedM;
	return m_point2 - m_expectedPoint2, 1 + m_point2*m_expectedPoint2;
}

void CRobotMoveController::sendCurveMessage()
{
	double 	directionDegree, angleRadius;
	double x2, y2, x3, y3, endM, tanA, revised_startM, revisedX, revisedY;
	// Set up new Gauss curve
	//if (m_splitPartCount == 1)
	{
		revisedX = m_gauss_startX; revisedY = m_gauss_startY;
		revised_startM = m_gauss_startM;
	}
	//else // comment out this for simulation
	//{
	//	tanA = getRevisdedAngleFromMeasuredDistance(revisedX, revisedY);
	//	revised_startM = (m_gauss_startM - tanA) / (1 + tanA*m_gauss_startM);
	//}

	SetSplineCurve(revisedX, revisedY, revised_startM, m_gauss_wholeDistance);
	x2 = m_gauss_startX + m_gauss_wholeDistance / m_curveSplitParts;
	y2 = SplineCurve(x2);
	// Get radius and angle from Gauss curve
	getRadiusAndAngleFromGauss(m_gauss_startX, m_gauss_startY,
		x2, y2, directionDegree, angleRadius,
		x3, y3, endM);
	/*	m_prevArc[1].radius = m_prevArc[0].radius; // prev prev
	m_prevArc[1].startM = m_prevArc[0].startM;
	m_prevArc[1].expectedM = m_prevArc[0].expectedM;
	m_prevArc[0].radius = angleRadius; // prev
	m_prevArc[0].startM = m_fix_startM;
	m_prevArc[0].expectedM = endM;*/
	m_prevArc.radius = angleRadius; // prev 
	m_prevArc.startM = revised_startM;
	m_prevArc.expectedM = endM;
	m_gauss_startX = x3; m_gauss_startY = y3;
	m_gauss_startM = endM;
	double splitDistance = directionDegree * 2 * M_PI * abs(angleRadius) / 360; // pixel convert to cm

	directionDegree = directionDegree / m_curveSplitParts;
	short directSpeed = m_autoNavSpeedFromSetting;

	char strTest[MAX_STR_LEN];
	sprintf(strTest, "TimerProc:Move Curve A:%f R:%f\n", directionDegree/*m_sensorAbsAngle*/, angleRadius);
	writeLogInfoIntoFile(strTest, strlen(strTest));
	directionDegree *= 10.0; // convert to 0.1 degree unit
	if ((UINT)round(directionDegree) == 0 || (short)round(angleRadius) == 0
		|| directSpeed<-128 || directSpeed>127)
	{
		memset(strTest, 0, MAX_STR_LEN);
		sprintf(strTest, "Invalid data input for angle tuning\n");
		writeLogInfoIntoFile(strTest, strlen(strTest));
		return;
	}

	short angle_speed = directSpeed << 9 | (UINT)round(directionDegree);
	SendMessage(WM_MOTORMOVINGMSG, DIRECTION_REQUEST,
		htons(angle_speed) << 16 | htons((short)round(angleRadius)));

	double speed = abs(m_autoNavSpeedFromSetting*1.06); // cm/s
	UINT timerInterval = (UINT)((double)splitDistance * 1000 / speed); // milliseconds
	m_nDirectionTimer = SetTimer(IDT_TIMER_CURVE, 100, NULL);// &CurveLineProc);
	memset(strTest, 0, MAX_STR_LEN);
	sprintf(strTest, "**Set curve timer:%d parts:%d splitDistance:%d\n", timerInterval, m_curveSplitParts, splitDistance);
	writeLogInfoIntoFile(strTest, strlen(strTest));
}

void CRobotMoveController::HandleCurveLineTimer()
{
	if (m_splitPartCount < m_curveSplitParts)
	{
		char strTest[MAX_STR_LEN];
		KillTimer(m_nDirectionTimer);
		sprintf(strTest, "CurveTimer:send 0x3005\n");
		writeLogInfoIntoFile(strTest, strlen(strTest));
		com_tx_packet(0x3005, NULL);
		/*
		if (WaitForSingleObject(m_hEvent, 5000)== WAIT_TIMEOUT)
		{
		KillTimer(m_nDirectionTimer);
		AfxMessageBox(_T("Curve Line data Response error, timeout"));
		return;
		}*/

	}
	else
	{
		KillTimer(m_nDirectionTimer);
		m_splitPartCount = 0;
		char strTest[MAX_STR_LEN];
		sprintf(strTest, "CurveTimer:**next move after curveLine distance:%x\n", m_measuredDistance);
		writeLogInfoIntoFile(strTest, strlen(strTest));
		HandleCurveSegmentsInBigCurveStep();
	}
}

void CRobotMoveController::sendRotationMessage()
{
	m_splitPartCount++;

	UINT measuredDistance;
	measuredDistance = m_measuredDistance & 0xffff;
	UINT measuredAngle = measuredDistance * 360 / (M_PI * 61);
	int revisedAngle = round(m_splitPartCount*m_autoNavRotationAngle / m_curveSplitParts) - (int)measuredAngle;

	char strTest[MAX_STR_LEN];
	sprintf(strTest, "TimerProc:Rotation Line Split %d onTimer\n", m_splitPartCount);
	writeLogInfoIntoFile(strTest, strlen(strTest));
	int rotationAngle = htons(round(m_autoNavRotationAngle / m_curveSplitParts));
	short rotationSpeed = htons(m_autoNavRotationSpeed);

	SendMessage(WM_MOTORMOVINGMSG, ROTATION_REQUEST, rotationSpeed << 16 | rotationAngle);

	UINT splitDistance = round(m_autoNavRotationAngle / m_curveSplitParts) * M_PI * 61 / 360;

	double speed = abs(m_autoNavSpeedFromSetting*1.06); // cm/s
	UINT timerInterval = splitDistance * 1000 / speed; // milliseconds
	m_nRotationTimer = SetTimer(IDT_TIMER_ROTATION, timerInterval, NULL);
	memset(strTest, 0, MAX_STR_LEN);
	sprintf(strTest, "**Set rotation timer:%d parts:%d\n", timerInterval, m_curveSplitParts);
	writeLogInfoIntoFile(strTest, strlen(strTest));
}
void CRobotMoveController::HandleRotationTimer()
{
	if (m_splitPartCount < m_curveSplitParts)
	{
		//com_tx_packet(0x3005, NULL);
		/*
		if (WaitForSingleObject(m_hEvent, 5000)== WAIT_TIMEOUT)
		{
		KillTimer(m_nRotationTimer);
		AfxMessageBox(_T("Rotation data Response error, timeout"));
		return;
		}*/
		sendRotationMessage();
	}
	else
	{
		KillTimer(m_nRotationTimer);
		m_splitPartCount = 0;
		char strTest[MAX_STR_LEN] = "RotationTimer:**next move after rotation\n";
		writeLogInfoIntoFile(strTest, strlen(strTest));
		RobotMoves(m_it, GetDC());
	}
}
void CRobotMoveController::HandleRouteTraceTimer()
{
	GrabLaserData();
	// Draw laser scan data

	static int prevLaserX = 0, prevLaserY = 0;
	static UINT prevAngle = 0;
	int laserX = 0, laserY = 0, gpsX, gpsY, laserAngle, gpsAngle;

	if (m_bLaserUdpConnected)
	{
		laserX = m_bmWidth / 2 + m_laserPose.x*m_scaleValue;
		laserY = m_bmHeight / 2 - m_laserPose.y*m_scaleValue;
		laserAngle = round(-m_laserPose.angle);
	}
	if (m_bGpsUdpConnected)
	{
		gpsX = m_bmWidth / 2 + m_gpsPose.x*m_scaleValue;
		gpsY = m_bmHeight / 2 - m_gpsPose.y*m_scaleValue;
		gpsAngle = round(-m_gpsPose.angle);
	}
	if (!m_bLaserUdpConnected && !m_bGpsUdpConnected)
		return;
	if (m_bLaserUdpConnected&&m_bGpsUdpConnected)
	{
		if (laserY <= 120) // suppose 118 is the boundary of indoor and outdoor
		{
			laserX = gpsX;
			laserY = gpsY;
			m_absoluteAngle = (gpsAngle + 360) % 360;
		}
		else
			m_absoluteAngle = (laserAngle + 360) % 360;
	}
	else if (m_bGpsUdpConnected)
	{
		laserX = gpsX;
		laserY = gpsY;
		m_absoluteAngle = (gpsAngle + 360) % 360;
	}
	else
		m_absoluteAngle = (laserAngle + 360) % 360;
	if (prevLaserX != 0)
	{
		int distanceThreas = 5 * m_scaleValue / 100;
		// no cooridinate or angle change, ignore it
		if (abs(prevLaserX - laserX) <= distanceThreas &&
			abs(prevLaserY - laserY) <= distanceThreas &&
			abs((int)prevAngle - (int)m_absoluteAngle) < 1)
			return;
		DrawTriangle(&dcRange, prevLaserX - m_nScrollDeltaX, prevLaserY - m_nScrollDeltaY, RGB(255, 255, 255), prevAngle, 16);

		DrawTriangle(pDC, prevLaserX - m_nScrollDeltaX, prevLaserY - m_nScrollDeltaY, RGB(255, 255, 255), prevAngle, 16);
		DrawPoint(pDC, prevLaserX - m_nScrollDeltaX, prevLaserY - m_nScrollDeltaY, RGB(0, 255, 0), 2);
	}
	//m_startNavPoint.x = laserX;
	//m_startNavPoint.y = laserY;
	m_realLocX = laserX;
	m_realLocY = laserY;
	m_bSetStartNav = true;
	prevLaserX = laserX;
	prevLaserY = laserY;
	prevAngle = m_absoluteAngle;
	DrawTriangle(&dcRange, laserX - m_nScrollDeltaX, laserY - m_nScrollDeltaY, RGB(0, 0, 255), m_absoluteAngle, 16);

	DrawTriangle(pDC, laserX - m_nScrollDeltaX, laserY - m_nScrollDeltaY, RGB(0, 0, 255), m_absoluteAngle, 16);

}


void CRobotMoveController::sendSplitDirectLineMove()
{
	com_tx_packet(0x300c, NULL); // send distance measurement reset command
	m_measuredDistance = 0;
	UINT directDistance = m_distance * 100 / m_scaleValue;
	if (directDistance <= 10)
		m_splitParts = directDistance;
	else
		m_splitParts = m_splitPartsFromSetting;
	sendDirectLineMessage();
}

void CRobotMoveController::sendSplitCurveLineMove(double movingDistance)
{
	com_tx_packet(0x300c, NULL); // send distance measurement reset command
	m_measuredDistance = 0;
	if (movingDistance < 11)
		m_curveSplitParts = (UINT)round(movingDistance);
	else
		m_curveSplitParts = m_curveSplitPartsFromSetting;

	sendCurveMessage();

}

void CRobotMoveController::sendSplitRotationMove(UINT rotationAngle)
{
	com_tx_packet(0x300c, NULL); // send distance measurement reset command

	m_measuredDistance = 0;
	m_curveSplitParts = round(m_autoNavRotationAngle) / 10;
	sendRotationMessage();
}


void CRobotMoveController::calculateThreeSegments(int startA, double wholeDistance, double& direct1stSegment, double& arc2ndSegmentRadius, double& arc3rdSegmentRadius)
{
	double x;
	
	double tanA = tan(startA*M_PI / 180);
	double sinA = sin(startA*M_PI / 180);
	double cosA = cos(startA*M_PI / 180);

	x = wholeDistance / (1 + tanA*tanA + tanA + tanA*tanA / sin((90 - startA) / 2 * M_PI / 180));
	direct1stSegment = x / cos(startA*M_PI / 180);
	arc2ndSegmentRadius = x*tanA / cosA;
	arc3rdSegmentRadius =
		arc2ndSegmentRadius*sinA*sin((90 + startA) / 2 * M_PI / 180) / (sin((90 - startA) / 2 * M_PI / 180)*sin((90 - startA)* M_PI / 180));
}

int CRobotMoveController::OnRobotMoving(long int wpara, long int lpara)
{
	m_cmdFromDlg = wpara;
	UINT robotPara = lpara;

	switch (m_cmdFromDlg)
	{
	case STOP_REQUEST:
		com_tx_packet(0x3003, robotPara);
		//HandleStop();
		break;
	case DIRECTION_REQUEST:
		m_bDirectionCmdFromDlg = true;
		m_bStopMove = false;

		/*char strTest[MAX_STR_LEN];
		sprintf(strTest, "Turn curve line request data field:%x\n", robotPara);
		writeLogInfoIntoFile(strTest, strlen(strTest));*/
		com_tx_packet(0x3004, robotPara);
		if (m_robotMoveDlg)
			m_dlgDirection = m_robotMoveDlg->m_directionDegree;
		break;
	case DIRECT_LINE_MOVE_REQUEST:
	{
		m_bDistanceCmdFromDlg = true;
		m_bStopMove = false;
		m_bDistanceResponse = true;
		m_distanceDlgReqStartX = m_currX;
		m_distanceDlgReqStartY = m_currY;
		/*	char strTest[MAX_STR_LEN];
		sprintf(strTest, "Direct Line request data field:%x\n", robotPara);
		writeLogInfoIntoFile(strTest, strlen(strTest));*/
		com_tx_packet(0x300a, robotPara);

		m_dlgDistance = robotPara & 0xffff;
		m_dlgDistance = htons(m_dlgDistance);
	}
	break;
	case ROTATION_REQUEST:
	{
		m_bDirectionCmdFromDlg = true;
		m_bStopMove = false;
		m_dlgRotationAngle = robotPara & 0x0ffff;
		m_rotationSpeed = htons((robotPara >> 16) & 0x0ffff);
		/*char strTest[MAX_STR_LEN];
		sprintf(strTest, "Rotation request data field:%x\n", robotPara);
		writeLogInfoIntoFile(strTest, strlen(strTest));*/
		com_tx_packet(0x300b, robotPara);
	}
	break;
	case SERIAL_CONNECTION_TEST:
		com_tx_packet(0x3006, robotPara);
		break;
	case AUTO_CHARGING_REQ:
		com_tx_packet(0x3009, robotPara);
		break;
	case DISTANCE_EXTRACTION:
		com_tx_packet(0x3005, robotPara);
		break;
	default:
		break;
	}
	return 0;
}
void CRobotMoveController::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//MessageBeep(0xFFFFFFFF);   // Beep
	if (nIDEvent == m_nRobotTimer)
	{
		HandleDirectLineTimer();
	}
	else if (nIDEvent == m_nDirectionTimer)
	{
		HandleCurveLineTimer();
	}
	else if (nIDEvent == m_nRotationTimer)
	{
		HandleRotationTimer();
	}
	else if (nIDEvent == m_nRouteTrace)
	{
		HandleRouteTraceTimer();
	}

ExitTimer:
	CScrollView::OnTimer(nIDEvent);
}
