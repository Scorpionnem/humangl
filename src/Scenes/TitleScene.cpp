/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:39:14 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 11:22:17 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scene.hpp"
#include "TitleScene.hpp"
#include "SceneManager.hpp"
#include "ShaderManager.hpp"
#include "FrameBuffer.hpp"
#include "Skybox.hpp"
#include "Engine.hpp"

std::string	getFPSString(bool debug)
{
	double	currentTime = glfwGetTime();
	static double		lastUpdate = 0;
	static double		lastMinMaxUpdate = 0;
	static std::string	fpsString = "0 fps";
	static std::string	fpsStringDebug = "0 fps | 0 min | 0 max";

	static int	minFPS = INT_MAX;
	static int	maxFPS = 0;

	if (currentTime - lastMinMaxUpdate >= 10)
	{
		minFPS = INT_MAX;
		maxFPS = 0;
		lastMinMaxUpdate = currentTime;
	}

	int currentFPS = (int)(1.0f / Engine::Window->getDeltaTime());
	if (currentFPS > maxFPS)
		maxFPS = currentFPS;
	if (currentFPS < minFPS)
		minFPS = currentFPS;

	if (currentTime - lastUpdate >= 0.1)
	{
		fpsString = std::to_string(currentFPS) + " fps";
		if (debug)
		{
			fpsStringDebug = fpsString;
			fpsStringDebug += " |" + std::to_string(minFPS) + " min";
			fpsStringDebug += " |" + std::to_string(maxFPS) + " max";
		}
		lastUpdate = currentTime;
	}
	if (debug)
		return (fpsStringDebug);
	return (fpsString);
}

static void	_buildMainInterface(Interface *interface)
{
	interface->addElement("button_start", new Button(UIAnchor::UI_CENTER, "start", glm::vec2(0, -90), glm::vec2(300, 80), NULL, NULL));

	interface->addElement("button_options", new Button(UIAnchor::UI_CENTER, "options", glm::vec2(0, 0), glm::vec2(300, 80), NULL, NULL));

	interface->addElement("button_quit", new Button(UIAnchor::UI_CENTER, "quit", glm::vec2(0, 90), glm::vec2(300, 80), [](ButtonInfo){Engine::Window->close();}, NULL));
}

static void	_buildInterface(Scene *scene)
{
	InterfaceManager	*manager = scene->getInterfaceManager();

	Interface	*main = manager->load("main");
	_buildMainInterface(main);

	Interface	*debug = manager->load("debug");

	debug->addElement("text_fps", new Text(UIAnchor::UI_TOP_LEFT, "fps", glm::vec2(0, 0),
		[](std::string &label)
		{
			label = getFPSString(false);
		}, false));	
}

static void	_frameKeyHook(Scene *)
{
}

static void	_updateShaders(void)
{
	Shader	*textShader = Engine::Shaders->get("text");

	textShader->use();
	textShader->setFloat("time", glfwGetTime());
	textShader->setFloat("SCREEN_WIDTH", Engine::Window->getWidth());
	textShader->setFloat("SCREEN_HEIGHT", Engine::Window->getHeight());
}

static void	_keyHookFunc(Scene *ptr, int key, int action)
{
	TitleScene	*scene = static_cast<TitleScene*>(ptr);

	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
		scene->setDebug(!scene->getDebug());

	scene->getInterfaceManager()->getCurrent()->specialInput(key, action);
}

static void	_charHookFunc(Scene *scene, uint key)
{
	scene->getInterfaceManager()->getCurrent()->charInput(key);
}


#include "Timeline.hpp"

Timeline	testtimeline;
Timeline	testtimeline2;

static void	_render(Scene *ptr)
{
	TitleScene	*scene = static_cast<TitleScene*>(ptr);

    glDisable(GL_DEPTH_TEST);

	FrameBuffer::drawFrame(Engine::Shaders->get("title_bg"), Engine::Textures->get(MBATTY_TEXTURE_PATH)->getID());
	scene->getInterfaceManager()->draw();

	if (scene->getDebug())
		scene->getInterfaceManager()->get("debug")->draw();
	
	Shader	*shader = Engine::Shaders->get("colored_quad");

	shader->setVec3("color", glm::vec3(0, 1, 0));
		UIElement::draw(shader, glm::vec2(testtimeline.getTranslation()), glm::vec2(50, 50));
	shader->setVec3("color", glm::vec3(1, 0, 0));
		UIElement::draw(shader, glm::vec2(testtimeline.getTranslation() + testtimeline2.getTranslation()), glm::vec2(50, 50));

	testtimeline.draw();

    glEnable(GL_DEPTH_TEST);
}

static void	_update(Scene *ptr)
{
	TitleScene	*scene = static_cast<TitleScene*>(ptr);

	testtimeline.update(Engine::Window->getDeltaTime());
	testtimeline2.update(Engine::Window->getDeltaTime());

	if (scene->getDebug())
		scene->getInterfaceManager()->get("debug")->update();

	_frameKeyHook(scene);
	scene->getInterfaceManager()->update();
	_updateShaders();
}

static void	_close(Scene *scene)
{
	(void)scene;
}

static void	_open(Scene *scene)
{
	scene->getInterfaceManager()->use("main");
}

TitleScene::TitleScene()
{
	_buildInterface(this);
	this->setKeyHook(_keyHookFunc);
	this->setCharHook(_charHookFunc);

	this->setRender(_render);
	this->setUpdate(_update);
	this->setOpen(_open);
	this->setClose(_close);

	testtimeline.addKeyFrame(KeyFrame(0, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(0.3, {100, 100, 100}, {0, 0, 0}, {1, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(2, {100, 0, 100}, {0, 0, 0}, {1, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(2.3, {0, 300, 100}, {0, 0, 0}, {1, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(4, {0, 0, 100}, {0, 0, 0}, {1, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(5, {400, 50, 100}, {0, 0, 0}, {1, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(6, {100, 100, 100}, {0, 0, 0}, {1, 1, 1}));

	testtimeline2.addKeyFrame(KeyFrame(0, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}));
	testtimeline2.addKeyFrame(KeyFrame(0.2, {50, 50, 50}, {0, 0, 0}, {1, 1, 1}));
	testtimeline2.addKeyFrame(KeyFrame(0.4, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}));
}

TitleScene::~TitleScene()
{
	
}
