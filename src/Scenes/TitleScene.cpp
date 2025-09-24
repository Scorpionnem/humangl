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

std::string			animationId = "";
std::string			modelId = "";
bool				redointerface = false;
AnimationManager	anims;
Part				*selectedPart = NULL;

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

std::string _getTimeString()
{
	std::time_t t = std::time(0);
	std::tm* tm = std::localtime(&t);
	char buf[64];

	std::strftime(buf, sizeof(buf), "%d_%m_%Y-%H_%M_%S", tm);
	return std::string(buf);
}

bool	redoMainInterface = true;
KeyFrame<glm::vec3> *selectedKeyframe = NULL;

void	addKeyFrameButton(Interface *interface, std::string id, float x, float y, KeyFrame<glm::vec3> &keyframe)
{
	interface->addElement(id, new Toggle(UIAnchor::UI_BOTTOM_LEFT, id, glm::vec2(x, y), glm::vec2(20, 20), []
	(ToggleInfo infos)
	{
		Interface *editor = Engine::Scenes->getCurrent()->getInterfaceManager()->get("editor");
		if (!editor)
			return ;

		selectedKeyframe = static_cast<KeyFrame<glm::vec3>*>(infos.data);
		if (!selectedKeyframe)
			return ;

		static_cast<TextBox*>(editor->getElement("keyframe_editor_x"))->input = std::to_string(selectedKeyframe->getValue().x);
		static_cast<TextBox*>(editor->getElement("keyframe_editor_y"))->input = std::to_string(selectedKeyframe->getValue().y);
		static_cast<TextBox*>(editor->getElement("keyframe_editor_z"))->input = std::to_string(selectedKeyframe->getValue().z);
		static_cast<TextBox*>(editor->getElement("keyframe_editor_time"))->input = std::to_string(selectedKeyframe->getTime());
	}, NULL, &keyframe));
}

static void	_buildEditorInterface(Interface *interface)
{
	interface->addElement("keyframe_editor_x", new TextBox(UIAnchor::UI_CENTER_RIGHT, "X", glm::vec2(0, 0), glm::vec2(200, 40), []
	(TextBoxInfo infos)
	{
		if (selectedKeyframe && infos.input.size())
			selectedKeyframe->setValue({std::atof(infos.input.c_str()), selectedKeyframe->getValue().y, selectedKeyframe->getValue().z});
		else
			infos.input = "";
	}, NULL));
	interface->addElement("keyframe_editor_y", new TextBox(UIAnchor::UI_CENTER_RIGHT, "Y", glm::vec2(0, 41), glm::vec2(200, 40), []
	(TextBoxInfo infos)
	{
		if (selectedKeyframe && infos.input.size())
			selectedKeyframe->setValue({selectedKeyframe->getValue().x, std::atof(infos.input.c_str()), selectedKeyframe->getValue().z});
		else
			infos.input = "";
	}, NULL));
	interface->addElement("keyframe_editor_z", new TextBox(UIAnchor::UI_CENTER_RIGHT, "Z", glm::vec2(0, 82), glm::vec2(200, 40), []
	(TextBoxInfo infos)
	{
		if (selectedKeyframe && infos.input.size())
			selectedKeyframe->setValue({selectedKeyframe->getValue().x, selectedKeyframe->getValue().y, std::atof(infos.input.c_str())});
		else
			infos.input = "";
	}, NULL));
	interface->addElement("bodypartselector", new TextBox(UIAnchor::UI_CENTER_RIGHT, "Body Part", glm::vec2(0, 123), glm::vec2(200, 40), []
	(TextBoxInfo infos)
	{
		if (!anims.getAnimation(animationId)->get(infos.input))
		{
			infos.input = "";
		}

		selectedPart = anims.getAnimationModel(animationId)->getPart(infos.input);
		if (!selectedPart)
			return ;

		modelId = selectedPart->id();

		Interface *editor = Engine::Scenes->getCurrent()->getInterfaceManager()->get("editor");
		if (!editor)
			return ;

		static_cast<TextBox*>(editor->getElement("keyframe_editor_x"))->input = "";
		static_cast<TextBox*>(editor->getElement("keyframe_editor_y"))->input = "";
		static_cast<TextBox*>(editor->getElement("keyframe_editor_z"))->input = "";
		static_cast<TextBox*>(editor->getElement("keyframe_editor_time"))->input = "";

		selectedKeyframe = NULL;
		redointerface = true;
	}, NULL));
	interface->addElement("keyframe_editor_time", new TextBox(UIAnchor::UI_CENTER_RIGHT, "Time", glm::vec2(0, -41), glm::vec2(200, 40), []
	(TextBoxInfo infos)
	{
		if (selectedKeyframe && infos.input.size())
			selectedKeyframe->setTime(std::atof(infos.input.c_str()));
		else
			infos.input = "";
		redointerface = true;
	}, NULL));



	interface->addElement("keyframeaddt", new Button(UIAnchor::UI_CENTER_RIGHT, "kft", glm::vec2(-160, 164), glm::vec2(40, 40), []
	(ButtonInfo )
	{
		Animation	*anim = anims.getAnimation(animationId);
		if (!anim || !anim->get(modelId))
			return ;
		anim->addKeyFrame(modelId, KeyFrameType::TRANSLATION, KeyFrame<glm::vec3>(anim->get(modelId)->getBiggestTime() + 1, glm::vec3(0), KeyFrameType::TRANSLATION));
		redointerface = true;
	}, NULL));
	interface->addElement("keyframeaddr", new Button(UIAnchor::UI_CENTER_RIGHT, "kfr", glm::vec2(-100, 164), glm::vec2(40, 40), []
	(ButtonInfo )
	{
		Animation	*anim = anims.getAnimation(animationId);
		if (!anim || !anim->get(modelId))
			return ;
		anim->addKeyFrame(modelId, KeyFrameType::ROTATION, KeyFrame<glm::vec3>(anim->get(modelId)->getBiggestTime() + 1, glm::vec3(0), KeyFrameType::ROTATION));
		redointerface = true;
	}, NULL));
	interface->addElement("keyframeadds", new Button(UIAnchor::UI_CENTER_RIGHT, "kfs", glm::vec2(-40, 164), glm::vec2(40, 40), []
	(ButtonInfo )
	{
		Animation	*anim = anims.getAnimation(animationId);
		if (!anim || !anim->get(modelId))
			return ;
		anim->addKeyFrame(modelId, KeyFrameType::SCALE, KeyFrame<glm::vec3>(anim->get(modelId)->getBiggestTime() + 1, glm::vec3(1), KeyFrameType::SCALE));
		redointerface = true;
	}, NULL));
}

static void	_buildMainInterface(Interface *interface)
{
	interface->clear();

	Animation	*anim = anims.getAnimation(animationId);
	if (!anim)
		return ;
		
	Timeline *timeline = anim->get(modelId);
	if (!timeline)
		return ;

	float	biggestTime = std::max(timeline->getBiggestTime(), 1.0f);
	float	maxWidth = (Engine::Window->getWidth() - 148);
	int	i = 0;
	for (KeyFrame<glm::vec3> &keyframe : timeline->getKeyFrames(KeyFrameType::ROTATION))
	{
		i++;
		float	offsetX = keyframe.getTime() / biggestTime;
		float	posX = offsetX * maxWidth;
		addKeyFrameButton(interface, "kfr_" + std::to_string(i), posX, 0, keyframe);
	}
	for (KeyFrame<glm::vec3> &keyframe : timeline->getKeyFrames(KeyFrameType::SCALE))
	{
		i++;
		float	offsetX = keyframe.getTime() / biggestTime;
		float	posX = offsetX * maxWidth;
		addKeyFrameButton(interface, "kfs_" + std::to_string(i), posX, -21, keyframe);
	}
	for (KeyFrame<glm::vec3> &keyframe : timeline->getKeyFrames(KeyFrameType::TRANSLATION))
	{
		i++;
		float	offsetX = keyframe.getTime() / biggestTime;
		float	posX = offsetX * maxWidth;
		addKeyFrameButton(interface, "kft_" + std::to_string(i), posX, -42, keyframe);
	}
}

static void	_buildInterface(Scene *scene)
{
	InterfaceManager	*manager = scene->getInterfaceManager();

	manager->load("main");
	
	_buildEditorInterface(manager->load("editor"));

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

	if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
	{
		if (selectedKeyframe)
		{
			anims.getAnimation(animationId)->get(modelId)->removeKeyFrame(selectedKeyframe->getType(), selectedKeyframe->getTime());
			redointerface = true;
		}
	}

	if (glfwGetKey(Engine::Window->data(), GLFW_KEY_S) == GLFW_PRESS)
		if (glfwGetKey(Engine::Window->data(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			anims.getAnimationModel(animationId)->exportAnimation("./exports/" + _getTimeString() + "_export.hgl");

	scene->getInterfaceManager()->getCurrent()->specialInput(key, action);
}

static void	_charHookFunc(Scene *scene, uint key)
{
	scene->getInterfaceManager()->getCurrent()->charInput(key);
}

void	_selectPart()
{
	FrameBuffer	selection;

	Shader	*shader2 = Engine::Shaders->get("cube");

	shader2->use();
	camera.setViewMatrix(*shader2);
	selection.use();

	glClearColor(0.0f, 0.0, 0.0f, 1.0f);

	anims.getAnimationModel(animationId)->selectDraw();

	double	pos[2] = {0};
	glfwGetCursorPos(Engine::Window->data(), &pos[0], &pos[1]);

	unsigned char data[3] = {0};
	glReadPixels(pos[0], Engine::Window->getHeight() - pos[1], 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

	selectedPart = anims.getAnimationModel(animationId)->getSelected({float(data[0]), float(data[1]), float(data[2])});
	if (selectedPart)
	{
		modelId = selectedPart->id();
		redointerface = true;
	}

	FrameBuffer::reset();
	glClearColor(0.6, 0.8, 1.0, 1.0f);
}

static void	_mouseBtnHook(Scene*, int button, int action, int)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		_selectPart();
}

#include "Timeline.hpp"

void	_draw2D(TitleScene *scene)
{
    glDisable(GL_DEPTH_TEST);

	scene->getInterfaceManager()->draw();

	scene->getInterfaceManager()->get("main")->draw();
	scene->getInterfaceManager()->get("debug")->draw();

	glEnable(GL_DEPTH_TEST);
}

#include "Model.hpp"

static void	_render(Scene *ptr)
{
	TitleScene	*scene = static_cast<TitleScene*>(ptr);

	Shader	*shader2 = Engine::Shaders->get("cube");

	shader2->use();
	camera.setViewMatrix(*shader2);

	anims.getAnimationModel(animationId)->draw();

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

	if (redointerface) {
		redointerface = false;
		_buildMainInterface(scene->getInterfaceManager()->get("main"));
	}

	scene->body.update(glm::mat4(1.0));
	anims.getAnimationModel(animationId)->update();

	scene->getInterfaceManager()->get("debug")->update();
	scene->getInterfaceManager()->get("main")->update();

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
	scene->getInterfaceManager()->use("editor");
}

TitleScene::TitleScene()
{
	camera.pos = glm::vec3(30, 1, 0);
	camera.yaw = 180.0f;
	camera.pitch = 0.f;

	_buildInterface(this);
	this->setKeyHook(_keyHookFunc);
	this->setCharHook(_charHookFunc);

	this->setRender(_render);
	this->setUpdate(_update);
	this->setOpen(_open);
	this->setClose(_close);
	this->setMouseBtnHookFunc(_mouseBtnHook);


	animationId = "walking";

	try {
		anims.load(animationId, "assets/test.hgl");
		anims.play(animationId);
	} catch (const std::exception &e) {
		Engine::log(e.what());
		throw ;
	}

	_buildMainInterface(this->getInterfaceManager()->get("main"));
}

TitleScene::~TitleScene()
{
}
