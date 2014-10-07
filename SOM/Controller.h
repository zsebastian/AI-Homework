#pragma once

#include "Window.h"
#include <vector>
#include "SOM.h"
#include "Coord.h"
#include "Random.h"


class Controller
{
public:
	Controller(int map_width, int map_height, const std::vector<Color>& training_set_);
	
	void train();
	bool is_done();
	void render(Window& window);
private:
	void create_data_set(const std::vector<Color>& training_set_);

	/* first is the color, second is a pointer to the closest node */
	std::vector<Color> training_set_;
	SOM::node_t* find_closest_match(Color& color);

	SOM som_;
	int w_, h_;
	double map_radius_;
	int iteration_count_, max_iterations_;
	double base_learning_rate_;
	
	Randomizer rand_;
};