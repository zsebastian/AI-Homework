#include <SDL.h>
#include "Window.h"

int main(int argc, char *argv[])
{
	Window win("Self Organizing Map", 800, 800);
	while (win.Open())
	{
		win.PollEvents();
		
		win.Display();
		win.Clear();
	}
	return 0;
}