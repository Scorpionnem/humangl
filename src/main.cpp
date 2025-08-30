/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 13:47:08 by mbatty           ###   ########.fr       */
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
