#include "Window.h"
#include "Timer.h"
#include "Mouse.h"
#include "World.h"
#include "Cat.h"

int main(int argc, char **argv)
{
	std::vector<Cat> cats;
	//can't move cats around in memory, due to world essentially saving their pointers
	//given the lambdas.
	cats.reserve(5);

	std::random_device rd;
	std::mt19937 random_engine(rd());
	std::uniform_real_distribution<float> dist(0, 800.f);

	World world(800, 800);
	Mouse mouse(world, 50.f, 50.f);

	for (int i = 0; i < 5; ++i)
	{
		cats.emplace_back(world, dist(random_engine), dist(random_engine));
	}

	for (int i = 0; i < 5; ++i)
	{
		float x = dist(random_engine);
		float y = dist(random_engine);
		Rect cheese(x, y, 10.f, Color::make_from_floats(1.f, 1.f, 0.f));
		world.add_object("cheese", [](float dt){}, [cheese]() {return cheese; });
	}

	Window window("mouse and cat and cheese", 800, 800);
	
	Timer timer;
	timer.Start();
	int last_update = timer.ElapsedMilliseconds();
	int last_screen_cap = timer.ElapsedMilliseconds() - 1000;
	int step = 0;
	while (window.Open())
	{
		window.PollEvents();

		if (timer.ElapsedMilliseconds() != last_update)
		{
			long elapsed = timer.ElapsedMilliseconds();
			float delta = (elapsed - last_update) / 1000.f;
			last_update = elapsed;
			world.update(delta);
		}

		world.render(window);
	
		if (timer.ElapsedMilliseconds() >= last_screen_cap + 1000)
		{
			window.PrintScreen("screen_" + std::to_string(++step) + ".bmp");
			last_screen_cap = timer.ElapsedMilliseconds();
		}

		window.Display();
		window.Clear();
	}

	return 0;
}