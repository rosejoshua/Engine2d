#include "TileFactory.hpp"

TileFactory::TileFactory()
{
	maxJumpHeightInTiles = -1;
	mapHeight = -1;
	minWidthSqueezeInTiles = -1;
	minHeightSqueezeInTiles = -1;
	maxPossibleStructures = -1;
	maxStructureLength = -1;
	freeSpaceGoal = -1;
	manipTilestackWidth = -1;
	regenSize = -1;
}

TileFactory::TileFactory(int mapHeight, int jumpHeight, int tileWidth, int playerHeight, int playerWidth)
{
	this->mapHeight = mapHeight;
	this->maxJumpHeightInTiles = jumpHeight / tileWidth;
	this->minWidthSqueezeInTiles = playerWidth > tileWidth ? (playerWidth / tileWidth + 1) : 1;
	this->minHeightSqueezeInTiles = playerHeight / tileWidth + 1;
	this->maxPossibleStructures = (mapHeight - 3) / minHeightSqueezeInTiles + 1;
	this->manipTilestackWidth = (TILE_FACTORY_WIDTH / 3) * 2;
	this->freeSpaceGoal = (manipTilestackWidth * mapHeight)/13*12;
	this->regenSize = TILE_FACTORY_WIDTH - manipTilestackWidth;
	this->maxStructureLength = 17;

	std::vector<int> temp;

	for (int i = 0; i < mapHeight; i++)
	{
		temp.push_back((rand() % maxStructureLength) + 1);
	}
	generator.push_back(temp);

	for (int i = 0; i < mapHeight; i++)
	{
		temp[i] = ((rand() % maxStructureLength) + 1);
	}
	generator.push_back(temp);

	for (int i = 0; i < mapHeight; i++)
	{
		temp[i] = 0;
	}

	int n = rand() % (maxPossibleStructures + 1);
	while (n > 0)
	{
		temp[rand() % temp.size()] = ((rand() % maxStructureLength) + 1);
		n--;
	}

	generator.push_back(temp);
	fillQueueInitial();
}

TileFactory::~TileFactory()
{
}

void TileFactory::copyTile(int& tileAddress, int heightIndex)
{
	tileAddress = tileStackQueue.front()[heightIndex];
}

void TileFactory::cycleTileQueue()
{
	tileStackQueue.erase(tileStackQueue.begin());

	if (tileStackQueue.size() == regenSize)
	{
		std::vector<int> temp(mapHeight, 1);
		while (tileStackQueue.size() < TILE_FACTORY_WIDTH)
		{
			tileStackQueue.push_back(temp);
		}
		collapseFreeSpace();
	}
	
	
}

void TileFactory::fillQueueInitial()
{
	std::vector<int> col(mapHeight, 1);
	while(tileStackQueue.size() < TILE_FACTORY_WIDTH)
	{ 
		tileStackQueue.push_back(col);
	}
	collapseFreeSpace();
}

void TileFactory::collapseFreeSpace()
{
	std::cout << "collapse called" << std::endl;
	int sum = std::accumulate(tileStackQueue.begin() + regenSize, tileStackQueue.end(), 0, [](auto lhs, const auto& rhs) {
		return std::accumulate(rhs.cbegin(), rhs.cend(), lhs);
		});
	int a = 0;
	while (sum > freeSpaceGoal)
	{
		a++;
		int x = regenSize + rand() % (manipTilestackWidth);
		int y = rand() % mapHeight;
		int n = (rand() % maxStructureLength) + 1;

			int p = (int)sqrt(n) - 1;
			if (x + p >= TILE_FACTORY_WIDTH || y - p < 0)
				continue;

			if (p > 0)
			{
				for (int i = x; i < x+p; i++)
				{
					for (int j = y; j > y-p; j--)
					{
						//std::cout << "i: " << i << " j: " << j << std::endl;
						if (tileStackQueue[i][j] == 1)
						{
							tileStackQueue[i][j] = 0;
							sum--;
						}
						
					}
				}
			}
			else
			{
				tileStackQueue[x][y] == 0;
				n--;
				sum--;
			}
	}

}
