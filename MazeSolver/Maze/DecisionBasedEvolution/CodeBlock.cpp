#include "CodeBlock.h"
#include "../Random.h"
#include <algorithm>

CodeBlock::ptr CodeBlock::make()
{
	return std::make_shared<CodeBlock>();
}

CodeBlock::CodeBlock()
{

}

Decision CodeBlock::evaluate(Maze& maze, Coord current_coord, Decision current_decision)
{
	for (Node::ptr n : children_)
	{
		current_decision = n->evaluate(maze, current_coord, current_decision);
	}
	return current_decision;
}

Node::ptr CodeBlock::select_node(Randomizer& random)
{
	//Only select self if there are no children.
	if (children_.size() == 0)
	{
		return get_this();
	}

	//select a child at random                                                                                                     
	int index = random.NextInt(0, children_.size() - 1);
	Node::ptr s = children_[index];
	//select that child half the time, select a childs child half the time.
	if (random.NextInt(0, 1) == 0)
	{
		return s;
	}
	else
	{
		return s->select_node(random);
	}
}

Node::ptr CodeBlock::clone()
{
	CodeBlock::ptr copy = std::make_shared<CodeBlock>();
	for (Node::ptr n : children_)
	{
		copy->children_.push_back(std::move(n->clone()));
	}
	return std::move(copy);
}

void CodeBlock::mutate(Randomizer& random)
{
	int size = children_.size();
	if (size != 0)
	{
		auto rand = std::bind(static_cast<int(Randomizer::*)(int, int)>(&Randomizer::NextInt), &random, 0, size - 1);
		//shuffle nodes
		std::random_shuffle(children_.begin(), children_.end(), rand);
		int rem = rand(0, size);
		//erase a random amount of nodes
		children_.erase(children_.begin() + rem, children_.end());
		//mutate some of the remaining children
		for (auto& child : children_)
		{
			if (random.NextInt(0, 1) == 0)
			{
				if (random.NextInt(0, 1) == 0)
				{
					child = Node::make_random(random);
				}
				child->mutate(random);
			}
		}
		//add a random amount of nodes, such that on average there are as many
		//nodes left as there were nodes to start with.
		for (int i = 0; i < rand(0, size); ++i)
		{
			Node::ptr c = Node::make_random(random);
			c->mutate(random);
			children_.push_back(c);
		}
	
	}
	else
	{
		//if no nodes, just add a random node.
		Node::ptr c = Node::make_random(random);
		c->mutate(random);
		children_.push_back(c);
	}
}

void CodeBlock::replace(Node::ptr& from, Node::ptr& to)
{
	//can't replace self
	if (this != from.get())
	{
		for (Node::ptr& c : children_)
		{
			if (c == from)
			{
				c = to;
				break;
			}
			c->replace(from, to);
		}
	}
}