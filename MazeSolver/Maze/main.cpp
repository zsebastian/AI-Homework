#include <SDL.h>
#include "Window.h"
#include "Timer.h"
#include "Maze.h"
#include "PrimGenerator.h"
#include "Coord.h"
#include "Astar/Astar.h"
#include "PathBasedEvolution/Evolver.h"
#include "DecisionBasedEvolution\DecisionEvolver.h"

Coord RandomCoord(Randomizer& r, Maze& maze)
{
	Coord ret;
	ret = Coord(r.NextInt(1, maze.Width() - 3), r.NextInt(1, maze.Height() - 3));
	while (ret.x % 2 == 0 ||ret.y % 2 == 0)
		ret = Coord(r.NextInt(1, maze.Width() - 3), r.NextInt(1, maze.Height() - 3));
	return ret;
}

int main(int argc, char *argv[])
{
	Window win("Maze solver!", 800, 800);

	std::random_device rd;
	Randomizer rand(rd());

	Coord start, end;
	Maze maze(100, 100);
	PrimGenerator generator(RandomCoord(rand, maze));

	start = RandomCoord(rand, maze);
	end = RandomCoord(rand, maze);
	while (start == end)
		end = RandomCoord(rand, maze);

	DecisionEvolver solver(start, end, 420, 10, 0.5f, 0.01f, 0.50f);

	while (win.Open())
	{
		win.PollEvents();
		while (generator.Step(maze))
			;

		if (!generator.Step(maze))
		{
			if (!solver.Step(maze))
			{
				start = RandomCoord(rand, maze);
				end = RandomCoord(rand, maze);
				while (end == start)
				{
					end = RandomCoord(rand, maze);
				}
				solver.Reset(start, end);
				generator.Reset(maze, RandomCoord(rand, maze));
			}
		}
	
		maze.Render(win, 0, 0, 800, 800);
		
		win.Display();
	}
	return 0;
}