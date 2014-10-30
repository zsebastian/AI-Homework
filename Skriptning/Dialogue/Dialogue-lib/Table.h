#pragma once

#include <unordered_map>
#include <memory>
#include "Node.h"

namespace Dialogue
{
	class Table
	{
	public:
		std::shared_ptr<Node> operator[](int id) const;
		bool exists(int id) const;
		void add(int id, std::shared_ptr<Node>);
	private:
		std::unordered_map<int, std::shared_ptr<Node>> table_;
	};
}