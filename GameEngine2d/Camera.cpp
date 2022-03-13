#include "Camera.hpp"

Camera::Camera(int startX, int startY, int maxX, int maxY, int leftBumper, int rightBumper, int topBumper, int bottomBumper)
{
	m_xPos = startX;
	m_yPos = startY;
	m_maxX = maxX;
	m_maxY = maxY;
	m_leftBumper = leftBumper;
	m_rightBumper = rightBumper;
	m_topBumper = topBumper;
	m_bottomBumper = bottomBumper;
}

Camera::~Camera(){}

void Camera::setCameraProperties(int maxX, int maxY, int leftBumper, int rightBumper, int topBumper, int bottomBumper)
{
	m_maxX = maxX;
	m_maxY = maxY;
	m_leftBumper = leftBumper;
	m_rightBumper = rightBumper;
	m_topBumper = topBumper;
	m_bottomBumper = bottomBumper;
}

void Camera::setCameraPos(int xPos, int yPos)
{
	m_xPos = xPos;
	m_yPos = yPos;
}


