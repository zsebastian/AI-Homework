#include "Node.h"

namespace Dialogue
{
	std::shared_ptr<Node> Node::make(const pugi::xml_node& xml)
	{
		return std::make_shared<Node>(xml, this_is_private());
	}

	Node::Node(const pugi::xml_node& xml, Node::this_is_private)
		:choices(this)
	{
		
	}
}