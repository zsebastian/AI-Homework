#pragma once

#include "Rect.h"
#include "StateMachine.h"
#include "World.h"
#include <random>
#include "Wanderer.h"
#include "Timer.h"

class Mouse
{
public:
	Mouse(World& world, float x, float y);
private:
	void update(float dt);

	void wander(World&);
	void find_cheese(World&);
	void eat_cheese(World&, int cheese_id, int start_time);
	void run_away(World&, std::function<const Rect&()> rect);
	void detect_cat(World&);

	StateMachine state_machine_;

	Rect rect_;
	float delta_time_;
	float hunger_meter_;
	
	Wanderer wanderer_;

	std::mt19937 random_engine_;
	Timer timer_;

	int id_;
};