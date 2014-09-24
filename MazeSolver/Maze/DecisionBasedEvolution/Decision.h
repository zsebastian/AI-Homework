#pragma once
#include "../Coord.h"

struct Decision
{
public:
	enum Action { TURN_LEFT = -1, TURN_RIGHT = 1 };
	enum Direction { UP, RIGHT, DOWN, LEFT };

	Decision(Direction direction);
	Decision();

	Coord forward(Coord coord);
	Decision perform(Action action);

	Direction direction() const { return direction_; }
private:
	Direction direction_;
};
