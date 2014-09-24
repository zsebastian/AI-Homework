#include "Maze.h"
#include <algorithm>
#include <cassert>

Maze::Maze(int w, int h)
	:w_(w)
	,h_(h)
	,maze_(w * h)
	,surface_(nullptr)
{
}

Maze::Maze()
	:w_(0)
	, h_(0)
	,surface_(nullptr)
{}

Maze::~Maze()
{
	if (surface_ != nullptr)
	{
		SDL_FreeSurface(surface_);
	}
}

Maze::Maze(const Maze& other)
{
	InnerCopy(other);
}

Maze& Maze::operator=(const Maze& other)
{
	if (&other != this)
	{
		InnerCopy(other);
	}
	return *this;
}

void Maze::InnerCopy(const Maze& maze)
{
	w_ = maze.w_;
	h_ = maze.h_;;
	std::swap(maze_, std::vector<Tile>(h_ * w_));
	surface_ = nullptr;
}

void Maze::Render(Window& window, const int x, const int y, const int w, const int h)
{
	if (surface_ == nullptr)
	{
		surface_ = window.NewSurface(w_, h_);
	}

	int tile_w = w / w_;
	int tile_h = h / h_;

	for (int i = 0; i < h_; ++i)
	{
		for (int j = 0; j < w_; ++j)
		{
			window.SetPixel(surface_, j, i, window.ToPixel(Get(j, i).color));
		}
	}
	window.RenderSurface(surface_, x, y, w, h);
}

Maze::Tile& Maze::operator()(int x, int y)
{
	return Get(x, y);
}

const Maze::Tile& Maze::operator()(int x, int y) const
{
	return Get(x, y);
}

Maze::Tile& Maze::Get(int x, int y)
{
	EnsureRange(x, y);
	return maze_[y * w_ + x];
}

const Maze::Tile& Maze::Get(int x, int y) const
{
	EnsureRange(x, y);;
	return maze_[y * w_ + x];
}


bool Maze::IsBorderCoord(const int x, const int y) const
{
	bool ret = (x <= 0 || y <= 0 || x >= w_- 1 || y >= h_ - 1);
	return ret;
}
