#pragma once

#include "../Maze.h"
#include "../Coord.h"
#include "../Solver.h"

#include <queue>
#include <unordered_set>
#include <map>

class Astar: public Solver
{
public:
	Astar(Coord start, Coord end);

	/* returns true if there are more steps */
	bool Step(Maze& maze) override;
	void Reset() override;
	void Reset(Coord start, Coord end) override;

private:
	int Heuristic(Coord a, Coord b);

	int steps_;
	Coord start_, end_;
	Coord current_;
	Coord walker_;

	class PriorityQueueCompare
	{
	public:
		bool operator()(std::pair<int, Coord>& t1, std::pair<int, Coord>& t2)
		{
			return t1.first > t2.first;
		}
	};

	std::unordered_set<Coord> closed_set_;
	std::unordered_set<Coord> open_set_;
	std::priority_queue<std::pair<int, Coord>, std::vector<std::pair<int, Coord>>, PriorityQueueCompare> open_queue_;
	std::unordered_map<Coord, int> g_;
	std::unordered_map<Coord, int> f_;
	std::unordered_map<Coord, Coord> parent_of_;
	std::vector<Coord> path_;

	Maze came_from_;
};