#pragma once

#include <vector>

struct Phenome
{
	enum Type { Halt, East, West, South, North };
};

class Genome
{
public:
	Genome() {};
	Genome(size_t genome_size);

	class Iterator
	{
	public:
		Iterator(size_t index, const std::vector<bool>& sequence);

		bool operator!=(const Iterator& rhs) const;
		Iterator& operator++();
		Phenome::Type operator*() const;
	private:
		size_t index_;
		const std::vector<bool>& sequence_;
	};

	size_t phenome_size() const;
	Phenome::Type get_phenome(size_t index) const;
	
	size_t gene_size() const;
	bool get_gene(size_t index) const;
	void set_gene(size_t index, bool value);
	static size_t phenome_length() { return 3; }

	Iterator begin() const;
	Iterator end() const;

	/* The Genome cannot calculate fitness */
	void set_fitness(int fitness);

	/* The Genome cannot calculate fitness */
	int get_fitness() const;
private:
	static Phenome::Type get_phenome(size_t index, std::vector<bool> sequence);
	std::vector<bool> sequence_;
	int fitness_;
};