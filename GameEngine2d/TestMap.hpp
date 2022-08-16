#pragma once

#include <vector>

class TestMap
{
public:	
	TestMap();
	~TestMap();

	static const int WIDTH = 70;
	static const int HEIGHT = 200;
	int m_mapArray[HEIGHT][WIDTH];
};

