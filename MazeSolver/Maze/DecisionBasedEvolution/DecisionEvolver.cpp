#include "DecisionEvolver.h"
#include "../Maze.h"

DecisionEvolver::DecisionEvolver()
	:population_size_(0)
	,genome_size_(0)
	,combination_p_(0.0f)
	,mutation_p_(0.0f)
	,selection_p_(0.0f)
	,steps_(0)
	,max_turtle_steps_(0)
{
}

DecisionEvolver::DecisionEvolver(Coord start, Coord end, int max_turtle_steps, int population_size, double combination_probability, double mutation_probability, double selection_probability)
	:population_size_(population_size)
	, combination_p_(combination_probability)
	, mutation_p_(mutation_probability)
	, selection_p_(selection_probability)
	, steps_(0)
	, start_(start)
	, end_(end)
	, max_turtle_steps_(max_turtle_steps)
{
	
}

bool DecisionEvolver::Step(Maze& maze)
{
	if (steps_ == 0)
	{
		for (int i = 0; i < population_size_; ++i)
		{
			population_.push_back(CodeTurtle());

			CodeTurtle& turtle = population_[i];
			turtle.get_base() = Node::make_random(rand_);
			turtle.get_base()->mutate(rand_);
		}
	}

	steps_++;
	
	calculate_fitness(maze);
	sort_by_fitness();
	
	std::vector<CodeTurtle> child_population;
	while (child_population.size() < population_size_)
	{
		CodeTurtle& c0 = select();
		CodeTurtle& c1 = select();

		if (rand_.NextBool(combination_p_))
		{
			crossover(c0, c1);
		}
		if (rand_.NextBool(mutation_p_))
		{
			mutate(c0);
		}
		if (rand_.NextBool(mutation_p_))
		{
			mutate(c1);
		}
		
		child_population.push_back(c0);
		child_population.push_back(c1);
	}
	population_ = child_population;
	calculate_fitness(maze);
	sort_by_fitness();

	return true;
}

void DecisionEvolver::Reset()
{
	steps_ = 0;
	population_.clear();
}

void DecisionEvolver::Reset(Coord start, Coord end)
{
	start_ = start;
	end_ = end;
	Reset();
}

CodeTurtle& DecisionEvolver::select()
{
	//tournament selection
	auto p = [this](int i){return std::pow(selection_p_*(1 - selection_p_), static_cast<double>(i)); };
	for (int i = 0; i < population_.size() - 1; ++i)
	{
		if (rand_.NextBool(p(i)))
		{
			return population_[i];
		}
	}
	return population_[population_.size() - 1];
}

void DecisionEvolver::sort_by_fitness()
{
	std::sort(population_.begin(), population_.end(), [&](const CodeTurtle& g0, const CodeTurtle& g1) {return g0.get_fitness() < g1.get_fitness(); });
}

int DecisionEvolver::calculate_fitness(CodeTurtle& g, Maze& maze)
{
	g.set_start_position(start_);
	while (g.step(maze) <= max_turtle_steps_)
		;

	return distance(g.get_current_coord(), end_);
}

void DecisionEvolver::calculate_fitness(Maze& maze)
{
	for (CodeTurtle& gene : population_)
	{
		int fitness = calculate_fitness(gene, maze);
		gene.set_fitness(fitness);
	}
}

void DecisionEvolver::crossover(CodeTurtle& t0, CodeTurtle& t1)
{
	Node::ptr b0 = t0.get_base();
	Node::ptr b1 = t1.get_base();
	Node::ptr s0 = b0->select_node(rand_);
	Node::ptr s1 = b1->select_node(rand_);
	if (b0 == s0)
	{
		b0 = b1;
	}
	else
	{
		b0->replace(s0, s1);
	}
	t0.get_base() = b0;

	if (b1 == s1)
	{
		b1 = b0;
	}
	else
	{
		b1->replace(s1, s0);
	}
	t1.get_base() = b1;
}

void DecisionEvolver::mutate(CodeTurtle& t)
{
	Node::ptr n = t.get_base()->select_node(rand_);

	if (rand_.NextInt(0, 1) == 1)
	{
		Node::ptr rand = Node::make_random(rand_);
		if (n == t.get_base())
		{
			t.get_base() = rand;
		}
		else
		{
			t.get_base()->replace(n, rand);
		}
		n = rand;
	}

	n->mutate(rand_);
}

int DecisionEvolver::distance(Coord a, Coord b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}
