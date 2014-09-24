#pragma once

#include "Decision.h"
#include <vector>
#include <memory>
#include "Node.h"

class Maze;
class Randomizer;

class Command : public Node
{
public:
	enum Type { BLANK, TURN_LEFT, TURN_RIGHT};
	Command(Type type);

	typedef std::shared_ptr<Command> ptr;
	static ptr make(Type type);

	Command(const Command&) = delete;
	Command& operator=(const Command&) = delete;
	Command(Command&&) = delete;
	Command& operator=(Command&&) = delete;

	Decision evaluate(Maze& maze, Coord current_coord, Decision current_decision) override;
	Node::ptr clone() override;
	Node::ptr select_node(Randomizer& random) override;
	void mutate(Randomizer& random) override;
	void replace(Node::ptr& from, Node::ptr& to) override;
	void format(Formatter& format) override;
private:

	Type type_;
};