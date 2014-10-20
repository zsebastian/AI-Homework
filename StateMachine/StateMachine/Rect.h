#pragma once

#include "Color.h"

class Rect
{
public:
	Rect(float cx, float cy, float extends, Color color)
		:cx_(cx), cy_(cy), extends_(extends), color_(color)
	{}

	float center_x() const
	{
		return cx_;
	}

	float center_y() const
	{
		return cy_;
	}

	float extends() const
	{
		return extends_;
	}

	void move(float dx, float dy)
	{
		cx_ += dx;
		cy_ += dy;
	}

	float distance_to(const Rect& other) const
	{
		float x = cx_ - other.cx_;
		float y = cy_ - other.cy_;
		float len = std::sqrtf((x * x) + (y * y));
		return len;
	}

	const Color& color() const
	{
		return color_;
	}

	/* because lazy */
	bool circular_collision(const Rect& other) const
	{
		float extends = other.extends() + extends_;
		return distance_to(other) < extends;
	}

public:
	float cx_, cy_, extends_;
	Color color_;
};