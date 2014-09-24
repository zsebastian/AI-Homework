#include "Population.h"

std::vector<Genome>::iterator Population::begin()
{
	return genomes_.begin();
}

std::vector<Genome>::iterator Population::end()
{
	return genomes_.end();
}

size_t Population::size() const
{
	return genomes_.size();
}

const Genome& Population::operator[](size_t i) const
{
	return genomes_[i];
}

Genome& Population::operator[](size_t i)
{
	return genomes_[i];
}

void Population::remove(size_t i)
{
	genomes_.erase(genomes_.begin() + i);
}

void Population::push_back(Genome& genome)
{
	genomes_.push_back(genome);
}

void Population::clear()
{
	genomes_.clear();
}