/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:52:22 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 22:13:05 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

Window			*Engine::Window;
ShaderManager	*Engine::Shaders;
TextureManager	*Engine::Textures;
Font			*Engine::Font;
SceneManager	*Engine::Scenes;
FrameBuffer		*Engine::FrameBuffer;

void	Engine::init()
{
	static bool	initialized = false;
	
	if (initialized)
	{
		Engine::log("Engine already initialized!", LogSeverity::ERROR);
		return ;
	}

	Engine::log("Starting...", NORMAL);
	Engine::Window = new class Window();
	Engine::Shaders = new ShaderManager();
	Engine::Shaders->setup();
	Engine::Textures = new TextureManager();
	Engine::Font = new class Font();
	Engine::Scenes = new SceneManager();
	Engine::FrameBuffer = new class FrameBuffer();
}

void	Engine::destroy()
{
	delete FrameBuffer;
	delete Scenes;
	delete Font;
	delete Textures;
	delete Shaders;
	delete Window;
}

void	Engine::log(const std::string &str, LogSeverity severity)
{
	switch (severity)
	{
		case NORMAL:
			std::cout << "[" << glfwGetTime() << "] " << str << std::endl;
			break ;
		case SUCCESS:
			std::cout << GREEN << "[" << glfwGetTime() << "] " << str << RESET << std::endl;
			break ;
		case WARNING:
			std::cerr << YELLOW << "[" << glfwGetTime() << "] " << str << RESET << std::endl;
			break ;
		case ERROR:
			std::cerr << RED << "[" << glfwGetTime() << "] " << str << RESET << std::endl;
			break ;
		case MELTDOWN:
			std::cerr << RED_FULL << "[" << glfwGetTime() << "] " << str << RESET << std::endl;
			break ;
	}
}

void	Engine::log(const std::string &str)
{
	Engine::log(str, LogSeverity::NORMAL);
}
