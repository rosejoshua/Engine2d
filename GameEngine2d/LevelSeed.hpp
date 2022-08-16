#pragma once

#include <vector>
#include <map>

//hopefully for level generation that resembles (or ideally is) wave function collapse
class LevelSeed
{
public:
	//these are set up to be able to visualize the level layout when reading the values in the constructor. Each humanReadableRow 
	//represents the values at a given index in all of the columns, in order from lowest column index (top of screen) to highest.
	//They need to be translated to their proper structure  before applying wave function collapse neighbor value analysis
	//which is a vector of columns that are from left to right on the screen and top to bottom in each column
	LevelSeed(std::vector< std::vector<int>>& humanReadableSeedVector);
	
	//interate over all tiles and push each onto the vector, giving a weighted pool to draw randomly from
	//std::vector<int> tileIdTallies;

	//these tile pools are populated after iterating over the entire seed level, giving us a weighted pool of tile IDs that 
	//can be neighbors of consective pair of pairs (vertical-up and horzontal-left neighbor pairs) of tiles.
	//std::map<std::pair<int, int>, std::vector<int>> constrainedVerticalTilePool;
	//std::map<std::pair<int, int>, std::vector<int>> constrainedHorizontalTilePool;
	std::map
	<
		std::pair
		<
			std::pair<int, int>,
			std::pair<int, int>
		>, 
		std::vector<int>
	> constrainedTilePool;

	//provide vertical(upward) neighbor pair (furthest away first in the pair) and 
	//horizontal (leftward) neighbor pair (furthest away first in the pair) and an 
	//acceptable tileID will be returned that fits constraints found in seed level
	int getGeneratedTileId(int tileIDTwoAbove, int tileIDOneAbove, int tileIDTwoToLeft, int tileIDOneToLeft);
	
};

