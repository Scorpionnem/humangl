#include "App.hpp"

#include <stdexcept>
#include <iostream>

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage:\n  ./humanGL <*.hgl>" << std::endl;
		return (1);
	}

	try
	{
		App	app;

		app.run(av[1]);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
