#include "SolverEvolver.h"
#include "EvaluationFunctions.h"
#include "picojson.h"
#include <fstream>

SolverEvolver::SolverEvolver(int population_size, double combination_probability, double mutation_probability)
	:population_size_(population_size)
	, combination_p_(combination_probability)
	, mutation_p_(mutation_probability)
	, steps_(0)
{
	std::ifstream file("population.json");
	picojson::value v;
	std::string err;
	err = picojson::parse(v, file);
	if (err.empty()) 
	{
		auto arr = v.get("population").get<picojson::array>();
		for (int i = 0; i < population_size && i < arr.size(); ++i)
		{
			auto genome = arr[i].get<picojson::array>();
			population_.push_back(Individual());
			if (genome.size() == population_[i].genome.size())
			{
				for (int j = 0; j < population_[i].genome.size(); ++j)
				{
					population_[i].genome[j] = genome[j].get<double>();
				}
			}
			else
			{
				population_[i].genome = random_genome();
			}
		}
	}

	for (int i = population_.size(); i < population_size; ++i)
	{
		population_.push_back(Individual());
		population_[i].genome = random_genome();
	}

	initate_population();	
}

SolverEvolver::Genome SolverEvolver::random_genome()
{
	Genome ret;
	for (int i = 0; i < ret.size(); ++i)
	{
		ret[i] = random_double(0.0f);
	}
	return std::move(ret);
}

double SolverEvolver::random_double(double middle)
{
	return rand_.NextDouble(middle - 10.f, middle + 10.f);
}

void SolverEvolver::update(Window& window)
{
	bool no_update = true;
	for (auto& p : population_)
	{
		if (!p.solver->is_done())
		{
			no_update = false;
			p.solver->update(*p.board);
			p.board->render(window);
		}
	}
	if (steps_ % 50 == 0)
		window.PrintScreen("screen_" + std::to_string(steps_) + ".bmp");
	++steps_;
	if (no_update)
	{
		evolve();
	}
}

void SolverEvolver::evolve()
{
	sort_by_fitness();

	std::vector<Individual> child_population;
	while (child_population.size() < population_size_)
	{
		Individual& c0 = select();
		Individual& c1 = select();

		if (rand_.NextBool(combination_p_))
		{
			crossover(c0.genome, c1.genome);
		}
		mutate(c0.genome);
		mutate(c1.genome);

		child_population.push_back(c0);
		child_population.push_back(c1);
	}
	population_ = child_population;
	initate_population();
}

SolverEvolver::Individual& SolverEvolver::select()
{
	//tournament selection
	auto rind = [this]() { return rand_.NextInt(0, population_.size() - 1); };

	int best = rind();

	for (int i = 0; i < population_.size() - 1; ++i)
	{
		int curr = rind();
		if (population_[curr].solver->get_score() > population_[best].solver->get_score())
		{
			best = curr;
		}
	}
	return population_[best];
}

void SolverEvolver::sort_by_fitness()
{
	std::sort(population_.begin(), population_.end(), [&](const Individual& g0, const Individual& g1) {return g0.solver->get_score() > g1.solver->get_score(); });
}

void SolverEvolver::crossover(SolverEvolver::Genome& t0, SolverEvolver::Genome& t1)
{
	Genome g0 = t0;
	Genome g1 = t1;
	size_t len = g0.size();

	for (size_t i = 0; i < len; i++)
	{
		t0[i] = rand_.NextInt(0, 1) == 1 ? g0[i] : g1[i];
		t1[i] = rand_.NextInt(0, 1) == 1 ? g0[i] : g1[i];
	}
	t0 = g0;
	t1 = g1;
}

void SolverEvolver::mutate(SolverEvolver::Genome& t)
{
	for (size_t i = 0; i < t.size(); i++)
	{
		if (rand_.NextBool(mutation_p_))
		{
			t[i] = random_double(t[i]);
		}
	}
}

void SolverEvolver::initate_population()
{
	int x = 0;
	int y = 0;
	for (auto& c : population_)
	{
		std::vector<std::pair<Solver::evaluation_function, double>> evaluations;
		evaluations.emplace_back(get_evaluation<0>(c.genome));
		evaluations.emplace_back(get_evaluation<1>(c.genome));
		evaluations.emplace_back(get_evaluation<2>(c.genome));
		evaluations.emplace_back(get_evaluation<3>(c.genome));
		evaluations.emplace_back(get_evaluation<4>(c.genome));
		evaluations.emplace_back(get_evaluation<5>(c.genome));
		evaluations.emplace_back(get_evaluation<6>(c.genome));
		c.solver = std::move(std::make_unique<Solver>(evaluations));
		c.board = std::move(std::make_unique<Board>(x, y, 16));
		x += 16 * 12;
		if (x > 800)
		{
			y += 16 * 22;
			x = 0;
		}
	}

	picojson::object obj;
	picojson::array arr;
	for (auto& c : population_)
	{
		picojson::array genome;
		for (int i = 0; i < c.genome.size(); ++i)
		{
			genome.push_back((picojson::value)c.genome[i]);
		}
		arr.push_back((picojson::value)genome);
	}
	obj.insert(std::make_pair("population", (picojson::value)arr));
	
	picojson::value val = (picojson::value)obj;
	std::string str = val.serialize();
	std::ofstream file("population.json");
	file << str;
}