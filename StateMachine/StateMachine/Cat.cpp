#include "Cat.h"


Cat::Cat(World& world, float x, float y)
	:rect_(x, y, 10.f, Color::red())
	, wanderer_(world)
{
	std::random_device rd;
	random_engine_.seed(rd());
	hunger_meter_ = 10;
	id_ = world.add_object("cat", [this](float dt){ update(dt); }, [this]() -> const Rect&{ return rect_; });
	state_machine_.push_state
	(
		[this, &world](){ wander(world); 
	});
}

void Cat::update(float dt)
{
	delta_time_ = dt;
	state_machine_.execute_current();
}

void Cat::wander(World& world)
{
	std::uniform_real_distribution<float> dist(-1.f, 1.f);
	float x = dist(random_engine_);
	float y = dist(random_engine_);
	wanderer_.wander(rect_, delta_time_, 50.f);
}
