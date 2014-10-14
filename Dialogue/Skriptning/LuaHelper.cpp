#include "LuaHelper.h"
#include <fstream>
#include <iostream>
#include <algorithm>

LuaHelper::LuaHelper(int level_w, int level_h)
	:level_w_(level_w)
	,level_h_(level_h)
{
	init_standard_functions();
	init_standard_values();
	std::random_device random;
	engine_ = std::mt19937(random());
}

void LuaHelper::run_script(std::string file)
{
	std::ifstream t(file);
	std::string what = lua_.RunScript({ std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>() });
	std::cout << "Loading: " << file << std::endl;
	std::cout << what << std::endl;
	std::cout << std::endl;
}

LuaTable LuaHelper::register_robot(std::string name, LuaFunction<void(LuaTable)> update_function)
{
	LuaTable table = lua_.CreateTable();
	table.Set("name", name);
	table.Set("x", random(0, level_w_ - 1));
	table.Set("y", random(0, level_h_ - 1));
	table.Set("r", 0.0f);
	table.Set("g", 0.0f);
	table.Set("b", 0.0f);
	table.Set("health", 100);

	robots_.push_back(std::make_pair(table, update_function));
	lua_.GetGlobalEnvironment().Set("robot_count", robots_.size());
	auto robots = lua_.GetGlobalEnvironment().Get<LuaTable>("robots");
	robots.Set(robots_.size() - 1, table);
	//robots.Set(name, table);
	return table;
}

void LuaHelper::init_standard_functions()
{
	lua_.GetGlobalEnvironment().Set("run_script", lua_.CreateFunction<void(std::string)>([this](std::string file){ run_script(file); }));
	lua_.GetGlobalEnvironment().Set("register_robot", lua_.CreateFunction<LuaTable(std::string, LuaFunction<void(LuaTable)>)>([this](std::string file, LuaFunction<void(LuaTable)> update_func){ return register_robot(file, update_func); }));
	lua_.GetGlobalEnvironment().Set("manhattan_distance", lua_.CreateFunction<int(LuaTable, LuaTable)>([this](LuaTable r0, LuaTable r1){ return manhattan_distance(r0, r1); }));
	lua_.GetGlobalEnvironment().Set("random", lua_.CreateFunction<int(int, int)>([this](int min, int max){ return random(min, max); }));
	lua_.GetGlobalEnvironment().Set("print", lua_.CreateFunction<void(std::string)>([this](std::string str){ print(std::move(str)); }));
}

void LuaHelper::print(std::string str) const
{
	std::cout << std::move(str) << std::endl;
}

void LuaHelper::init_standard_values()
{
	lua_.GetGlobalEnvironment().Set("robot_count", 0);
	lua_.GetGlobalEnvironment().Set("robots", lua_.CreateTable());
	
	LuaTable map = lua_.CreateTable();
	lua_.GetGlobalEnvironment().Set("level", map);
	map.Set("width", level_w_);
	map.Set("height", level_w_);

}

void LuaHelper::update_robots()
{
	for (auto p : robots_)
	{
		p.second.Invoke(p.first);
	}

	for (int i = 0; i < robots_.size(); ++i)
	{
		for (int j = 0; j < robots_.size(); ++j)
		{
			if (j != i)
			{
				int x0, y0, x1, y1;
				x0 = robots_[i].first.Get<int>("x");
				y0 = robots_[i].first.Get<int>("y");
				x1 = robots_[j].first.Get<int>("x");
				y1 = robots_[j].first.Get<int>("y");
				if (x0 == x1 && y0 == y1)
				{
					robots_[i].first.Set("health", robots_[i].first.Get<int>("health") - 10);
					robots_[j].first.Set("health", robots_[j].first.Get<int>("health") - 10);
				}
			}
		}
	}

	auto robots = lua_.GetGlobalEnvironment().Get<LuaTable>("robots");

	robots_.erase(std::remove_if(std::begin(robots_), std::end(robots_), [](const std::pair < LuaTable, LuaFunction<void(LuaTable)>>& p) { return p.first.Get<int>("health") <= 0; }), std::end(robots_));

	for (int i = 0; i < robots_.size(); ++i)
	{
		robots.Set(i, robots_[i].first);
	}
}

int LuaHelper::get_robot_count() const
{
	return robots_.size();
}

Robot LuaHelper::get_robot_at(int index) const
{
	Robot ret;
	LuaTable table = robots_[index].first;
	ret.x = table.Get<int>("x");
	ret.y = table.Get<int>("y");
	ret.r = table.Get<float>("r");
	ret.g = table.Get<float>("g");
	ret.b = table.Get<float>("b");
	ret.health = table.Get<int>("health");
	ret.name = table.Get<std::string>("name");
	return ret;
}

LuaTable LuaHelper::get_table_at(int index) const
{
	return robots_[index].first;
}

Robot LuaHelper::get_robot(const std::string& name) const
{
	for (int i = 0; i < robots_.size(); ++i)
	{
		if (robots_[i].first.Get<std::string>("name") == name)
		{
			return get_robot_at(i);
		}
	}
	throw std::out_of_range("No such robot");
}

LuaTable LuaHelper::get_table(const std::string& name) const
{
	for (int i = 0; i < robots_.size(); ++i)
	{
		if (robots_[i].first.Get<std::string>("name") == name)
		{
			return robots_[i].first;
		}
	}
	throw std::out_of_range("No such robot");
}

int LuaHelper::manhattan_distance(LuaTable robot0, LuaTable robot1) const
{
	int x0, y0, x1, y1;
	x0 = robot0.Get<int>("x");
	y0 = robot0.Get<int>("y");
	x1 = robot1.Get<int>("x");
	y1 = robot1.Get<int>("y");
	int ret = std::abs(x0 - x1) + std::abs(y0 - y1);
	return ret;
}

int LuaHelper::random(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(engine_);
}