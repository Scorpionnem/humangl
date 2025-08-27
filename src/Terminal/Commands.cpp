/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 09:51:17 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/27 18:02:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"

extern Skybox	*SKYBOX;
extern uint		seed;
extern Camera	*CAMERA;

void	closeWindow();

std::string	command_quit(std::istringstream &)
{
	closeWindow();
	return ("quitting game");
}

Commands::Commands()
{
	_commands["/quit"] = command_quit;
	_commands["/q"] = command_quit;
}
