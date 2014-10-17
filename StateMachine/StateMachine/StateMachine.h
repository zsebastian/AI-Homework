#pragma once

#include <vector>
#include <functional>

class StateMachine
{
public:
	void pop_state();
	void push_state(std::function<void ()>);
	void execute_current();
private:
	std::vector<std::function<void()>> state_stack_;
};