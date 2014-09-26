#include "ControlBlock.h"
#include "../Random.h"
#include <algorithm>
#include "Command.h"

ControlBlock::ptr ControlBlock::make()
{
	return std::make_shared<ControlBlock>();
}


ControlBlock::ControlBlock()
{
	reporter_ = std::make_unique<TrueReporter>();
	if_ = Command::make(Command::BLANK);
	else_ = Command::make(Command::BLANK);
}

Decision ControlBlock::evaluate(Maze& maze, Coord current_coord, Decision current_decision)
{
	if (reporter_->evaluate(maze, current_coord, current_decision))
	{
		return if_->evaluate(maze, current_coord, current_decision);
	}
	else
	{
		return else_->evaluate(maze, current_coord, current_decision);
	}
	
}

Node::ptr ControlBlock::select_node(Randomizer& random)
{
	int r = random.NextInt(0, 2);
	if (r == 0)
	{
		return get_this();
	}
	else if (r == 1)
	{
		return if_;
	}
	else
	{
		return else_;
	}
}

Node::ptr ControlBlock::clone()
{
	ControlBlock::ptr copy = std::make_shared<ControlBlock>();
	copy->reporter_ = reporter_->clone();
	copy->if_ = if_->clone();
	copy->else_ = else_->clone();
	return std::move(copy);
}

void ControlBlock::mutate(Randomizer& random, int depth)
{
	//half the time randomize the reporter
	if (random.NextInt(0, 1) == 0 || dynamic_cast<TrueReporter*>(reporter_.get()) != nullptr)
	{
		reporter_ = Reporter::make(random);
	}
	//half the time randomize the if
	if (random.NextInt(0, 1) == 0)
	{
		if (random.NextInt(0, 1) == 0)
		{
			if_ = Node::make_random(random);
		}
		if (depth > 0)
		{
			if_->mutate(random, depth - 1);
		}
	}
	//half the time randomize the else
	if (random.NextInt(0, 1) == 0)
	{
		//half the time randomize a type
		if (random.NextInt(0, 1) == 0)
		{
			else_ = Node::make_random(random);
		}
		if (depth > 0)
		{
			else_->mutate(random, depth);
		}
	}
}

void ControlBlock::replace(Node::ptr& from, Node::ptr& to)
{
	//can't replace self
	if (this != from.get())
	{
		if (if_ == from)
		{
			if_ = to;
		}
		else if (else_ == from)
		{
			else_ = to;
		}
		else
		{
			if_->replace(from, to);
			else_->replace(from, to);
		}
		
	}
}

void ControlBlock::format(Formatter& format)
{
	format.append("if (" + reporter_->to_string() + "?)");
	format.push_indent();
	if_->format(format);
	format.pop_indent();
	format.append("else");
	format.push_indent();
	else_->format(format);
	format.pop_indent();
}

int ControlBlock::max_depth()
{
	return std::max(else_->max_depth(), if_->max_depth()) + 1;
}