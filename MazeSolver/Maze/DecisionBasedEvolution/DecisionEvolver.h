#pragma once

#include "CodeTurtle.h"
#include "../Random.h"
#include "../Coord.h"
#include "../Solver.h"
#include <utility>

class DecisionEvolver : public Solver
{
public:
	DecisionEvolver();
	DecisionEvolver(Coord start, Coord end, int max_turtle_steps, int population_size, double combination_probability, double mutation_probability);

	bool Step(Maze& maze) override;
	void Reset() override;
	void Reset(Coord start, Coord end) override;
	void DecisionEvolver::NewMaze(Coord start, Coord end, int max_steps);

private:
	void crossover(CodeTurtle&, CodeTurtle&);
	void mutate(CodeTurtle& n);

	int calculate_fitness(CodeTurtle& g, Maze& maze);
	void calculate_fitness(Maze& maze);
	void sort_by_fitness();

	void evolve(Maze& maze);

	int distance(Coord a, Coord b);

	Randomizer rand_;
	std::vector<CodeTurtle> population_;
	CodeTurtle& select();

	int population_size_;
	float combination_p_, mutation_p_;
	Coord start_, end_;
	int steps_, max_turtle_steps_;
};