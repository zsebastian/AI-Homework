#pragma once

#include <utility>

struct Coord
{
	Coord(int x_, int y_)
		:x(x_), y(y_)
	{}

	Coord() {};

	int x, y;

	bool operator==(const Coord& other) const
	{
		return other.x == x && other.y == y;
	}

	bool operator!=(const Coord& other) const
	{
		return !(*this == other);
	}

	bool operator<(const Coord& other) const
	{
		return (x < other.x) || (x == other.x && y < other.y);
	}

	static int manhattan_distance(const Coord& lhs, const Coord& rhs)
	{
		return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y);
	}
};

namespace std {
	template <> struct hash<Coord>
	{
		size_t operator()(const Coord& c) const
		{
			size_t seed = std::hash<int>()(c.x);
			return std::hash<int>()(c.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
	};
}
