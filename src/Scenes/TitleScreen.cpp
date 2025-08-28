/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScreen.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:39:14 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 21:59:34 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scene.hpp"
#include "TitleScreen.hpp"
#include "SceneManager.hpp"
#include "ShaderManager.hpp"
#include "FrameBuffer.hpp"
#include "Skybox.hpp"
#include "Engine.hpp"

void	closeWindow(ButtonInfo);

#define TITLES_COUNT 5
#define TITLE_TIME 8

std::string	popupTitles[TITLES_COUNT] =
{
	"by mbatty and mbirou!",
	"outstanding project!",
	"42 angouleme",
	"dont forget to git push",
	"c++ > all"
};

static void	_buildMainInterface(Interface *interface)
{
	interface->addElement("button_start", new Button(UIAnchor::UI_CENTER, "start", glm::vec2(0, -90), glm::vec2(300, 80), NULL, NULL));

	interface->addElement("button_options", new Button(UIAnchor::UI_CENTER, "options", glm::vec2(0, 0), glm::vec2(300, 80), NULL, NULL));

	interface->addElement("button_quit", new Button(UIAnchor::UI_CENTER, "quit", glm::vec2(0, 90), glm::vec2(300, 80), closeWindow, NULL));

	// Text	*text_popup = static_cast<Text*>(interface->addElement("text_popup", new Text(UIAnchor::UI_TOP_CENTER_HALF, "by mbatty and mbirou!", glm::vec2(175, -40), NULL, false)));
	// interface->addElement("image_icon", new Image(UIAnchor::UI_TOP_CENTER_HALF, glm::vec2(0, 0), glm::vec2(400, 150)));

	// text_popup->setRotation(glm::vec3(0.0, 0.0, 1.0));
	// text_popup->setAngle(-10);
	// text_popup->setColor(glm::vec3(1.0, 1.0, 0.0));

	// interface->setUpdateFunc([]
	// 	(Interface *interface)
	// 	{
	// 		static double	lastUpdate = 0;
	// 		Text		*text_popup = static_cast<Text*>(interface->getElement("text_popup"));

	// 		if (glfwGetTime() - lastUpdate >= TITLE_TIME)
	// 		{
	// 			text_popup->setText(popupTitles[rand() % TITLES_COUNT]);
	// 			lastUpdate = glfwGetTime();
	// 		}

	// 		float	scale = 1 + std::abs(cos(glfwGetTime() * 5)) / 10;

	// 		text_popup->setScale(glm::vec2(scale, scale));
	// 	});
}

static void	_buildInterface(Scene *scene)
{
	InterfaceManager	*manager = scene->getInterfaceManager();

	Interface	*main = manager->load("main");
	_buildMainInterface(main);
}

static void	_frameKeyHook(Scene *scene)
{
	(void)scene;
}

static void	_updateShaders(void)
{
	Shader	*textShader = Engine::Shaders->get("text");

	textShader->use();
	textShader->setFloat("time", glfwGetTime());
	textShader->setFloat("SCREEN_WIDTH", SCREEN_WIDTH);
	textShader->setFloat("SCREEN_HEIGHT", SCREEN_HEIGHT);
}

static void	_keyHookFunc(Scene *scene, int key, int action)
{
	scene->getInterfaceManager()->getCurrent()->specialInput(key, action);
}

static void	_charHookFunc(Scene *scene, uint key)
{
	scene->getInterfaceManager()->getCurrent()->charInput(key);
}

void	TitleScreen::build(Scene *scene)
{
	_buildInterface(scene);
	scene->setKeyHook(_keyHookFunc);
	scene->setCharHook(_charHookFunc);
	scene->getInterfaceManager()->use("main");
}

void	TitleScreen::destructor(Scene *scene)
{
	(void)scene;
}

void	TitleScreen::render(Scene *scene)
{
    glDisable(GL_DEPTH_TEST);
	FrameBuffer::drawFrame(Engine::Shaders->get("title_bg"), Engine::Textures->get(MBATTY_TEXTURE_PATH)->getID());
	scene->getInterfaceManager()->draw();
    glEnable(GL_DEPTH_TEST);
}

void	TitleScreen::update(Scene *scene)
{
	_frameKeyHook(scene);
	scene->getInterfaceManager()->update();
	_updateShaders();
}

void	TitleScreen::close(Scene *scene)
{
	(void)scene;
}

void	TitleScreen::open(Scene *scene)
{
	(void)scene;
	scene->getInterfaceManager()->use("main");
}
