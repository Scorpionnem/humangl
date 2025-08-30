/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 09:51:17 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 10:56:44 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Engine.hpp"

extern Skybox	*SKYBOX;
extern uint		seed;
extern Camera	*CAMERA;

std::string	command_quit(std::istringstream &)
{
	Engine::Window->close();
	return ("Closing.");
}

Commands::Commands()
{
	_commands["/quit"] = command_quit;
	_commands["/q"] = command_quit;
}
