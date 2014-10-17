#include "Mouse.h"


Mouse::Mouse(World& world, float x, float y)
	:rect_(x, y, 10.f, Color::blue())
	,wanderer_(world)
{
	std::random_device rd;
	random_engine_.seed(rd());
	hunger_meter_ = 10;
	id_ = world.add_object("mouse", [this](float dt){ update(dt); }, [this]() -> const Rect& {return rect_; });
	state_machine_.push_state([this, &world](){ wander(world); });
}

void Mouse::update(float dt)
{
	delta_time_ = dt;
	hunger_meter_ -= dt;
	state_machine_.execute_current();
}

void Mouse::wander(World& world)
{
	if (hunger_meter_ < 5)
	{
		state_machine_.pop_state();
		state_machine_.push_state([this, &world](){ find_cheese(world); });
		//don't waste an update.
		find_cheese(world);
	}
	else
	{
		std::uniform_real_distribution<float> dist(-1.f, 1.f);
		float x = dist(random_engine_);
		float y = dist(random_engine_);
		wanderer_.wander(rect_, delta_time_, 50.f);
	}
}

void Mouse::find_cheese(World& world)
{
	auto cheeses = world.find_with_tag("cheese");
	if (cheeses.size() != 0)
	{
		auto begin = cheeses.begin();
		auto end = cheeses.end();

		auto current = (begin++);
		for (; begin != end; ++begin)
		{
			if (current->second().distance_to(rect_) >
				begin->second().distance_to(rect_))
			{
				current = begin;
			}
		}
		auto closest_cheese = current;
		wanderer_.force_target(rect_, closest_cheese->second().center_x(), closest_cheese->second().center_y());
		wanderer_.wander(rect_, delta_time_, 50.f);
	}
}

void Mouse::eat_cheese(World& world, int cheese_id)
{
	
}

void Mouse::run_away(World& world)
{

}
