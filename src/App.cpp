#include "App.hpp"
#include "Shader.hpp"
#include "Chunk.hpp"

#include <stdexcept>
#include <iostream>

void	App::_loop()
{
	while (_window.is_open())
	{
		const Window::Events	&events = _window.pollEvents();
		if (events.getKey(SDLK_ESCAPE))
		{
			_window.close();
			break ;
		}
	}
}

void	App::_init()
{
	_window.open("I Love Voxels", 512, 384);
}

void	App::run(void)
{
	_init();

	_loop();
}
