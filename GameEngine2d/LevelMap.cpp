#include "LevelMap.hpp"

LevelMap::LevelMap()
{
	m_width = 0;
	m_height = 0;
	m_tileIds = nullptr;
}

LevelMap::~LevelMap()
{
	if (m_tileIds != nullptr)
		delete m_tileIds;
}

LevelMap::LevelMap(int width, int height): m_width(width), m_height(height)
{
	m_tileIds = new std::vector<std::vector<int>>(width, std::vector<int>(height, 0));
}

void LevelMap::setTileIdFromBitmapArray(int sourceX, int sourceY, int tileId)
{
	if (sourceY >= m_width)
		std::cerr << "error, attempting to set array x-value outside of array size" << std::endl;
	else if (sourceX >= m_height)
		std::cerr << "error, attempting to set array y-value outside of array size" << std::endl;
	else
	{
		(* m_tileIds)[sourceY][m_height - 1 - sourceX] = tileId;
	}
}
