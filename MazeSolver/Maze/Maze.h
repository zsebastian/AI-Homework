#pragma once

#include "Window.h"

class Maze
{
public:
	struct Tile
	{
		Tile()
			 :cost(-1)
			, color(Color::black())
		{
		}

		Tile(int cost_, Color color_)
			:cost(cost_)
			,color(color_)
		{
		}
		//Any negative cost means the tile is solid/closed/occupied etc..
		int cost;
		Color color;
	};
	Maze(int w, int h);
	Maze();
	Maze(const Maze& other);
	Maze& operator=(const Maze& other);

	void Render(Window& window, int x, int y, int w, int h);
	~Maze();
	
	Tile& operator()(int x, int y);
	const Tile& operator()(int x, int y) const;

	const int Width() const { return w_; }
	const int Height() const { return h_; }

	bool IsBorderCoord(int x, int y) const;

private:
	Tile& Get(int x, int y);
	const Tile& Get(int x, int y) const;

	inline bool EnsureRange(int x, int y) const { return x >= 0 && x < w_ && y >= 0 && y < h_; };

	void InnerCopy(const Maze& other);

	std::vector<Tile> maze_;
	int w_, h_;
	SDL_Surface* surface_;
};