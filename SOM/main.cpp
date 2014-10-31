#include <SDL.h>
#include "Window.h"
#include "Controller.h"

int main(int argc, char *argv[])
{
	//create a data set
	Color red, green, blue, yellow, orange, purple, dark_green, dark_blue;

	red.set_red(1);
	red.set_green(0);
	red.set_blue(0);

	green.set_red(0);
	green.set_green(1);
	green.set_blue(0);

	dark_green.set_red(0);
	dark_green.set_green(0.5);
	dark_green.set_blue(0.25);

	blue.set_red(0);
	blue.set_green(0);
	blue.set_blue(1);

	dark_blue.set_red(0);
	dark_blue.set_green(0);
	dark_blue.set_blue(0.5);

	yellow.set_red(1);
	yellow.set_green(1);
	yellow.set_blue(0.2);

	orange.set_red(1);
	orange.set_green(0.4);
	orange.set_blue(0.25);

	purple.set_red(1);
	purple.set_green(0);
	purple.set_blue(1);

	Window win("Self Organizing Map", 400, 400);
	Controller c(400, 400, { red, green, blue, yellow, orange, purple, dark_green, dark_blue });
	win.RenderRectangle(48 * 0, 0, 32, 32, red);
	win.RenderRectangle(48 * 1, 0, 32, 32, green);
	win.RenderRectangle(48 * 2, 0, 32, 32, blue);
	win.RenderRectangle(48 * 3, 0, 32, 32, yellow);
	win.RenderRectangle(48 * 4, 0, 32, 32, orange);
	win.RenderRectangle(48 * 5, 0, 32, 32, purple);
	win.RenderRectangle(48 * 6, 0, 32, 32, dark_green);
	win.RenderRectangle(48 * 7, 0, 32, 32, dark_blue);
	win.PrintScreen("colors.bmp");
	int steps = 0;
	while (win.Open())
	{
		win.PollEvents();
		c.train();
		c.render(win);
		win.PrintScreen("screen_" + std::to_string(steps++) + ".bmp");
		win.Display();

		win.Clear();
	}
	return 0;
}