#include "Reporter.h"
#include "../Random.h"
#include "../Maze.h"

Reporter::ptr Reporter::make(Randomizer& random)
{
	int r = random.NextInt(0, 2);
	switch (r)
	{
	case 0:
		return std::move(std::make_unique<WallAhead>());
	case 1:
		return std::move(std::make_unique<WallLeft>());
	case 2:
		return std::move(std::make_unique<WallRight>());
	default:
		return nullptr;
	}
}

Reporter::ptr WallAhead::clone()
{
	return std::move(std::make_unique<WallAhead>());
}

Reporter::ptr WallLeft::clone()
{
	return std::move(std::make_unique<WallLeft>());
}

Reporter::ptr WallRight::clone()
{
	return std::move(std::make_unique<WallRight>());
}

Reporter::ptr TrueReporter::clone()
{
	return std::move(std::make_unique<TrueReporter>());
}

bool Reporter::evaluate_helper(Maze& maze, Coord current_coord, Decision decision)
{
	auto dir = decision.direction();
	switch (dir)
	{
	case Decision::UP:
		return maze(current_coord.x, current_coord.y - 1).cost == -1;
	case Decision::RIGHT:
		return maze(current_coord.x + 1, current_coord.y).cost == -1;
	case Decision::DOWN:
		return maze(current_coord.x, current_coord.y + 1).cost == -1;
	case Decision::LEFT:
		return maze(current_coord.x - 1, current_coord.y).cost == -1;
	default:
		return false;
	}
}

bool WallAhead::evaluate(Maze& maze, Coord current_coord, Decision decision)
{
	return evaluate_helper(maze, current_coord, decision);
}

bool WallLeft::evaluate(Maze& maze, Coord current_coord, Decision decision)
{
	decision.perform(Decision::TURN_LEFT);
	return evaluate_helper(maze, current_coord, decision);
}

bool WallRight::evaluate(Maze& maze, Coord current_coord, Decision decision)
{
	decision.perform(Decision::TURN_RIGHT);
	return evaluate_helper(maze, current_coord, decision);
}

bool TrueReporter::evaluate(Maze& maze, Coord current_coord, Decision decision)
{
	return true;
}
