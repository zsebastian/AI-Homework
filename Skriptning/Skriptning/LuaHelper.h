#pragma once

#include <luacppinterface.h>
#include <string>
#include "Robot.h"
#include <map>
#include <vector>
#include <random>

class LuaHelper
{
public:
	LuaHelper(int level_w, int level_h);
	void init_standard_functions();
	void init_standard_values();

	void run_script(std::string file);
	
	int get_robot_count() const;
	Robot get_robot_at(int index) const;
	Robot get_robot(const std::string& name) const;
	

	template <typename func_t, typename... Ts>
	auto call(std::string func_name, Ts... ts) -> typename LuaGenericFunction<typename func_t>::rettype
	{
		auto lua_func = lua_.GetGlobalEnvironment().Get<LuaFunction<func_t>>(func_name);
		auto result = lua_func.Invoke(ts ...);
		return result;
	}

	Lua get_lua() const { return lua_; }

	void update_robots();

private:
	LuaTable get_table_at(int index) const;
	LuaTable get_table(const std::string& name) const;
	LuaTable register_robot(std::string name, LuaFunction<void(LuaTable)> update_function);
	int manhattan_distance(LuaTable robot0, LuaTable robot1) const;
	int random(int min, int max);
	void print(std::string) const;
	
	Lua lua_;
	std::vector<std::pair<LuaTable, LuaFunction<void(LuaTable)>>> robots_;
	int level_w_, level_h_;

	std::mt19937 engine_;
};