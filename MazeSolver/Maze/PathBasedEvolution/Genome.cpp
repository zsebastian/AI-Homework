#include "Genome.h"

Genome::Genome(size_t size)
{
	sequence_.reserve(size);
	for (int i = 0; i < size; ++i)
	{
		sequence_.push_back(false);
	}
}

Genome::Iterator::Iterator(size_t index, const std::vector<bool>& sequence)
	:sequence_(sequence)
	,index_(index)
{}

bool Genome::Iterator::operator!=(const Genome::Iterator& rhs) const
{
	return sequence_ != rhs.sequence_ || index_ != rhs.index_;
}

Genome::Iterator& Genome::Iterator::operator++()
{
	index_ += Genome::phenome_length();
	return *this;
}

Phenome::Type Genome::Iterator::operator*() const
{
	return Genome::get_phenome(index_, sequence_);
}

Phenome::Type Genome::get_phenome(size_t index, std::vector<bool> sequence)
{
	if (sequence[index] == false)
	{
		return Phenome::Halt;
	}
	else if (sequence[index + 1] == false)
	{
		if (sequence[index + 2] == false)
		{
			return Phenome::North;
		}
		else
		{
			return Phenome::South;
		}
	}
	else
	{
		if (sequence[index + 2] == false)
		{
			return Phenome::West;
		}
		else
		{
			return Phenome::East;
		}
	}
}
	
size_t Genome::phenome_size() const
{
	return sequence_.size() / 3;
}

Phenome::Type Genome::get_phenome(size_t index) const
{
	return get_phenome(index, sequence_);
}

size_t Genome::gene_size() const
{
	return sequence_.size();
}

bool Genome::get_gene(size_t index) const
{
	return sequence_[index];
}

void Genome::set_gene(size_t index, bool value)
{
	sequence_[index] = value;
}

Genome::Iterator Genome::begin() const
{
	return Genome::Iterator(0, sequence_);
}

Genome::Iterator Genome::end() const
{
	return Genome::Iterator(sequence_.size(), sequence_);
}

void Genome::set_fitness(int fitness)
{
	fitness_ = fitness;
}

int Genome::get_fitness() const
{
	return fitness_;
}