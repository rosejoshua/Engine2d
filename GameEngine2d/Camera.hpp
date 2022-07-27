#pragma once
class Camera
{
public:
	Camera();
	Camera(int minX, int maxX, int minY, int maxY, int tileW);
	~Camera();

	const int LOOK_MODIFIER_SCALE = 20;

	int minX;
	int maxX;
	int minY;
	int maxY;
	int tileW;
	int lookModifierValue;

	int cameraX;
	int cameraY;
	double zoomScale;

	void useLookDirection(double radians);
};

