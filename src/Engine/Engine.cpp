/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:52:22 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 11:22:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"
#include "TitleScene.hpp"

Window			*Engine::Window;
ShaderManager	*Engine::Shaders;
TextureManager	*Engine::Textures;
Font			*Engine::Font;
SceneManager	*Engine::Scenes;
FrameBuffer		*Engine::FrameBuffer;

static void	_buildTextures()
{
	Engine::log("Loading textures...", LogSeverity::NORMAL);
	Engine::Textures->load(MBATTY_TEXTURE_PATH);
	Engine::Textures->load(STONE_TEXTURE_PATH);
	Engine::Textures->load(DIRT_TEXTURE_PATH);
	Engine::Textures->load(GRASS_TOP_TEXTURE_PATH);
	Engine::Textures->load(GRASS_SIDE_TEXTURE_PATH);
	Engine::Textures->load(COBBLESTONE_TEXTURE_PATH);
	Engine::log("Finished loading textures", LogSeverity::SUCCESS);
}

static void	_buildShaders()
{
	Engine::log("Building shaders...", LogSeverity::NORMAL);
	Shader *textShader = Engine::Shaders->load({"text", TEXT_VERT_SHADER_PATH, TEXT_FRAG_SHADER_PATH});
	Shader *skyboxShader = Engine::Shaders->load({"skybox", SKYBOX_VERT_SHADER_PATH, SKYBOX_FRAG_SHADER_PATH});
	Shader *postShader = Engine::Shaders->load({"post", POST_VERT_SHADER_PATH, POST_FRAG_SHADER_PATH});
	Shader *guiShader = Engine::Shaders->load({"gui", GUI_VERT_SHADER_PATH, GUI_FRAG_SHADER_PATH});
	Shader *titleBackground = Engine::Shaders->load({"title_bg", BACKGROUND_VERT_SHADER_PATH, BACKGROUND_FRAG_SHADER_PATH});
	Engine::Shaders->load({"colored_quad", "shaders/coloredquad.vs", "shaders/coloredquad.fs"});

	guiShader->setInt("tex0", 0);

	Texture::use("tex0", 0, 0, textShader);

	Texture::use("skybox", 0, 0, skyboxShader);

	Texture::use("screenTexture", 0, 0, postShader);
	Texture::use("depthTex", 0, 1, postShader);

	Texture::use("screenTexture", 0, 0, titleBackground);

	Engine::log("Finished building shaders", LogSeverity::SUCCESS);
}

static void	_buildScenes()
{
	Engine::Scenes->load("title_scene", new TitleScene());
	Engine::Scenes->use("title_scene");
}

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
	Engine::Textures = new TextureManager();
	Engine::Scenes = new SceneManager();
	Engine::FrameBuffer = new class FrameBuffer();

	_buildShaders();
	_buildTextures();
	_buildScenes();

	Engine::Font = new class Font();
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

void	Engine::keyboardCharHook(GLFWwindow *, unsigned int key)
{
	Engine::Scenes->getCurrent()->charHook(key);
}

void	Engine::moveMouseHook(GLFWwindow*, double xpos, double ypos)
{
	Engine::Scenes->getCurrent()->moveMouseHook(xpos, ypos);
}

void	Engine::scrollMouseHook(GLFWwindow *, double xoffset, double yoffset)
{
	Engine::Scenes->getCurrent()->mouseScrollHook(xoffset, yoffset);
}

void	Engine::clickMouseHook(GLFWwindow*, int button, int action, int mods)
{
	Engine::Scenes->getCurrent()->mouseBtnHookFunc(button, action, mods);
}

void	Engine::keyboardKeyHook(GLFWwindow *window, int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && Engine::Window->up() && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		Engine::log("Quitting game.", LogSeverity::WARNING);
	}
	else
		Engine::Scenes->getCurrent()->keyHook(key, action);
}
