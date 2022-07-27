#pragma once

class TestMap
{
public:	
	TestMap();
	~TestMap();

	//was 50wide
	static const int WIDTH = 70;
	//was 88tall
	static const int HEIGHT = 200;
	int m_mapArray[HEIGHT][WIDTH];
	
};

