#include <SDL.h>
#include "Window.h"
#include "Board.h"
#include "Timer.h"
#include "SolverEvolver.h"

int handle_input(Board&, Window&);

void render_piece(Piece& piece, Window& window)
{
	Color color = piece.get_color();
	auto tiles = piece.get_tiles();
	for (int i = 0; i < PIECE_SIZE; ++i)
	{
		for (int j = 0; j < PIECE_SIZE; ++j)
		{
			int x = piece.get_x() + i;
			int y = piece.get_y() + j;
			if (tiles[j * PIECE_SIZE + i] != 0)
			window.RenderRectangle(x * 16, y * 16, 16, 16, color);
		}
	}
}

int main(int argc, char *argv[])
{
	Window win("Tetris!", 5 * 16 * 12, 2 * 16 * 22);
	win.MapKey(SDLK_UP, "up");
	win.MapKey(SDLK_DOWN, "down");
	win.MapKey(SDLK_LEFT, "left");
	win.MapKey(SDLK_RIGHT, "right");

	SolverEvolver solver(10, 0.5f, 0.05f);

	float rot = 0.0f;

	Timer timer;
	timer.Start();
	int tick_time = 1000;

	win.Clear();

	std::vector<Piece> pieces;

	pieces.push_back(Piece::make_I(5 * 0, 0, 1));
	pieces.push_back(Piece::make_J(5 * 1 - 1, 1, 3));
	pieces.push_back(Piece::make_L(5 * 2 - 3, 1, 1));
	pieces.push_back(Piece::make_O(5 * 3 - 3, 1, 0));
	pieces.push_back(Piece::make_S(5 * 4 - 4, 1, 0));
	pieces.push_back(Piece::make_T(5 * 5 - 4, 1, 2));
	pieces.push_back(Piece::make_Z(5 * 6 - 4, 1, 0));

	for (auto& p : pieces)
	{
		render_piece(p, win);
	}

	win.PrintScreen("Tetromins.bmp");
	win.Display();
	while (win.Open())
	{
		win.PollEvents();
	}
		
	while (win.Open())
	{
		win.PollEvents();
		win.Clear();
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