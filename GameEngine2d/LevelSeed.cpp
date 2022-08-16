#include <iostream>
#include "LevelSeed.hpp"

LevelSeed::LevelSeed(std::vector<std::vector<int>>& humanReadableSeedVector)
{
	if (humanReadableSeedVector.size() < 10 || humanReadableSeedVector[0].size() < 10)
	{
		std::cerr << "critical failure: seed level too small, level will not generate!" << std::endl;
		return;
	}
	else
	{
		for (int y = 2; y < humanReadableSeedVector.size(); y++)
		{
			for (int x = 2; x < humanReadableSeedVector[y].size(); x++)
			{
				auto p = std::make_pair
				(
					std::make_pair
					(
						humanReadableSeedVector[y - 2][x],
						humanReadableSeedVector[y - 1][x]
					),
					std::make_pair
					(
						humanReadableSeedVector[y][x - 2],
						humanReadableSeedVector[y][x - 1]
					)
				);

				auto it = constrainedTilePool.find(p);

				if (it == constrainedTilePool.end())
				{
					constrainedTilePool.emplace(p, std::vector<int>());
					it = constrainedTilePool.find(p);
					it->second.push_back(humanReadableSeedVector[y][x]);
				}
				else
				{
					it->second.push_back(humanReadableSeedVector[y][x]);
				}				
			}
		}

		for (auto i : constrainedTilePool)
		{
			std::cout << "ENTRY:" << "\n";
			std::cout << "\t" << i.first.first.first << "/" << i.first.first.second << ", "
				<< i.first.second.first << "/" << i.first.second.second << std::endl;
			std::cout << "\t" << "tile ids: ";
			for (auto j : i.second)
			{
				std::cout << j << ",";
			}
			std::cout << std::endl;
		}

	}
}

int LevelSeed::getGeneratedTileId(int tileIDTwoAbove, int tileIDOneAbove, int tileIDTwoToLeft, int tileIDOneToLeft)

{
	auto lookupKeyPair = std::make_pair(std::make_pair(tileIDTwoAbove, tileIDOneAbove),
		std::make_pair(tileIDTwoToLeft, tileIDOneToLeft));
	auto it = constrainedTilePool.find(lookupKeyPair);

	if (it == constrainedTilePool.end())
	{
		std::cout << "critical failure, could not find key matching those tile neighbors" << std::endl;
		std::cout << "\t" << "<<" << tileIDTwoAbove << "," << tileIDOneAbove << "><" << tileIDTwoToLeft << "," << tileIDOneToLeft << ">>" << std::endl;
		return -999;
	}
	else
	{
		return it->second[rand() % (it->second.size())];
	}
}
