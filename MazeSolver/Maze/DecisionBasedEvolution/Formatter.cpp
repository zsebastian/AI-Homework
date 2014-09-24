#include "Formatter.h"
#include <exception>

Formatter::Formatter()
	:indent_(0)
{}

void Formatter::append(std::string line)
{
	for (int i = 0; i < indent_; ++i)
	{
		ss_ << "    ";
	}
	ss_ << line << "\n";
}

void Formatter::push_indent()
{
	indent_++;
}

void Formatter::pop_indent()
{
	if (indent_ == 0)
	{
		throw std::out_of_range("Too many pops");
	}
	indent_--;
}

std::string Formatter::to_string()
{
	return ss_.str();
}
