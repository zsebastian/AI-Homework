#include <SDL.h>
#include "Window.h"
#include "Timer.h"
#include "Maze.h"
#include "PrimGenerator.h"
#include "Coord.h"
#include "Astar/Astar.h"
#include "PathBasedEvolution/Evolver.h"
#include "DecisionBasedEvolution\DecisionEvolver.h"

int astar_main();
int decision_based_main();

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
	return astar_main();
}

void print_maze(int w, int h, const std::string& filename, Window& win)
{
	std::random_device rd;
	Randomizer rand(rd());

	Maze maze(w, h);
	PrimGenerator generator(RandomCoord(rand, maze));
	while (generator.Step(maze));
	win.Clear();
	maze.Render(win, 0, 0, 800, 800);
	win.Display();
	win.PrintScreen(filename);
	win.Clear();
}

int astar_main()
{
	Window win("Maze solver!", 800, 800);

	std::random_device rd;
	Randomizer rand(rd());

	Coord start, end;
	Maze maze(51, 51);
	PrimGenerator generator(RandomCoord(rand, maze));

	start = RandomCoord(rand, maze);
	end = RandomCoord(rand, maze);
	while (start == end)
		end = RandomCoord(rand, maze);

	Astar solver(start, end);

	int step = 0;

	while (win.Open())
	{
		win.PollEvents();
		bool generated = false;
		bool solved = false;
		if (!generator.Step(maze))
		{
			generated = true;
			if (!solver.Step(maze))
			{
				solved = true;
				std::cout << "SOLVED!" << std::endl;
				maze = Maze(maze.Width() + 2, maze.Height() + 2);
				generator.Reset(maze, RandomCoord(rand, maze));

				start = RandomCoord(rand, maze);
				end = RandomCoord(rand, maze);
				while (end == start)
				{
					end = RandomCoord(rand, maze);
				}

			}
		}
	

		maze.Render(win, 0, 0, 800, 800);
	
		win.Display();
		if (generated && !solved)
		{
			win.PrintScreen("step_" + std::to_string(step++) + ".bmp");
		}
		win.Clear();

	}
	return 0;
}

int decision_based_main()
{
	Window win("Maze solver!", 800, 800);

	std::random_device rd;
	Randomizer rand(rd());
	print_maze(100, 100, "maze.bmp", win);

	Coord start, end;
	Maze maze(11, 11);
	PrimGenerator generator(RandomCoord(rand, maze));

	start = RandomCoord(rand, maze);
	end = RandomCoord(rand, maze);
	while (start == end)
		end = RandomCoord(rand, maze);

	DecisionEvolver solver(start, end, maze.Width() * maze.Height(), 100, 0.5f, 0.01f);
	//Astar solver(start, end);
	long long step = 0;
	system("mkdir screens");
	while (win.Open())
	{
		win.PollEvents();
		while (generator.Step(maze))
			;

		if (!generator.Step(maze))
		{
			if (!solver.Step(maze))
			{
				std::cout << "SOLVED!" << std::endl;
				maze = Maze(maze.Width() + 2, maze.Height() + 2);
				generator.Reset(maze, RandomCoord(rand, maze));

				start = RandomCoord(rand, maze);
				end = RandomCoord(rand, maze);
				while (end == start)
				{
					end = RandomCoord(rand, maze);
				}
				solver.NewMaze(start, end, maze.Width() * maze.Height() / 2);
			}
		}

		maze.Render(win, 0, 0, 800, 800);
		
		win.Display();
		if (step % 10 == 0)
			win.PrintScreen("screens/screen_" + std::to_string(step) + ".bmp");
		step++;
		win.Clear();

	}
	return 0;
}
