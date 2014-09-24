#include "Decision.h"

Decision::Decision(Decision::Direction direction)
{
	direction_ = direction;
}

Decision::Decision()
{
	direction_ = Decision::UP;
}

Coord Decision::forward(Coord coord)
{
	switch (direction_)
	{
	case Decision::UP:
		return Coord(coord.x, coord.y - 1);
		break;
	case Decision::RIGHT:
		return Coord(coord.x + 1, coord.y);
		break;
	case Decision::DOWN:
		return Coord(coord.x, coord.y + 1);
		break;
	case Decision::LEFT:
		return Coord(coord.x - 1, coord.y);
		break;
	default:
		break;
	}
}

Decision Decision::perform(Decision::Action action)
{
	int n = static_cast<int>(direction_)+static_cast<int>(action);
	if (n < 0)
	{
		n = 3;
	}
	direction_ = static_cast<Direction>(n % 4);
	return *this;
}
