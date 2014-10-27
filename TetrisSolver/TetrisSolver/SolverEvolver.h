#pragma once

#include "Random.h"
#include "Solver.h"
#include "Board.h"
#include <utility>

class SolverEvolver
{
public:
	SolverEvolver(int population_size, double combination_probability, double mutation_probability);

	void update(Window& window);
private:
	typedef std::array<double, 7U> Genome;
	struct Individual
	{
		Genome genome;
		std::shared_ptr<Solver> solver;
		std::shared_ptr<Board> board;
	};
	
	void initate_population();
	void crossover(Genome&, Genome&);
	void mutate(Genome& n);
	void evolve();

	void sort_by_fitness();

	template<int i>
	std::pair<Solver::evaluation_function, double> get_evaluation(const Genome& g)
	{
		return std::make_pair(EvaluationFunction<i>(), g[i]);
	}

	Randomizer rand_;
	std::vector<Individual> population_;
	Individual& select();
	Genome random_genome();
	double random_double(double middle);

	int population_size_;
	float combination_p_, mutation_p_;
	int steps_;
};