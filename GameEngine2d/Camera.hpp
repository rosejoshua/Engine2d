#pragma once
class Camera
{
public:
	int m_xPos;
	int m_yPos;
	int m_maxX;
	int m_maxY;
	int m_leftBumper;
	int m_rightBumper;
	int m_topBumper;
	int m_bottomBumper;

	Camera(int startX, int startY, int maxX, int maxY, int leftBumper, int rightBumper, int topBumper, int bottomBumper);
	~Camera();

	void setCameraProperties(int maxX, int maxY, int leftBumper, int rightBumper, int topBumper, int bottomBumper);

	void setCameraPos(int xPos, int yPos);
};

