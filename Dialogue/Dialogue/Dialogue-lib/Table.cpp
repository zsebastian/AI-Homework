#include "Table.h"
#include <exception>

/*	Reads:
http://stackoverflow.com/questions/372915/game-logic-in-xml-files
*/

namespace Dialogue
{
	std::shared_ptr<Node> Table::operator[](int id) const
	{
		auto iter = table_.find(id);
		if (iter != table_.end())
		{
			return iter->second;
		}
		else
		{
			return nullptr;
		}
	}

	bool Table::exists(int id) const
	{
		return table_.count(id) != 0;
	}

	void Table::add(int id, std::shared_ptr<Node> node)
	{
		if (id)
		{
			throw std::invalid_argument("id already in table");
		}
		else if (node == nullptr)
		{
			throw std::invalid_argument("node cannot be null");
		}
		table_.insert(std::make_pair(id, std::move(node)));
	}
}