#pragma once

#include <string>
#include <map>

class EvaluationSettings
{
public:
	double& operator[](const std::string& key);
	void save(const std::string& file);
	void load(const std::string& file);
private:
	void process(const std::string& line);
	std::map<std::string, double> vals_;
};