#pragma once

#include <sstream>

class Formatter
{
public:
	Formatter();
	void append(std::string line);
	void push_indent();
	void pop_indent();

	std::string to_string();
private:
	std::stringstream ss_;
	int indent_;
};