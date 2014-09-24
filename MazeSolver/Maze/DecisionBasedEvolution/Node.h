#pragma once

#include "../Coord.h"
#include "Decision.h"
#include <vector>
#include <memory>
#include "Formatter.h"

class Maze;
class Randomizer;

class Node : std::enable_shared_from_this<Node>
{
public:
	typedef std::shared_ptr<Node> ptr;
	//returns a random node (with no child nodes)
	static ptr make_random(Randomizer& random);

	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;
	Node(Node&&) = delete;
	Node& operator=(Node&&) = delete;

	virtual Decision evaluate(Maze& maze, Coord current_coord, Decision current_decision) = 0;
	virtual Node::ptr clone() = 0;
	virtual void mutate(Randomizer& random) = 0;
	virtual Node::ptr select_node(Randomizer& random) = 0;
	virtual void replace(Node::ptr& from, Node::ptr& to) = 0;
	virtual void format(Formatter& format) = 0;
		
	Node::ptr get_this();
protected:
	Node() {};
};