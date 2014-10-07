#include "Controller.h"
#include "Random.h"

Controller::Controller(int map_width, int map_height, const std::vector<Color>& training_set)
	:w_(map_width)
	,h_(map_height)
	,som_(map_width, map_height)
	,map_radius_(std::max(map_width, map_height) / 2)
	,iteration_count_(0)
	,base_learning_rate_(0.1f)
	,max_iterations_(10000)
{
	create_data_set(training_set);
}

void Controller::train()
{
	if (is_done())
	{
		return;
	}

	++iteration_count_;

	double time_constant = static_cast<double>(max_iterations_) / std::log(map_radius_);
	double neighbour_hood_radius = map_radius_ * exp(-static_cast<double>(iteration_count_ / time_constant));
	double learning_rate = base_learning_rate_ * exp(-static_cast<double>(iteration_count_/ max_iterations_));

	auto training_p= training_set_[rand_.NextInt(training_set_.size() - 1)];
	auto closest = find_closest_match(training_p);

	std::vector<SOM::node_t*> to_remove;
	for (auto& n : som_)
	{
		if (som_.euclidean_position_distance(n, *closest) < neighbour_hood_radius)
		{
			Color& c = training_p;
			std::array<double, 3> v{ { c.get_red(), c.get_green(), c.get_blue() } };

			double distance = som_.euclidean_position_distance(n, *closest);
			double theta = std::exp(-(distance / neighbour_hood_radius));
			for (int i = 0; i < 3; ++i)
			{
				n[i] = n[i] + theta * learning_rate * (v[i] - n[i]);
			}
		}
	}

	Color& c = training_p;
	std::array<double, 3> v{ { c.get_red(), c.get_green(), c.get_blue() } };
	SOM::node_t& node = *closest;

	double distance = som_.euclidean_position_distance(node, *closest);
	double theta = std::exp(-(distance / neighbour_hood_radius));
	for (int i = 0; i < 3; ++i)
	{
		node[i] = node[i] + theta * learning_rate * (v[i] - node[i]);
	}

}

SOM::node_t* Controller::find_closest_match(Color& color)
{
	SOM::node_t* closest = nullptr;
	for (auto& n : som_)
	{
		for (auto& p : training_set_)
		{
			if (closest == nullptr)
			{
				closest = &n;
			}
			else if (som_.euclidean_color_distance(n, color) < som_.euclidean_color_distance(*closest, color))
			{
				closest = &n;
			}
		}
	}
	return closest;
}

bool Controller::is_done()
{
	return (iteration_count_ > max_iterations_);
}

void Controller::render(Window& window)
{
	double n_w = static_cast<double>(window.Width()) / static_cast<double>(w_) ;
	double n_h = static_cast<double>(window.Heigth()) / static_cast<double>(w_);

	for (auto& n : som_)
	{
		auto color = Color::make_from_floats(n[0], n[1], n[2]).clamp();
		window.RenderRectangle(n.x() * n_w, n.y() * n_h, n_w, n_h, color);
	}
}

void Controller::create_data_set(const std::vector<Color>& training_set)
{
	double x = 0.f;
	double y = 0.f;
	for (Color c : training_set)
	{
		training_set_.emplace_back(c);
	}

	for (auto& n : som_)
	{
		n.x() = x;
		n.y() = y;
		
		for (int i = 0; i < n.dimensionality(); ++i)
		{
			n[i] = rand_.NextDouble(0.f, 1.f);
		}

		x += 1.f;
		if (x >= static_cast<double>(w_))
		{
			x = 0.f;
			y += 1.f;
		}
	}
}