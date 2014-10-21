#include <SDL.h>
#include "Window.h"
#include "Board.h"
#include "Timer.h"
#include "SolverEvolver.h"

int handle_input(Board&, Window&);

int main(int argc, char *argv[])
{
	Window win("Tetris!", 5 * 16 * 12, 2 * 16 * 22);
	win.MapKey(SDLK_UP, "up");
	win.MapKey(SDLK_DOWN, "down");
	win.MapKey(SDLK_LEFT, "left");
	win.MapKey(SDLK_RIGHT, "right");

	SolverEvolver solver(10, 0.5f, 0.01f);

	float rot = 0.0f;

	Timer timer;
	timer.Start();
	int tick_time = 1000;

	while (win.Open())
	{
		win.PollEvents();
		
		/*
		tick_time = handle_input(board, win);
		
		if (timer.ElapsedMilliseconds() > tick_time)
		{
			int tick = board.tick();

			if (tick == -1)
			{
				win.Exit();
			}
			timer.Start();
		}*/
		solver.update(win);

		win.Display();
	}
	return 0;
}

int handle_input(Board& board, Window& win)
{
	int tick_time;

	if (win.GetKey("up").pressed)
	{
		board.perform_action(Board::Rotate);
	}

	if (win.GetKey("down").down)
	{
		tick_time = 100;
	}
	else
	{
		tick_time = 500;
	}

	if (win.GetKey("left").pressed)
	{
		board.perform_action(Board::Left);
	}
	if (win.GetKey("right").pressed)
	{
		board.perform_action(Board::Right);
	}
	return tick_time;
}