#include "CodeTurtle.h"
#include "Command.h"
#include "../Maze.h"

CodeTurtle::CodeTurtle()
{
	base_ = Command::make(Command::BLANK);
	steps_ = 0;
}

int CodeTurtle::step(Maze& maze)
{
	current_decision_ = base_->evaluate(maze, current_coord_, current_decision_);
	Coord next = current_decision_.forward(current_coord_);
	
	if (maze(next.x, next.y).cost != -1)
	{
		current_coord_ = next;
	}
	++steps_;
	return steps_;
}

void CodeTurtle::set_start_position(Coord coord)
{
	steps_ = 0;
	current_coord_ = coord;
}

const Node::ptr& CodeTurtle::get_base() const
{
	return base_;
}

Node::ptr& CodeTurtle::get_base()
{
	return base_;
}

Decision CodeTurtle::get_current_decision() const
{
	return current_decision_;
}	

Coord CodeTurtle::get_current_coord() const
{
	return current_coord_;
}