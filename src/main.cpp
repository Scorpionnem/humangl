/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/04 14:16:38 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

int	main()
{
	try {
		Engine::init();

		Engine::log("Starting rendering...", NORMAL);
		while (Engine::Window->up())
		{
			Engine::Window->loopStart();

			Engine::Scenes->update();
			Engine::Scenes->render();

			Engine::Window->loopEnd();
		}
	} catch (const std::exception &e) {
		Engine::log("Program terminated: " + std::string(e.what()), LogSeverity::MELTDOWN);
		Engine::destroy();
		return (1);
	}
	Engine::destroy();
}
