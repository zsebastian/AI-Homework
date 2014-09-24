#pragma once

#include "Coord.h"
class Maze;


class Solver
{
public:
	virtual ~Solver() {}
	virtual bool Step(Maze& maze) = 0;
	virtual void Reset() = 0;
	virtual void Reset(Coord start, Coord end) = 0;
};