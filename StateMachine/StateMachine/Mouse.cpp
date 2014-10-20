#include "Mouse.h"


Mouse::Mouse(World& world, float x, float y)
	:rect_(x, y, 10.f, Color::blue())
	,wanderer_(world)
{
	timer_.Start();
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
;
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

	detect_cat(world);

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
		float distance = rect_.distance_to(closest_cheese->second());
		if (rect_.circular_collision(closest_cheese->second()))
		{
			int elapsed = timer_.ElapsedMilliseconds();
			int id = closest_cheese->first;
			state_machine_.pop_state();
			state_machine_.push_state([this, &world, id, elapsed](){ eat_cheese(world, id, elapsed); });
			//don't waste an update.
			eat_cheese(world, id, elapsed);
		}
	}

	detect_cat(world);
}

void Mouse::eat_cheese(World& world, int cheese_id, int start_time)
{

	if (timer_.ElapsedMilliseconds() - start_time > 1000)
	{
		world.remove_object(cheese_id);
		hunger_meter_ += 5;
		if (hunger_meter_ < 5)
		{
			state_machine_.pop_state();
			state_machine_.push_state([this, &world](){ find_cheese(world); });
			//don't waste an update.
			find_cheese(world);
		}
		else
		{
			state_machine_.pop_state();
			state_machine_.push_state([this, &world](){ wander(world); });
			//don't waste an update.
			wander(world);
		}
	}
	
	detect_cat(world);
}

void Mouse::run_away(World& world, std::function<const Rect&()> rect_func)
{
	const Rect& rect = rect_func();
	if (rect.distance_to(rect_) < 400.f)
	{
		wanderer_.force_reverse_target(rect_, rect.center_x(), rect.center_y());
		wanderer_.wander(rect_, delta_time_, 100.f);
	}
	else
	{
		state_machine_.pop_state();
	}
}

void Mouse::detect_cat(World& world)
{
	auto cats = world.find_with_tag("cat");
	if (cats.size() != 0)
	{
		for (auto& cat : cats)
		{
			if (cat.second().distance_to(rect_) < 200.f)
			{
				auto func = cat.second;
				state_machine_.push_state([this, &world, func](){ run_away(world, func); });
				break;
			}
		}
	}
}