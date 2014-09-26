#include "Evolver.h"
#include "../Maze.h"

Evolver::Evolver()
	:population_size_(0)
	,genome_size_(0)
	,combination_p_(0.0f)
	,mutation_p_(0.0f)
	,steps_(0)
{
}

Evolver::Evolver(Coord start, Coord end, int population_size, int genome_size, double combination_probability, double mutation_probability)
	:population_size_(population_size)
	, genome_size_(genome_size)
	, combination_p_(combination_probability)
	, mutation_p_(mutation_probability)
	, steps_(0)
	, start_(start)
	, end_(end)
{
	
}

bool Evolver::Step(Maze& maze)
{
	if (steps_ == 0)
	{
		for (int i = 0; i < population_size_; ++i)
		{
			population_.push_back(Genome(genome_size_ * Genome::phenome_length()));
			Genome& genome = population_[i];
			for (int j = 0; j < genome_size_ * Genome::phenome_length(); ++j)
			{
				genome.set_gene(j, rand_.NextInt(0, 1) == 1 ? true : false);
			}
		}
	}

	
	calculate_fitness(maze);
	sort_by_fitness();
	
	Population child_population;
	while (child_population.size() < population_size_)
	{
		Genome& genome0 = select();
		Genome& genome1 = select();

		if (rand_.NextBool(combination_p_)){
			combine(genome0, genome1);
		}
		if (rand_.NextBool(mutation_p_))
		{

		}
		mutate(genome0);
		mutate(genome1);
		child_population.push_back(genome0);
		child_population.push_back(genome1);
	}
	population_ = child_population;
	calculate_fitness(maze);
	sort_by_fitness();
	steps_++;
	
	clear_paths(maze);
	render_path(population_[0], maze);
	
	return true;
}

void Evolver::Reset()
{
	steps_ = 0;
	population_.clear();
}

void Evolver::Reset(Coord start, Coord end)
{
	start_ = start;
	end_ = end;
	Reset();
}

Genome& Evolver::select()
{
	//tournament selection
	auto rind = [this]() { return rand_.NextInt(0, population_.size() - 1); };

	int best = rind();
	
	for (int i = 0; i < population_.size() - 1; ++i)
	{
		int curr = rind();
		if (population_[best].get_fitness() > population_[curr].get_fitness())
		{
			best = curr;
		}
	}
	return population_[best];
}

void Evolver::clear_paths(Maze& maze)
{
	for (int i = 0; i < maze.Height(); ++i)
	{
		for (int j = 0; j < maze.Width(); ++j)
		{
			if (maze(j, i).cost != -1)
			{
				maze(j, i).color = Color::white();
			}
		}
	}
}

void Evolver::render_path(Genome& g, Maze& maze)
{
	Coord current = start_;
	for (Phenome::Type dir : g)
	{
		current = transform(current, dir, maze);
		maze(current.x, current.y).color = Color::blue();
	}
	maze(start_.x, start_.y).color = Color::green();
	maze(end_.x, end_.y).color = Color::red();
}

void Evolver::sort_by_fitness()
{
	std::sort(population_.begin(), population_.end(), [&](const Genome& g0, const Genome& g1) {return g0.get_fitness() < g1.get_fitness(); });
}

int Evolver::calculate_fitness(Genome& g, Maze& maze)
{
	Coord current = start_;
	for (Phenome::Type phenome : g)
	{
		current = transform(current, phenome, maze);
	}
	return distance(current, end_);
}

void Evolver::calculate_fitness(Maze& maze)
{
	for (Genome& gene : population_)
	{
		int fitness = calculate_fitness(gene, maze);
		gene.set_fitness(fitness);
	}
}

Coord Evolver::transform(Coord c, Phenome::Type dir, Maze& maze)
{
	Coord to = c;
	switch (dir)
	{
	case Phenome::East:
		to.x++;
		break;
	case Phenome::West:
		to.x--;
		break;
	case Phenome::South:
		to.y++;
		break;
	case Phenome::North:
		to.y--;
		break;
	default:
		break;
	}

	if (maze.IsBorderCoord(to.x, to.y) || maze(to.x, to.y).cost == -1)
	{
		return c;
	}
	else
	{
		return to;
	}
}
Genome Evolver::combine(Genome& g0, Genome& g1)
{
	size_t len = g0.gene_size();
	Genome gene(len);

	for (size_t i = 0; i < len; i++)
	{
		gene.set_gene(i, rand_.NextInt(0, 1) == 1 ? g0.get_gene(i) : g1.get_gene(i));
	}
	return gene;
}

void Evolver::mutate(Genome& g)
{
	for (size_t i = 0; i < g.gene_size(); i++)
	{
		if (rand_.NextBool(mutation_p_))
		{
			g.set_gene(i, rand_.NextInt(0, 1) == 1 ? true : false);
		}
	}
}

int Evolver::distance(Coord a, Coord b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}
