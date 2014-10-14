#include <lua.hpp>
#include <luacppinterface.h>
#include <string>
#include <fstream>
#include <iostream>
#include "LuaHelper.h"
#include "Window.h"
#include "Timer.h"

void render_robots(Window& window, const LuaHelper& helper)
{
	for (int i = 0; i < helper.get_robot_count(); ++i)
	{
		Robot robot = helper.get_robot_at(i);
		window.RenderRectangle(robot.x * 8, robot.y * 8, 8, 8, Color::make_from_floats(robot.r, robot.g, robot.b));
	}
}

int main(int argc, char **argv)
{
	LuaHelper helper(800 / 8, 800 / 8);
	Window window("robot wars", 800, 800);
	helper.run_script("robots.lua");
	
	Timer timer;
	timer.Start();

	while (window.Open())
	{
		window.PollEvents();
		if (timer.ElapsedMilliseconds() > 1000 / 10)
		{
			timer.Start();
			helper.update_robots();
		}
		render_robots(window, helper);
		window.Display();
		window.Clear();
	}

	return 0;
}