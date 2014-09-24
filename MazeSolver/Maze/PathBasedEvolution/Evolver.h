#pragma once

#include "Population.h"
#include "../Random.h"
#include "../Coord.h"
#include "../Solver.h"

class Maze;

class Evolver : public Solver
{
public:
	Evolver();
	Evolver(Coord start, Coord end, int population_size, int genome_size, double combination_probability, double mutation_probability, double selection_probability);

	bool Step(Maze& maze) override;
	void Reset() override;
	void Reset(Coord start, Coord end) override;

private:
	Genome combine(Genome& g0, Genome& g1);
	void mutate(Genome& g);

	int calculate_fitness(Genome& g, Maze& maze);
	void calculate_fitness(Maze& maze);
	void sort_by_fitness();
	void clear_paths(Maze& maze);
	void render_path(Genome& g, Maze& maze);

	Genome& select();

	int distance(Coord a, Coord b);
	Coord transform(Coord c, Phenome::Type dir, Maze& maze);

	Randomizer rand_;
	Population population_;

	int population_size_, genome_size_;
	float combination_p_, mutation_p_, selection_p_;
	Coord start_, end_;
	int steps_;
};