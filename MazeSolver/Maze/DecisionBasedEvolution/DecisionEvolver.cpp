#include "DecisionEvolver.h"
#include "../Maze.h"

DecisionEvolver::DecisionEvolver()
	:population_size_(0)
	,combination_p_(0.0f)
	,mutation_p_(0.0f)
	,steps_(0)
	,max_turtle_steps_(0)
{
}

DecisionEvolver::DecisionEvolver(Coord start, Coord end, int max_turtle_steps, int population_size, double combination_probability, double mutation_probability)
	:population_size_(population_size)
	, combination_p_(combination_probability)
	, mutation_p_(mutation_probability)
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
			turtle.get_base()->mutate(rand_, 5);
		}
	}

	/*Formatter format;

	format.append("Population");
	format.push_indent();

	for (int i = 0; i < population_.size(); ++i)
	{
		format.append(std::to_string(i) + "(" + std::to_string(population_[i].get_fitness()) + "):");
		format.push_indent();
		population_[i].get_base()->format(format);
		format.pop_indent();
	}
	format.pop_indent();

	std::cout << format.to_string() << std::endl;*/


	if (steps_ % 2 == 0)
	{
		evolve(maze);

		/*for (auto& bot : population_)
		{
			std::cout << bot.get_fitness() << ", ";
		}
		std::cout << std::endl;*/
	}
	else
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

		for (auto& bot : population_)
		{
			bot.set_start_position(start_);
			while (bot.step(maze) < max_turtle_steps_)
			{
				if (bot.get_current_coord() == end_)
				{
					Formatter format;
					format.append("Good bot! (" + std::to_string(bot.get_fitness()) + ")");
					format.push_indent();

					format.push_indent();
					bot.get_base()->format(format);
					format.pop_indent();
					
					format.pop_indent();
					std::cout << format.to_string() << std::endl;
					return false;
				}
			}

			Coord c = bot.get_current_coord();
			maze(c.x, c.y).color = Color::blue();
		}
		maze(start_.x, start_.y).color = Color::green();
		maze(end_.x, end_.y).color = Color::red();

	}
	steps_++;

	return true;
}

void DecisionEvolver::evolve(Maze& maze)
{
	calculate_fitness(maze);
	sort_by_fitness();

	std::vector<CodeTurtle> child_population;
	while (child_population.size() < population_size_)
	{
		Formatter format;

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
}

void DecisionEvolver::Reset()
{
	steps_ = 0;
	population_.clear();
}

void DecisionEvolver::NewMaze(Coord start, Coord end, int max_steps)
{
	start_ = start;
	end_ = end;
	max_turtle_steps_ = max_steps;
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
	auto rind = [this]() { return rand_.NextInt(0, population_.size() - 1); };

	int best = rind();

	for (int i = 0; i < population_.size() - 1; ++i)
	{
		int curr = rind();
		if (population_[curr].get_fitness() < population_[best].get_fitness())
		{
			best = curr;
		}
	}
	return population_[best];
}

void DecisionEvolver::sort_by_fitness()
{
	std::sort(population_.begin(), population_.end(), [&](const CodeTurtle& g0, const CodeTurtle& g1) {return g0.get_fitness() < g1.get_fitness(); });
}

int DecisionEvolver::calculate_fitness(CodeTurtle& g, Maze& maze)
{
	g.set_start_position(start_);
	for (int i = 0; i < max_turtle_steps_; ++i)
	{
		if (g.get_current_coord() == end_)
		{
			return i;
		}
		g.step(maze);
	}
	while (g.step(maze) <= max_turtle_steps_)
		;

	return max_turtle_steps_ + distance(g.get_current_coord(), end_);
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
		b0 = b1->clone();
	}
	else
	{
		int max_d = b0->max_depth();
		int select_d = max_d - s1->max_depth();
		//Force the code to a certain depth
		if (max_d - select_d >= 5)
		{
			Node::ptr n = Node::make_random(rand_);
			n->mutate(rand_, std::max(0, std::min(2, 5 - select_d)));
			b0->replace(s0, n);
		}
		else
		{
			b0->replace(s0, s1->clone());
		}
	}
	t0.get_base() = b0;

	if (b1 == s1)
	{
		b1 = b0->clone();
	}
	else
	{
		int max_d = b1->max_depth();
		int select_d = max_d - s0->max_depth();
		//Force the code to a certain depth
		if (max_d - select_d > 5)
		{
			Node::ptr n = Node::make_random(rand_);
			n->mutate(rand_, std::max(0, std::min(2, 5 - select_d)));
			b1->replace(s0, n);
		}
		else
		{
			b1->replace(s1, s0->clone());
		}
	}
	t1.get_base() = b1;
}

void DecisionEvolver::mutate(CodeTurtle& t)
{
	Node::ptr s = t.get_base()->select_node(rand_);
	Node::ptr n = s->clone();

	int max_d = t.get_base()->max_depth();
	int select_d = max_d - s->max_depth();

	if (rand_.NextInt(0, 1) == 1 || max_d - select_d >= 5)
	{
		n = Node::make_random(rand_);
	}

	n->mutate(rand_, std::max(0, std::min(2, 5 - select_d)));
	if (s == t.get_base())
	{
		t.get_base() = n;
	}
	else
	{
		t.get_base()->replace(s, n);
	}
}

int DecisionEvolver::distance(Coord a, Coord b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}
