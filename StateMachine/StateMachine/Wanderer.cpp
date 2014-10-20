#include "Wanderer.h"
#include <random>

Wanderer::Wanderer(World& world)
	: has_target_(false)
	, dist_x_(0.f, world.width())
	, dist_y_(0.f, world.height())
{
	std::random_device rd;
	random_.seed(rd());
	random_coord_ = [this]{ return std::make_pair(dist_x_(random_), dist_y_(random_)); };
}

void Wanderer::reset()
{
	has_target_ = false;
}

void Wanderer::force_target(const Rect& rect, float x, float y)
{
	has_target_ = true;
	
	target_x_ = x;
	target_y_ = y;
	start_x_ = rect.center_x();
	start_y_ = rect.center_y();
	x = target_x_ - start_x_;
	y = target_y_ - start_y_;
	float len = std::sqrtf((x * x) + (y * y));
	target_wander_len_ = len;
	wander_len_ = 0.f;
}

void Wanderer::force_reverse_target(const Rect& rect, float x, float y)
{
	has_target_ = true;
	
	start_x_ = rect.center_x();
	start_y_ = rect.center_y();

	target_x_ = start_x_ - x;
	target_y_ = start_y_ - y;
	
	x = target_x_ - start_x_;
	y = target_y_ - start_y_;
	float len = std::sqrtf((x * x) + (y * y));
	target_wander_len_ = len;
	wander_len_ = 0.f;
}

void Wanderer::wander(Rect& rect, float dt, float speed)
{
	if (!has_target_)
	{
		auto target = random_coord_();
		force_target(rect, target.first, target.second);
	}
	float x = target_x_ - rect.center_x();
	float y = target_y_ - rect.center_y();

	float len = std::sqrtf((x * x) + (y * y));
	x = (x / len) * dt * speed;
	y = (y / len) * dt * speed;

	rect.move(x, y);
	len = std::sqrtf((x * x) + (y * y));
	wander_len_ += len;

	if (wander_len_ > target_wander_len_)
	{
		has_target_ = false;
	}
}
