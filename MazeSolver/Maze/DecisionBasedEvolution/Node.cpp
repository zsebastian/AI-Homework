#include "Node.h"
#include "../Random.h"
#include "CodeBlock.h"
#include "ControlBlock.h"
#include "Command.h"

Node::ptr Node::get_this()
{
	return shared_from_this();
}

Node::ptr Node::make_random(Randomizer& random)
{
	int r = random.NextInt(0, 2);
	switch (r)
	{
	case 0:
		return CodeBlock::make();
	case 1:
		return ControlBlock::make();
	case 2:
		return Command::make(Command::BLANK);
	default:
		break;
	}
}