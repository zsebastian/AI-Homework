#include "Command.h"
#include "../Random.h"
#include <algorithm>

Command::ptr Command::make(Command::Type type)
{
	return std::make_shared<Command>(type);
}


Command::Command(Command::Type type)
{
	type_ = type;
}

Decision Command::evaluate(Maze& maze, Coord current_coord, Decision current_decision)
{
	switch (type_)
	{
	case Command::BLANK:
		break;
	case Command::TURN_LEFT:
		current_decision.perform(Decision::TURN_LEFT);
		break;
	case Command::TURN_RIGHT:
		current_decision.perform(Decision::TURN_RIGHT);
		break;
	default:
		break;
	}
	return current_decision;
}

Node::ptr Command::select_node(Randomizer& random)
{
	return get_this();
}

Node::ptr Command::clone()
{	
	return std::move(Command::make(type_));
}

void Command::mutate(Randomizer& random)
{
	type_ = static_cast<Command::Type>(random.NextInt(0, 2));
}

void Command::replace(Node::ptr& from, Node::ptr& to)
{

}