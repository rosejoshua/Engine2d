#include "Camera.hpp"

Camera::Camera()
{
	cameraX = 0;
	cameraY = 0;
	zoomScale = 1.0;
	minX = 0;
	maxX = 0;
	minY = 0;
	maxY = 0;
	tileW = 0;
}

Camera::Camera(int minX, int maxX, int minY, int maxY, int tileW)
{
	cameraX = 0;
	cameraY = 0;
	zoomScale = 1.0;

	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->tileW = tileW;
	
	lookModifierValue = tileW * LOOK_MODIFIER_SCALE;
}

Camera::~Camera()
{

}

void useLookDirection(double radians)
{

}