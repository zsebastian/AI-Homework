#include "StateMachine.h"

void StateMachine::pop_state()
{
	state_stack_.pop_back();
}

void StateMachine::push_state(std::function<void()> state)
{
	state_stack_.push_back(state);
}

void StateMachine::execute_current()
{
	state_stack_.back()();
}
