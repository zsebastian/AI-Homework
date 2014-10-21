#include "EvaluationSettings.h"
#include <fstream>
#include <regex>

double& EvaluationSettings::operator[](const std::string& key)
{
	auto iter = vals_.find(key);
	if (iter == vals_.end())
	{
		vals_.insert(std::make_pair(key, double()));
		return vals_[key];
	}
	return iter->second;
}

void EvaluationSettings::load(const std::string& file)
{
	std::ifstream infile(file);
	std::string line;
	while (std::getline(infile, line))
	{
		process(line);
	}
}

void EvaluationSettings::process(const std::string& line)
{
	std::regex r(R"([A-Za-z0-9_]+):([0-9\.]*)");
	std::smatch sm;
	std::regex_match(line, sm, r);
	
	vals_.insert(std::make_pair(sm[1], std::stod(sm[2])));
}

void EvaluationSettings::save(const std::string& file)
{
	std::ofstream outfile(file);
	for (auto p : vals_)
	{
		outfile << p.first << ":" << p.second << std::endl;
	}
}
