#pragma once

#include "Rect.h"
#include "StateMachine.h"
#include "World.h"
#include <random>
#include "Wanderer.h"

class Mouse
{
public:
	Mouse(World& world, float x, float y);
private:
	void update(float dt);

	void wander(World&);
	void find_cheese(World&);
	void eat_cheese(World&, int cheese_id);
	void run_away(World&);

	StateMachine state_machine_;

	Rect rect_;
	float delta_time_;
	float hunger_meter_;
	
	Wanderer wanderer_;

	std::mt19937 random_engine_;

	int id_;
};