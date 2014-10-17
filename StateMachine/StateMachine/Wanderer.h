#pragma once

#include <utility>
#include "Rect.h"
#include "World.h"
#include <random>

class Wanderer
{
public:
	Wanderer(World& world);
	void wander(Rect& rect, float dt, float speed);
	void reset();
	void force_target(const Rect& rect, float x, float y);
private:
	float target_x_, target_y_;
	float start_x_, start_y_;
	float wander_len_;
	float target_wander_len_;

	bool has_target_;
	std::function<std::pair<float, float>()> random_coord_;

	std::mt19937 random_;
	std::uniform_real_distribution<float> dist_x_;
	std::uniform_real_distribution<float> dist_y_;
};
