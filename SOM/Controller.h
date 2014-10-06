#pragma once

#include "Window.h"
#include <vector>
#include "SOM.h"
#include "Coord.h"

class Controller
{
public:
	Controller(int map_width, int map_height, int nodes_x, int nodes_y);
	
	void train();
	bool is_done();
	void render(Window& window);
private:
	void create_data_set();
	SOM som_;
	int w_, h_;
};