#pragma once

#include "Node.h"
#include "Decision.h"

class CodeTurtle
{
public:
	CodeTurtle();

	//returns amount of steps since latest start position reset
	int step(Maze& maze);
	void set_start_position(Coord coord);

	const Node::ptr& get_base() const;
	Node::ptr& get_base();

	Decision get_current_decision() const;
	Coord get_current_coord() const;

	void set_fitness(int fitness) { fitness_ = fitness; }
	int get_fitness() const { return fitness_; }
private:
	int steps_;
	int fitness_;
	Node::ptr base_;
	Decision current_decision_;
	Coord current_coord_;
};