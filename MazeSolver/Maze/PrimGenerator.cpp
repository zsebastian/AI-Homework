#include "PrimGenerator.h"
#include <cassert>
#include <random>

PrimGenerator::PrimGenerator(const Coord& start)
	:steps_(0)
	,start_(start.x + (1 - (start.x % 2)), start.x + (1 - (start.x % 2)))
{
	std::random_device rd;
	random_.Reseed(rd());
}

/* returns true if there are more steps */
bool PrimGenerator::Step(Maze& maze)
{
	if (steps_ == 0)
	{
		Clear(maze);
		TryAddWalls(start_, maze);
		maze(start_.x, start_.y).cost = 1;
		maze(start_.x, start_.y).color = Color::white();
	}

	if (walls_.size() == 0)
	{
		return false;
	}

	while (walls_.size() != 0)
	{
		int rand = random_.NextInt(0, walls_.size() - 1);
		Coord wall = walls_[rand];
		walls_[rand] = walls_.back();
		walls_.pop_back();
		
		auto horizontal = [this, &maze](const Coord& coord) 
		{
			return(maze(coord.x, coord.y - 1).cost == -1 || maze(coord.x, coord.y + 1).cost == -1);
		};
		auto vertical = [this, &maze](const Coord& coord)
		{
			return(maze(coord.x - 1, coord.y).cost == -1 || maze(coord.x + 1, coord.y).cost == -1);
		};

		if (!vertical(wall) && !horizontal(wall))
		{
			continue;
		}

		Coord node;
		bool vert = wall.x % 2 == 0;
		
		if (vert && vertical(wall))
		{
			if (maze(wall.x - 1, wall.y).cost == -1)
			{
				node = Coord(wall.x - 1, wall.y);
			}
			else
			{
				node = Coord(wall.x + 1, wall.y);
			}
		}
		else if (!vert && horizontal(wall))
		{
			if (maze(wall.x, wall.y - 1).cost == -1)
			{
				node = Coord(wall.x, wall.y - 1);
			}
			else
			{
				node = Coord(wall.x, wall.y + 1);
			}
		}
		else
		{
			continue;
		}
		
		if (maze.IsBorderCoord(node.x, node.y))
		{
			continue;
		}

		maze(node.x, node.y).cost = 1;
		maze(node.x, node.y).color = Color::white();
		maze(wall.x, wall.y).cost = 1;
		maze(wall.x, wall.y).color = Color::white();
		TryAddWalls(node, maze);
		break;
	}

	++steps_;
	return walls_.size() != 0;
}

void PrimGenerator::Clear(Maze& maze)
{
	for (int i = 0; i < maze.Height(); ++i)
	{
		for (int j = 0; j < maze.Width(); ++j)
		{
			auto& tile = maze(j, i);
			tile.color = Color::black();
			tile.cost = -1;
		}
	}
	walls_.clear();
}

void PrimGenerator::Reset(Maze& maze, const Coord& start)
{
	start_ = start;
	steps_ = 0;
}

void PrimGenerator::TryAddWalls(const Coord& coord, const Maze& maze)
{
	Coord coords[4] = 
	{
		{ coord.x - 1, coord.y }, { coord.x + 1, coord.y },
		{ coord.x, coord.y - 1}, { coord.x, coord.y + 1 }
	};

	for (int i = 0; i < 4; ++i)
	{
		if (!maze.IsBorderCoord(coords[i].x, coords[i].y) && std::find(walls_.begin(), walls_.end(), coords[i]) == walls_.end() && (maze(coords[i].x, coords[i].y).cost == -1))
		{
			walls_.push_back(coords[i]);
		}
	}
}
