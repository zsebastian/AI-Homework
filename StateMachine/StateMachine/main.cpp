#include "Window.h"
#include "Timer.h"
#include "Mouse.h"
#include "World.h"

int main(int argc, char **argv)
{
	World world(800, 800);
	Mouse mouse(world, 50.f, 50.f);

	std::random_device rd;
	std::mt19937 random_engine(rd());
	std::uniform_real_distribution<float> dist(0, 800.f);
	for (int i = 0; i < 50; ++i)
	{
		float x = dist(random_engine);
		float y = dist(random_engine);
		Rect cheese(x, y, 10.f, Color::green());
		world.add_object("cheese", [](float dt){}, [cheese]() {return cheese; });
	}

	Window window("mouse and cat and cheese", 800, 800);
	
	Timer timer;
	timer.Start();
	int last_update = timer.ElapsedMilliseconds();
	
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

		window.Display();
		window.Clear();
	}

	return 0;
}