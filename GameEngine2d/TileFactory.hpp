#pragma once
#include <vector>
#include <queue>
#include <iostream>
#include <numeric>

class TileFactory
{
public:
	TileFactory();
	TileFactory(int mapHeight, int jumpHeight, int tileWidth, int playerHeight, int playerWidth);
	~TileFactory();

	const int TILE_FACTORY_WIDTH = 30;

	int mapHeight;
	int maxJumpHeightInTiles;
	int minWidthSqueezeInTiles;
	int minHeightSqueezeInTiles;
	int maxPossibleStructures;
	int maxStructureLength;
	int freeSpaceGoal;
	int manipTilestackWidth;
	int regenSize;

	std::vector<std::vector<int>> tileStackQueue;
	std::vector<std::vector<int>> generator;

	void copyTile(int& tileAddress, int heightIndex);
	void cycleTileQueue();

	void incrementDesign();
	void fillQueueInitial();
	void collapseFreeSpace();
};

