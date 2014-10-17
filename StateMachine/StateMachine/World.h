#pragma once

#include <functional>
#include <vector>
#include <map>

#include "Coord.h"
#include "Window.h"
#include "Rect.h"

class World
{
public:
	World(float w, float h);
	int add_object(std::string tag, std::function<void(float)> update, std::function<const Rect&()> rect);
	void update(float dt);
	void remove_object(int id);
	void render(Window& window);

	std::vector<std::pair<int, std::function<const Rect&()>>> find_with_tag(const std::string& tag) const;

	float width() const
	{
		return w_;
	}
	
	float height() const
	{
		return h_;
	}
private:
	struct Object
	{
		std::string tag;
		std::function<void(float)> update;
		std::function<const Rect&()> rect;
	};

	std::map<int, Object> objects_;

	std::vector<int> to_remove_;
	int next_id_;

	void cull_dead();
	float w_, h_;
};
