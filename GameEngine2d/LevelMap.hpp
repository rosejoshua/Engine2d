#pragma once

#include <vector>
#include <iostream>

class LevelMap
{
public:
	LevelMap();
	~LevelMap();
	LevelMap(int width, int height);
	int m_width;
	int m_height;
	std::vector<std::vector<int>>* m_tileIds;

	//Initial temp 2d array of tileIds populated from bitmap is composed of rows from bottom to top
	//containing tileIds from left to right. This needs to be converted to a 2d array composed of columns
	//from left to right and those columns containing the row elements from top to bottom. This is so maps
	//can be drawn from basic app like Paint.net and look on screen as they will in game. However,
	//this layout precludes buffering columns from left to right, and is incompatible with the engine drawing 
	//logic which has an origin (0,0) at the top-left.
	void setTileIdFromBitmapArray(int sourceX, int sourceY, int tileId);
};

