#include "World.h"

World::World(float w, float h)
	:w_(w), h_(h) {}

int World::add_object(std::string tag, std::function<void(float)> update, std::function<const Rect&()> rect)
{
	objects_.emplace(next_id_, Object{ tag, update, rect });
	return next_id_++;
}

void World::update(float dt)
{
	cull_dead();
	for (auto& obj : objects_)
	{
		obj.second.update(dt);
	}
}

std::vector<std::pair<int, std::function<const Rect&()>>> World::find_with_tag(const std::string& tag) const
{
	std::vector<std::pair<int, std::function<const Rect&()>>> ret;
	for (auto& obj : objects_)
	{
		if (obj.second.tag == tag)
		{
			ret.emplace_back(obj.first, obj.second.rect);
		}
	}
	return std::move(ret);
}

void World::remove_object(int id)
{
	to_remove_.push_back(id);
}

void World::cull_dead()
{
	for (int i : to_remove_)
	{
		objects_.erase(i);
	}
	to_remove_.clear();
}

void World::render(Window& window)
{
	for (auto& obj : objects_)
	{
		const Rect& r = obj.second.rect();
		window.RenderRectangle(r.center_x() - r.extends(), r.center_y() - r.extends(), r.extends() * 2, r.extends() * 2, r.color());
	}
}