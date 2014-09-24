#pragma once

#include "Random.h"
#include "Maze.h"
#include <vector>
#include "Coord.h"

class PrimGenerator
{
public:
	PrimGenerator(const Coord& start);

	/* returns true if there are more steps */
	bool Step(Maze& maze);
	void Reset(Maze& maze, const Coord& start);

private:
	
	void TryAddWalls(const Coord& coord, const Maze& maze);
	void Clear(Maze& maze);

	std::vector<Coord> walls_;

	int steps_;
	Randomizer random_;

	Coord start_;
};