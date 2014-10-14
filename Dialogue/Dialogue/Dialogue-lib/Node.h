#pragma once

#include <pugixml.hpp>
#include <string>
#include <vector>
#include <lua.hpp>
#include <memory>

#include "Choice.h"

/*	Reads:
	http://stackoverflow.com/questions/372915/game-logic-in-xml-files
*/

namespace Dialogue
{
	class Node
	{
	private:
		struct this_is_private{};
	public:
		static std::shared_ptr<Node> make(const pugi::xml_node& xml);

		Node(const pugi::xml_node& xml, this_is_private);
		Node();

		const int id()  const { return id_; }

		const std::string& message() const { return message_; }

		const struct node_choice_container
		{
			typedef std::vector<const Choice>::const_iterator iterator_t;

			node_choice_container(const Node* const n)
				:n_(n) {}

			iterator_t begin() const
			{
				return n_->choices_.cbegin();
			}

			iterator_t end() const
			{
				return n_->choices_.cend();
			}
		private:
			const Node* const n_;
		} choices;

		
	private:
		std::vector<const Choice> choices_;
		int id_;
		const std::string message_;
	};
}