#pragma once

#include "../Maze.h"
#include "Genome.h"

#include <vector>

class Population
{
public:
	std::vector<Genome>::iterator begin();
	std::vector<Genome>::iterator end();

	size_t size() const;
	const Genome& operator[](size_t i) const;
	Genome& operator[](size_t i);

	/*preserves order*/
	void remove(size_t i);
	void push_back(Genome& genome);
	void clear();

private:
	std::vector<Genome> genomes_;
};