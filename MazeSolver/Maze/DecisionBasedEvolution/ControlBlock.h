#pragma once 

#include "Decision.h"
#include <vector>
#include <memory>
#include "Node.h"
#include "Reporter.h"

class Maze;
class Randomizer;

class ControlBlock : public Node
{
public:
	typedef std::shared_ptr<ControlBlock> ptr;
	static ptr make();

	ControlBlock();

	ControlBlock(const ControlBlock&) = delete;
	ControlBlock& operator=(const ControlBlock&) = delete;
	ControlBlock(ControlBlock&&) = delete;
	ControlBlock& operator=(ControlBlock&&) = delete;

	Decision evaluate(Maze& maze, Coord current_coord, Decision current_decision) override;
	Node::ptr clone() override;
	Node::ptr select_node(Randomizer& random) override;
	void mutate(Randomizer& random) override;
	void replace(Node::ptr& from, Node::ptr& to) override;
	void format(Formatter& format) override;
private:

	Node::ptr if_;
	Node::ptr else_;
	Reporter::ptr reporter_;
};