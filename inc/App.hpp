#pragma once

#include "Window.hpp"
#include <string>

class	App
{
	public:
		App() {}
		~App() {}

		void	run(const std::string &path);
	private:
		Window	_window;
	private:
		void	_loop(const std::string &path);
		void	_init();
};
