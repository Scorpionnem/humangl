/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:33:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/27 18:02:23 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
#include "Font.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "FrameBuffer.hpp"
#include "InterfaceManager.hpp"
#include "UIElement.hpp"
#include "Terminal.hpp"
#include "SceneManager.hpp"

float	FOV = 80;
float	SCREEN_WIDTH = 860;
float	SCREEN_HEIGHT = 520;
float	RENDER_DISTANCE = 1024;

bool		F3 = false;
bool		PAUSED = false;

Font				*FONT;
Window				*WINDOW;
Camera				*CAMERA;
Skybox				*SKYBOX;
TextureManager		*TEXTURE_MANAGER;
ShaderManager		*SHADER_MANAGER;
SceneManager		*SCENE_MANAGER;
FrameBuffer			*MAIN_FRAME_BUFFER;

void	closeWindow()
{
	if (!WINDOW->up())
		return ;
	glfwSetWindowShouldClose(WINDOW->getWindowData(), true);
	consoleLog("Quitting game.", LogSeverity::WARNING);
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
	consoleLog("Loading textures...", LogSeverity::NORMAL);
	textures->load(MBATTY_TEXTURE_PATH);
	textures->load(STONE_TEXTURE_PATH);
	textures->load(DIRT_TEXTURE_PATH);
	textures->load(GRASS_TOP_TEXTURE_PATH);
	textures->load(GRASS_SIDE_TEXTURE_PATH);
	textures->load(COBBLESTONE_TEXTURE_PATH);
	consoleLog("Finished loading textures", LogSeverity::SUCCESS);
}

/*
	Function used to build and load all shaders used in the program, shaders can be built later on during execution but I wouldnt recommend it
*/
void	build(ShaderManager *shader)
{
	consoleLog("Building shaders...", LogSeverity::NORMAL);
	Shader *textShader = shader->load({"text", TEXT_VERT_SHADER_PATH, TEXT_FRAG_SHADER_PATH});
	Shader *skyboxShader = shader->load({"skybox", SKYBOX_VERT_SHADER_PATH, SKYBOX_FRAG_SHADER_PATH});
	Shader *postShader = shader->load({"post", POST_VERT_SHADER_PATH, POST_FRAG_SHADER_PATH});
	Shader *guiShader = shader->load({"gui", GUI_VERT_SHADER_PATH, GUI_FRAG_SHADER_PATH});
	Shader *titleBackground = shader->load({"title_bg", BACKGROUND_VERT_SHADER_PATH, BACKGROUND_FRAG_SHADER_PATH});
	shader->load({"colored_quad", "shaders/coloredquad.vs", "shaders/coloredquad.fs"});

	guiShader->setInt("tex0", 0);

	Texture::use("tex0", 0, 0, textShader);

	Texture::use("skybox", 0, 0, skyboxShader);

	Texture::use("screenTexture", 0, 0, postShader);
	Texture::use("depthTex", 0, 1, postShader);

	Texture::use("screenTexture", 0, 0, titleBackground);

	consoleLog("Finished building shaders", LogSeverity::SUCCESS);
}

/*
	Small structure used to load all necessary classes in order so no weird stuff should happen
*/
struct	Engine
{
	Engine()
	{
		srand(std::time(NULL));
		WINDOW = new Window();
		SCENE_MANAGER = new SceneManager();
		TEXTURE_MANAGER = new TextureManager();
		build(TEXTURE_MANAGER);
		SHADER_MANAGER = new ShaderManager();
		build(SHADER_MANAGER);
		FONT = new Font();
		MAIN_FRAME_BUFFER = new FrameBuffer();
		SKYBOX = new Skybox({SKYBOX_PATHES});
	}
	~Engine()
	{
		delete SKYBOX;
		delete MAIN_FRAME_BUFFER;
		delete FONT;
		delete SHADER_MANAGER;
		delete TEXTURE_MANAGER;
		delete SCENE_MANAGER;
		consoleLog("Done.", LogSeverity::SUCCESS);
		delete WINDOW;
	}
};

#include "TitleScreen.hpp"

void	keyboard_input(GLFWwindow *, unsigned int key)
{
	SCENE_MANAGER->getCurrent()->charHook(key);
}

void	move_mouse_hook(GLFWwindow*, double xpos, double ypos)
{
	SCENE_MANAGER->getCurrent()->moveMouseHook(xpos, ypos);
}

void scroll_callback(GLFWwindow *, double xoffset, double yoffset)
{
	SCENE_MANAGER->getCurrent()->mouseScrollHook(xoffset, yoffset);
}

void	press_mouse_hook(GLFWwindow*, int button, int action, int mods)
{
	SCENE_MANAGER->getCurrent()->mouseBtnHookFunc(button, action, mods);
}

void	key_hook(GLFWwindow *window, int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && WINDOW->up() && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		consoleLog("Quitting game.", LogSeverity::WARNING);
	}
	else
		SCENE_MANAGER->getCurrent()->keyHook(key, action);
}

void	setupScenes()
{
	Scene	*titleScene = SCENE_MANAGER->load("title_scene", TitleScreen::build, TitleScreen::destructor, TitleScreen::render, TitleScreen::update);
	titleScene->setClose(TitleScreen::close);
	titleScene->setOpen(TitleScreen::open);
	SCENE_MANAGER->use("title_scene");
}

int	main()
{
	consoleLog("Starting...", NORMAL);

	try {
		Engine	engine;

		setupScenes();

		consoleLog("Starting rendering...", NORMAL);
		while (WINDOW->up())
		{
			WINDOW->loopStart();

			SCENE_MANAGER->update();
			SCENE_MANAGER->render();

			WINDOW->loopEnd();
		}
	} catch (const std::exception &e) {
		consoleLog("Program terminated: " + std::string(e.what()), LogSeverity::MELTDOWN);
		return (1);
	}
}
