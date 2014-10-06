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
	//select a child at random                                                                                                     
	int index = random.NextInt(0, children_.size());

	//Only select self if there are no children or once in every lenght of child.
	if (index == 0 || children_.size() == 0)
	{
		return get_this();
	}

	Node::ptr s = children_[index - 1];
	//select that child half the time, select a childs child half the time.
	if (random.NextInt(0, 2) == 0)
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

void CodeBlock::mutate(Randomizer& random, int depth)
{
	int size = children_.size();
	if (size != 0)
	{
		int size_change = random.NextInt(-2 - (2 - children_.size()), 2 - (2 - children_.size()));

		auto rand = [this, &random, size](int i){return random.NextInt(0, i - 1); };
		
		//shuffle nodes
		try
		{
			std::random_shuffle(children_.begin(), children_.end(), rand);
		}
		catch (...)
		{
			throw;
		}
		int rem = std::min(static_cast<int>(children_.size()), std::max(0, rand(size) + size_change));
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
				
				if (depth > 0)
				{
					child->mutate(random, depth - 1);
				}
			}
		}
		//add a random amount of nodes, such that on average there are as many
		//nodes left as there were nodes to start with.
		for (int i = 0; i < rand(size) + size_change; ++i)
		{
			Node::ptr c = Node::make_random(random);
			if (depth > 0)
			{
				c->mutate(random, depth - 1);
			}
			children_.push_back(c);
		}
	
	}
	else
	{
		int size = random.NextInt(0, 2);
		for (int i = 0; i < size; ++i)
		{
			//if no nodes, just add a random node.
			Node::ptr c = Node::make_random(random);
			if (depth > 0)
			{
				c->mutate(random, depth - 1);
			}
			children_.push_back(c);
		}
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

void CodeBlock::format(Formatter& format)
{
	for (Node::ptr& n : children_)
	{
		n->format(format);
	}
}

int CodeBlock::max_depth()
{
	int max = 0;
	for (Node::ptr& n : children_)
	{
		max = std::max(n->max_depth(), max);
	}
	return max + 1;
}
