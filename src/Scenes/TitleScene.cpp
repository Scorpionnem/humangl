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
#include "AnimationManager.hpp"

AnimationManager	anims;

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

bool	redoMainInterface = true;
KeyFrame<glm::vec3> *selectedKeyframe = NULL;

void	addKeyFrameButton(Interface *interface, std::string id, float x, float y, KeyFrame<glm::vec3> &keyframe)
{
	interface->addElement(id + std::to_string(keyframe.getTime()), new Toggle(UIAnchor::UI_BOTTOM_LEFT, id + std::to_string(keyframe.getTime()), glm::vec2(x, y), glm::vec2(20, 20), []
	(ToggleInfo infos)
	{
		selectedKeyframe = static_cast<KeyFrame<glm::vec3>*>(infos.data);
		selectedKeyframe->setTime(selectedKeyframe->getTime() + 0.3);
		redoMainInterface = true;
	}, &keyframe));
}

static void	_buildMainInterface(Interface *interface)
{
	interface->clear();
	interface->addElement("keyframe_options", new Button(UIAnchor::UI_BOTTOM_RIGHT, "keyframe_options", glm::vec2(0, -120), glm::vec2(128, Engine::Window->getHeight() - 120), NULL, NULL));

	Timeline *timeline = anims.getCurrent()->get("torso");
	if (!timeline)
		return ;
	float	biggestTime = timeline->getBiggestTime();
	float	maxWidth = (Engine::Window->getWidth() - 148);
	for (KeyFrame<glm::vec3> &keyframe : timeline->getKeyFrames(KeyFrameType::ROTATION))
	{
		float	offsetX = keyframe.getTime() / biggestTime;
		float	posX = offsetX * maxWidth;
		addKeyFrameButton(interface, "kfr_", posX, 0, keyframe);
	}
	for (KeyFrame<glm::vec3> &keyframe : timeline->getKeyFrames(KeyFrameType::SCALE))
	{
		float	offsetX = keyframe.getTime() / biggestTime;
		float	posX = offsetX * maxWidth;
		addKeyFrameButton(interface, "kfs_", posX, -21, keyframe);
	}
	for (KeyFrame<glm::vec3> &keyframe : timeline->getKeyFrames(KeyFrameType::TRANSLATION))
	{
		float	offsetX = keyframe.getTime() / biggestTime;
		float	posX = offsetX * maxWidth;
		addKeyFrameButton(interface, "kft_", posX, -42, keyframe);
	}
}

static void	_buildInterface(Scene *scene)
{
	InterfaceManager	*manager = scene->getInterfaceManager();

	manager->load("main");

	Interface	*debug = manager->load("debug");

	debug->addElement("text_fps", new Text(UIAnchor::UI_TOP_LEFT, "fps", glm::vec2(0, 0),
		[](std::string &label)
		{
			label = getFPSString(false);
		}, false));	
}

static Camera	camera;

static void	_frameKeyHook(Scene *)
{
	float	cameraSpeed = 1 * Engine::Window->getDeltaTime();
	float	speedBoost = 1.0f;

	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		speedBoost = 20.0f;

	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		speedBoost *= 15.0f;

	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_W) == GLFW_PRESS)
		camera.pos = camera.pos + camera.front * (cameraSpeed * speedBoost);
	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_S) == GLFW_PRESS)
		camera.pos = camera.pos - camera.front * (cameraSpeed * speedBoost);
	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.pos = camera.pos + camera.worldUp * (cameraSpeed * speedBoost);
	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.pos = camera.pos - camera.worldUp * (cameraSpeed * speedBoost);

	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_A) == GLFW_PRESS)
		camera.pos = camera.pos - glm::normalize(glm::cross(camera.front, camera.worldUp)) * (cameraSpeed * speedBoost);
	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_D) == GLFW_PRESS)
		camera.pos = camera.pos + glm::normalize(glm::cross(camera.front, camera.worldUp)) * (cameraSpeed * speedBoost);

	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(Engine::Window->data(), GLFW_TRUE);
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

void	_draw2D(TitleScene *scene)
{
    glDisable(GL_DEPTH_TEST);

	scene->getInterfaceManager()->draw();

	scene->getInterfaceManager()->get("debug")->draw();
	
	glEnable(GL_DEPTH_TEST);
}

#include "Model.hpp"

Model	model;

static void	_render(Scene *ptr)
{
	TitleScene	*scene = static_cast<TitleScene*>(ptr);
	(void)scene;

	Shader	*shader2 = Engine::Shaders->get("cube");

	shader2->use();
	camera.setViewMatrix(*shader2);

	model.draw();

	_draw2D(scene);
}

void	_moveMouseHookFunc(Scene*, double xpos, double ypos)
{
	float xoffset = xpos - Engine::Window->getLastMouseX();
	float yoffset = Engine::Window->getLastMouseY() - ypos;

	Engine::Window->setLastMouseX(xpos);
	Engine::Window->setLastMouseY(ypos);

	const float sensitivity = 0.1f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.yaw += xoffset;
	camera.pitch += yoffset;

	if (camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if (camera.pitch < -89.0f)
		camera.pitch = -89.0f;
	if (camera.yaw > 360)
		camera.yaw = 0;
	if (camera.yaw < 0)
		camera.yaw = 360;
}

static void	_update(Scene *ptr)
{
	TitleScene	*scene = static_cast<TitleScene*>(ptr);

	if (redoMainInterface)
	{
		_buildMainInterface(scene->getInterfaceManager()->get("main"));
		redoMainInterface = false;	
	}

	scene->body.update(glm::mat4(1.0));
	model.update();

	scene->getInterfaceManager()->get("debug")->update();

	_frameKeyHook(scene);
	camera.update();
	scene->getInterfaceManager()->update();
	_updateShaders();
}

static void	_close(Scene *scene)
{
	(void)scene;
}

static void	_open(Scene *scene)
{
	// Engine::Window->setDefaultMousePos();
	scene->getInterfaceManager()->use("main");
}

Timeline	torso;
Timeline	head;

Timeline	upperLeftArm;
Timeline	upperRightArm;
Timeline	lowerLeftArm;
Timeline	lowerRightArm;

Timeline	upperLeftLeg;
Timeline	upperRightLeg;
Timeline	lowerLeftLeg;
Timeline	lowerRightLeg;


TitleScene::TitleScene()
{
	camera.pos = glm::vec3(30, 1, 0);
	camera.yaw = 180.0f;
	camera.pitch = 20.f;

	_buildInterface(this);
	this->setKeyHook(_keyHookFunc);
	this->setCharHook(_charHookFunc);

	this->setRender(_render);
	this->setUpdate(_update);
	this->setOpen(_open);
	this->setClose(_close);

	this->setMoveMouseHook(_moveMouseHookFunc);


	torso.addKeyFrame(KeyFrameType::TRANSLATION, KeyFrame(0.0, glm::vec3(0.0f)));
	torso.addKeyFrame(KeyFrameType::TRANSLATION, KeyFrame(3.0, glm::vec3(10.0f, 10, 10)));

	torso.addKeyFrame(KeyFrameType::SCALE, KeyFrame(0.0, glm::vec3(5.0f)));
	torso.addKeyFrame(KeyFrameType::SCALE, KeyFrame(1.0, glm::vec3(10.0f)));
	torso.addKeyFrame(KeyFrameType::SCALE, KeyFrame(2.0, glm::vec3(5.0f)));

	torso.addKeyFrame(KeyFrameType::ROTATION, KeyFrame(0.0, glm::vec3(0.0f)));
	torso.addKeyFrame(KeyFrameType::ROTATION, KeyFrame(2.0, glm::vec3(0.0f, 360.0, 0.0)));

	model.addPart(new Part("torso", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(6,99,6) / 255.0f));
	model.addPart(new Part("upperLeftArm", glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(224,171,148) / 255.0f));
	model.addPart(new Part("upperRightArm", glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(224,171,148) / 255.0f));
	model.setChild("upperLeftArm", new Part("lowerLeftArm", glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(224,171,148) / 255.0f));
	model.setChild("upperRightArm", new Part("lowerRightArm", glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(224,171,148) / 255.0f));
	model.addPart(new Part("upperLeftLeg", glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(3,76,153) / 255.0f));
	model.addPart(new Part("upperRightLeg", glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(3,76,153) / 255.0f));
	model.setChild("upperLeftLeg", new Part("lowerLeftLeg", glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(3,76,153) / 255.0f));
	model.setChild("upperRightLeg", new Part("lowerRightLeg", glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(3,76,153) / 255.0f));

	model.setChild("torso", new Part("head", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(224,171,148) / 255.0f));
	model.setChild("torso", "upperLeftArm");
	model.setChild("torso", "upperRightArm");
	model.setChild("torso", "upperLeftLeg");
	model.setChild("torso", "upperRightLeg");

	model.setRoot("torso");

	try {
		anims.load("walking", "assets/test.hgl");
		anims.play("walking", model);
	} catch (const std::exception &e) {
		Engine::log(e.what());
	}

	// body.exportAnimation("assets/test.hgl");
}

TitleScene::~TitleScene()
{
}
