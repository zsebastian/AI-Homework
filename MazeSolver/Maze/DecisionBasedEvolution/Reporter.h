#pragma once

#include "../Coord.h"
#include <memory>
#include "Decision.h"
#include <string>

class Maze;
class Randomizer;

class Reporter
{
public:
	typedef std::unique_ptr<Reporter> ptr;
	static ptr make(Randomizer& random);

	virtual ~Reporter() {};
	virtual bool evaluate(Maze& maze, Coord current_coord, Decision decision) = 0;
	virtual ptr clone() = 0;
	virtual std::string to_string() const;
protected:
	bool evaluate_helper(Maze& maze, Coord current_coord, Decision decision);
};

class WallAhead : public Reporter
{
	bool evaluate(Maze& maze, Coord current_coord, Decision decision) override;
	std::string to_string() const override;
	ptr clone() override;
};

class WallLeft : public Reporter
{
	bool evaluate(Maze& maze, Coord current_coord, Decision decision) override;
	std::string to_string() const override;
	ptr clone() override;
};

class WallRight : public Reporter
{
	bool evaluate(Maze& maze, Coord current_coord, Decision decision) override;
	std::string to_string() const override;
	ptr clone() override;
};

class TrueReporter : public Reporter
{
	bool evaluate(Maze& maze, Coord current_coord, Decision decision) override;
	std::string to_string() const override;
	ptr clone() override;
};