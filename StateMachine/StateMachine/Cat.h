#pragma once

#include "Rect.h"
#include "StateMachine.h"
#include "World.h"
#include <random>
#include "Wanderer.h"

class Cat
{
public:
	Cat(World& world, float x, float y);
private:
	void update(float dt);

	void wander(World&);

	StateMachine state_machine_;

	Rect rect_;
	float delta_time_;
	float hunger_meter_;

	Wanderer wanderer_;

	std::mt19937 random_engine_;

	int id_;
};