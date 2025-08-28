/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 22:05:11 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

float	FOV = 80;
float	SCREEN_WIDTH = 860;
float	SCREEN_HEIGHT = 520;
float	RENDER_DISTANCE = 1024;

bool		F3 = false;
bool		PAUSED = false;

void	closeWindow()
{
	if (!Engine::Window->up())
		return ;
	glfwSetWindowShouldClose(Engine::Window->getWindowData(), true);
	Engine::log("Quitting game.", LogSeverity::WARNING);
}

void	closeWindow(ButtonInfo)
{
	closeWindow();
}

/*
	Function used to load all textures, as with the shaders, id recommend loading textures at the start especially if they are big
*/
void	build(TextureManager *textures)
{
	Engine::log("Loading textures...", LogSeverity::NORMAL);
	textures->load(MBATTY_TEXTURE_PATH);
	textures->load(STONE_TEXTURE_PATH);
	textures->load(DIRT_TEXTURE_PATH);
	textures->load(GRASS_TOP_TEXTURE_PATH);
	textures->load(GRASS_SIDE_TEXTURE_PATH);
	textures->load(COBBLESTONE_TEXTURE_PATH);
	Engine::log("Finished loading textures", LogSeverity::SUCCESS);
}

#include "TitleScreen.hpp"

void	keyboard_input(GLFWwindow *, unsigned int key)
{
	Engine::Scenes->getCurrent()->charHook(key);
}

void	move_mouse_hook(GLFWwindow*, double xpos, double ypos)
{
	Engine::Scenes->getCurrent()->moveMouseHook(xpos, ypos);
}

void scroll_callback(GLFWwindow *, double xoffset, double yoffset)
{
	Engine::Scenes->getCurrent()->mouseScrollHook(xoffset, yoffset);
}

void	press_mouse_hook(GLFWwindow*, int button, int action, int mods)
{
	Engine::Scenes->getCurrent()->mouseBtnHookFunc(button, action, mods);
}

void	key_hook(GLFWwindow *window, int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && Engine::Window->up() && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		Engine::log("Quitting game.", LogSeverity::WARNING);
	}
	else
		Engine::Scenes->getCurrent()->keyHook(key, action);
}

void	setupScenes()
{
	Scene	*titleScene = Engine::Scenes->load("title_scene", TitleScreen::build, TitleScreen::destructor, TitleScreen::render, TitleScreen::update);
	titleScene->setClose(TitleScreen::close);
	titleScene->setOpen(TitleScreen::open);
	Engine::Scenes->use("title_scene");
}

int	main()
{
	try {
		Engine::init();
		
		setupScenes();

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
