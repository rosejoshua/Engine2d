#pragma once

#include<vector>

//circular array style buffer for onscreen tiles and some extra in case the engine bogs down when transferring colums from additional levels
class MapBuffer
{
public:
	const int WIDTH = 70;
	const int HEIGHT;
	const int TILE_WIDTH;
	int startIndex;
	std::vector<std::vector<int>> MapBufferColumns;

	MapBuffer(int levelHeight, int tileWidth);

	int getTileKey(int x, int y, int tileWidth);
	void addColumn(std::vector<int>& newColumn);
};

