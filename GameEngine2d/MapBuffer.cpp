#include "MapBuffer.hpp"

MapBuffer::MapBuffer(int levelHeight, int tileWidth): HEIGHT(levelHeight), TILE_WIDTH(tileWidth)
{
	//we manually start at column index 0 (collision blocks) no matter what to keep player from walking left off map
	//start and end index will change as player moves.
	startIndex = 1;
	//push back left most hidden column of collision blocks
	MapBufferColumns.push_back(std::vector<int>(HEIGHT, 0));

	for (size_t i = 1; i < WIDTH; i++)
	{
		//push back columns of keys for background tiles (1) before adding any specific tile keys
		MapBufferColumns.push_back(std::vector<int>(HEIGHT, 0));
		MapBufferColumns.back()[HEIGHT - 2] = 0;
		MapBufferColumns.back()[HEIGHT - 1] = 0;
	}
}

int MapBuffer::getTileKey(int x, int y, int tileWidth)
{
	return MapBufferColumns[(x/tileWidth + startIndex) % WIDTH][y/tileWidth];
}

void MapBuffer::addColumn(std::vector<int>& newColumn)
{
	//setColumn to location of startIndex which will be last index in buffer once startIndex is incremented
	MapBufferColumns[startIndex] = newColumn;

	if (startIndex == WIDTH - 1)
	{
		startIndex = 1;
	}
	else
	{
		startIndex++;
	}

	//for (int i = 1; i < testMap.HEIGHT - 3; i++)
	//{
	//	tileFactory.copyTile(testMap.m_mapArray[i][testMap.WIDTH - 2], i);
	//}
	//tileFactory.cycleTileQueue();
}
