#pragma once

/*	Reads:
http://stackoverflow.com/questions/372915/game-logic-in-xml-files
*/

struct Choice
{
	Choice(std::string id_, std::vector<const std::string> commands_, std::string condition_)
		: commands(std::move(commands_))
		, condition(std::move(condition_))
		, id(std::move(id_))
	{}

	struct commands_container
	{
		typedef std::vector<const std::string>::const_iterator iterator_t;

		commands_container(std::vector<const std::string> commands)
			: commands_(std::move(commands))
		{}

		iterator_t begin() const
		{
			return commands_.cbegin();
		}

		iterator_t end() const
		{
			return commands_.cend();
		}
	private:
		std::vector<const std::string> commands_;
	};
	
	/*	All commands, in order, that will be run in the lua interpreter when this choice
		is chose */
	const commands_container commands;

	/*	a lua-condition that must evaluate to true for this choice to be visible */
	const std::string condition;

	/*	what the player says */
	const std::string id;
};