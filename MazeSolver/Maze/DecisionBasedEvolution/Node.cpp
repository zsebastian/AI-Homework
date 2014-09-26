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
	int r = random.NextInt(0, 5);
	switch (r)
	{
	case 0:
		return ControlBlock::make();
	case 1:
		return CodeBlock::make();
	case 2:
		return CodeBlock::make();
	case 3:
		return Command::make(Command::BLANK);
	case 4:
		return Command::make(Command::BLANK);
	case 5:
		return Command::make(Command::BLANK);
	default:

		//supress warning
		return nullptr;
	}
}