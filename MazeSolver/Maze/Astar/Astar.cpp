#include "Astar.h"

Astar::Astar(Coord start, Coord end)
	:start_(start)
	,end_(end)
	,current_(start)
	,steps_(0)
{

}


bool Astar::Step(Maze& maze)
{
	if (steps_ == 0)
	{
		came_from_ = Maze(maze.Width(), maze.Height());
		maze(start_.x, start_.y).color = Color::red();
		maze(end_.x, end_.y).color = Color::green();

		Reset();
	}
	
	++steps_;

	bool done = false;

	if (open_queue_.size() != 0)
	{
		current_ = open_queue_.top().second;

		if (current_ != end_)
		{
			open_queue_.pop();
			open_set_.erase(current_);
			closed_set_.insert(current_);
			Coord neighbours[] =
			{
				{ current_.x - 1, current_.y }, { current_.x + 1, current_.y },
				{ current_.x, current_.y - 1 }, { current_.x, current_.y + 1 }
			};

			for (auto& n : neighbours)
			{
				if (closed_set_.count(n) == 0 && !maze.IsBorderCoord(n.x, n.y) && maze(n.x, n.y).cost >= 0)
				{
					int tentative_g = g_[current_] + maze(current_.x, current_.y).cost;
					if (open_set_.count(n) == 0 || tentative_g < g_[n])
					{
						came_from_(n.x, n.y).cost = 1;
						if (n != end_)
						{
							maze(n.x, n.y).color = Color::blue();
						}
						if (open_set_.count(n) == 0)
						{
							parent_of_.insert(std::make_pair(n, current_));
							g_.insert(std::make_pair(n, tentative_g));
							f_.insert(std::make_pair(n, Heuristic(n, end_)));
							open_set_.insert(n);
							open_queue_.push(std::make_pair(Heuristic(n, end_), n));
						}
						else
						{
							parent_of_[n] = current_;
							g_[n] = tentative_g;
							f_[n] = Heuristic(n, end_);
						}
					}
				}
			}
		}
	}

	if (current_ == end_)
	{
		if (walker_ != start_)
		{
			walker_ = parent_of_[walker_];
			if (walker_ != end_ && walker_ != start_)
			{
				path_.push_back(walker_);
				maze(walker_.x, walker_.y).color = Color::make_from_bytes(127, 127, 255);
			}
		}
		else if (!path_.empty())
		{
			if (steps_ % 5 != 0)
			{
				return true;
			}
			Coord p = path_.back();
			maze(p.x, p.y).color = Color::make_from_bytes(127, 127, 255);
			path_.pop_back();
			if (!path_.empty())
			{
				Coord p = path_.back();
				maze(p.x, p.y).color = Color::make_from_bytes(0, 255, 255);
			}
		}
		else
		{
			return false;
		}
	}

	return !done;
}

void Astar::Reset(Coord start, Coord end)
{
	start_ = start;
	end_ = end;
	Reset();
}

void Astar::Reset()
{
	for (int i = 0; i < came_from_.Height(); i++)
	{
		for (int j = 0; j < came_from_.Width(); ++j)
		{
			came_from_(j, i).cost = -1;
		}
	}
	current_ = start_;
	walker_ = end_;
	steps_ = 0;
	g_.clear();
	f_.clear();
	path_.clear();
	while (!open_queue_.empty())
	{
		open_queue_.pop();
	}
	closed_set_.clear();
	open_set_.clear();
	g_.insert(std::make_pair(start_, 0));
	f_.insert(std::make_pair(start_, Heuristic(start_, end_)));
	open_queue_.push(std::make_pair(Heuristic(start_, end_), start_));
	open_set_.insert(start_);
	parent_of_.clear();
}


int Astar::Heuristic(Coord a, Coord b)
{
	//Manhattan distance
	return std::abs((a.x - b.x)) + std::abs((a.y - b.y));
}
