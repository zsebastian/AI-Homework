#pragma once

#include "Decision.h"
#include <vector>
#include <memory>
#include "Node.h"

class Maze;
class Randomizer;

class CodeBlock: public Node
{
public:
	typedef std::shared_ptr<CodeBlock> ptr;
	static ptr make();

	CodeBlock();

	CodeBlock(const CodeBlock&) = delete;
	CodeBlock& operator=(const CodeBlock&) = delete;
	CodeBlock(CodeBlock&&) = delete;
	CodeBlock& operator=(CodeBlock&&) = delete;

	Decision evaluate(Maze& maze, Coord current_coord, Decision current_decision) override;
	Node::ptr clone() override;
	Node::ptr select_node(Randomizer& random) override;
	void mutate(Randomizer& random) override;
	void replace(Node::ptr& from, Node::ptr& to) override;
	void format(Formatter& format) override;
private:

	std::vector<Node::ptr> children_;
};