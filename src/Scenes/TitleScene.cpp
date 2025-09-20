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

static void	_buildMainInterface(Interface *interface)
{
	(void)interface;
	// interface->addElement("button_start", new Button(UIAnchor::UI_CENTER, "start", glm::vec2(0, -90), glm::vec2(300, 80), NULL, NULL));

	// interface->addElement("button_options", new Button(UIAnchor::UI_CENTER, "options", glm::vec2(0, 0), glm::vec2(300, 80), NULL, NULL));

	// interface->addElement("button_quit", new Button(UIAnchor::UI_CENTER, "quit", glm::vec2(0, 90), glm::vec2(300, 80), [](ButtonInfo){Engine::Window->close();}, NULL));
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

// void	_draw2D(TitleScene *scene)
// {
//     glDisable(GL_DEPTH_TEST);

// 	FrameBuffer::drawFrame(Engine::Shaders->get("title_bg"), Engine::Textures->get(SAND_TEXTURE_PATH)->getID());
// 	scene->getInterfaceManager()->draw();

// 	if (scene->getDebug())
// 		scene->getInterfaceManager()->get("debug")->draw();
	
// 	Shader	*shader = Engine::Shaders->get("colored_quad");

// 	shader->setVec3("color", glm::vec3(0, 1, 0));
// 		UIElement::draw(shader, glm::vec2(testtimeline.getTranslation()), glm::vec2(50, 50));
// 	shader->setVec3("color", glm::vec3(1, 0, 0));
// 		UIElement::draw(shader, glm::vec2(testtimeline.getTranslation() + testtimeline2.getTranslation()), glm::vec2(50, 50));

// 	shader->setVec3("color", glm::vec3(0, 0, 1));
// 	UIElement::draw(shader, glm::vec2(testtimeline.getTranslation() + testtimeline2.getTranslation() + testtimeline3.getTranslation()), glm::vec2(50, 50));

// 	testtimeline.draw(Engine::Window->getHeight() - 16);
// 	testtimeline2.draw(Engine::Window->getHeight() - 32);

// 	glEnable(GL_DEPTH_TEST);
// }

#include "Model.hpp"

static void	_render(Scene *ptr)
{
	TitleScene	*scene = static_cast<TitleScene*>(ptr);
	(void)scene;

	// _draw2D(scene);

	Shader	*shader2 = Engine::Shaders->get("cube");

	shader2->use();
	camera.setViewMatrix(*shader2);

	// scene->body.draw();
	anims.getAnimationModel(animationId)->draw();

	// glm::mat4	model = glm::mat4(1.0);

	// glm::vec3	translation = testtimeline.getTranslation();
	// glm::vec3	rotation = testtimeline.getRotation();
	// glm::vec3	scale = testtimeline.getScale();

	// model = glm::translate(model, translation);
	// model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	// model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	// model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	// model = glm::scale(model, scale);

	// shader2->setMat4("model", model);
	// camera.setViewMatrix(*shader2);

	// glBindVertexArray(scene->VAO);
	// glDrawArrays(GL_TRIANGLES, 0, 36);
	// glBindVertexArray(0);
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

	// testtimeline.update(Engine::Window->getDeltaTime());
	scene->body.update(glm::mat4(1.0));
	anims.getAnimationModel(animationId)->update();

	if (scene->getDebug())
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
	Engine::Window->setDefaultMousePos();
	scene->getInterfaceManager()->use("main");
}

// Timeline	torso;
// Timeline	head;

// Timeline	upperLeftArm;
// Timeline	upperRightArm;
// Timeline	lowerLeftArm;
// Timeline	lowerRightArm;

// Timeline	upperLeftLeg;
// Timeline	upperRightLeg;
// Timeline	lowerLeftLeg;
// Timeline	lowerRightLeg;


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


	// torso.addKeyFrame(KeyFrameType::TRANSLATION, KeyFrame(0.0, glm::vec3(0.0f)));
	// torso.addKeyFrame(KeyFrameType::TRANSLATION, KeyFrame(3.0, glm::vec3(10.0f, 10, 10)));

	// torso.addKeyFrame(KeyFrameType::SCALE, KeyFrame(0.0, glm::vec3(5.0f)));
	// torso.addKeyFrame(KeyFrameType::SCALE, KeyFrame(1.0, glm::vec3(10.0f)));
	// torso.addKeyFrame(KeyFrameType::SCALE, KeyFrame(2.0, glm::vec3(5.0f)));

	// torso.addKeyFrame(KeyFrameType::ROTATION, KeyFrame(0.0, glm::vec3(0.0f)));
	// torso.addKeyFrame(KeyFrameType::ROTATION, KeyFrame(2.0, glm::vec3(0.0f, 360.0, 0.0)));

	// torso.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(4.0f, glm::vec3(0.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(4.25f, glm::vec3(0.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(8.75f, glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(8.75f, glm::vec3(0.0f, 0.0f, -50.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(13.25f, glm::vec3(0.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(13.5f, glm::vec3(0.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// // torso.addKeyFrame(KeyFrame(14.0f, glm::vec3(0.0f), glm::vec3(0.0f, .0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(15.0f, glm::vec3(0.0f), glm::vec3(0.0f, 360.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(15.001f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(16.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(17.0f, glm::vec3(0.0f, 7.0f, 0.0f), glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(18.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(19.0f, glm::vec3(0.0f, 7.0f, 0.0f), glm::vec3(0.0f, 135.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(20.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(21.0f, glm::vec3(0.0f, 7.0f, 0.0f), glm::vec3(0.0f, 225.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(22.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(23.0f, glm::vec3(0.0f, 7.0f, 0.0f), glm::vec3(0.0f, 315.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(24.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 360.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(24.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));
	// torso.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 8.0f, 6.0f)));


	// head.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(2.5f, 2.5f, 2.5f)));
	// head.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(2.5f, 2.5f, 2.5f)));


	// upperLeftArm.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(4.25f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(5.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(6.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(7.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(8.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(9.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(10.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(11.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(12.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(13.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(13.5f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(16.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(17.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(18.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(19.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(20.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(21.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(22.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(23.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(24.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftArm.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));

	// upperRightArm.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(4.25f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(5.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(6.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(7.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(8.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(9.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(10.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(11.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(12.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(13.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(13.5f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(16.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(17.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(18.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(19.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(20.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(21.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(22.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(23.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(24.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightArm.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));


	// lowerLeftArm.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(4.25f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(5.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(6.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(8.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(9.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(10.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(12.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(13.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(13.5f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(16.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(24.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftArm.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));

	// lowerRightArm.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(4.25f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(6.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(7.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(8.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(10.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(11.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(12.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(16.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(24.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightArm.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));


	// upperLeftLeg.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(4.25f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(5.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(6.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(7.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(8.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(9.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(10.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(11.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(12.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(13.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(13.5f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(17.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 00.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(18.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(18.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(19.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(20.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(20.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(21.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(22.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(22.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperLeftLeg.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));

	// upperRightLeg.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(-0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(4.25f, glm::vec3(0.0f), glm::vec3(-0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(5.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(6.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(7.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(8.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(9.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(10.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(11.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(12.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(13.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(13.5f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(16.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(17.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 00.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(18.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(18.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(19.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(20.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(20.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(21.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(22.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(22.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// upperRightLeg.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));


	// lowerLeftLeg.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(4.25f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(5.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(6.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(8.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(9.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(10.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(12.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(13.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(13.5f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(17.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 00.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(18.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(18.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(19.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(20.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(20.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(21.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(22.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(22.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerLeftLeg.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));

	// lowerRightLeg.addKeyFrame(KeyFrame(0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(4.25f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(6.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(7.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(8.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(10.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(11.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(12.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(16.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(17.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 00.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(18.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(18.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(19.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(20.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(20.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(21.75f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(22.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(22.25f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));
	// lowerRightLeg.addKeyFrame(KeyFrame(26.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f)));


	// body = Part("torso", torso, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(6,99,6) / 255.0f);

	// Part leftArm = Part("upperLeftArm", upperLeftArm, glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(224,171,148) / 255.0f);
	// Part rightArm = Part("upperRightArm", upperRightArm, glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(224,171,148) / 255.0f);
	// leftArm.addChild(Part("lowerLeftArm", lowerLeftArm, glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(224,171,148) / 255.0f));
	// rightArm.addChild(Part("lowerRightArm", lowerRightArm, glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(224,171,148) / 255.0f));

	// Part leftLeg = Part("upperLeftLeg", upperLeftLeg, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(3,76,153) / 255.0f);
	// Part rightLeg = Part("upperRightLeg", upperRightLeg, glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(3,76,153) / 255.0f);
	// leftLeg.addChild(Part("lowerLeftLeg", lowerLeftLeg, glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(3,76,153) / 255.0f));
	// rightLeg.addChild(Part("lowerRightLeg", lowerRightLeg, glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(3,76,153) / 255.0f));

	// body.addChild(Part("head", head, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(224,171,148) / 255.0f));
	// body.addChild(leftArm);
	// body.addChild(rightArm);
	// body.addChild(leftLeg);
	// body.addChild(rightLeg);



	// model.addPart(new Part("torso", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(6,99,6) / 255.0f));
	// model.addPart(new Part("upperLeftArm", glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(224,171,148) / 255.0f));
	// model.addPart(new Part("upperRightArm", glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(224,171,148) / 255.0f));
	// model.setChild("upperLeftArm", new Part("lowerLeftArm", glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(224,171,148) / 255.0f));
	// model.setChild("upperRightArm", new Part("lowerRightArm", glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(224,171,148) / 255.0f));
	// model.addPart(new Part("upperLeftLeg", glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(3,76,153) / 255.0f));
	// model.addPart(new Part("upperRightLeg", glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(3,76,153) / 255.0f));
	// model.setChild("upperLeftLeg", new Part("lowerLeftLeg", glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(3,76,153) / 255.0f));
	// model.setChild("upperRightLeg", new Part("lowerRightLeg", glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(3,76,153) / 255.0f));

	// model.setChild("torso", new Part("head", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(224,171,148) / 255.0f));
	// model.setChild("torso", "upperLeftArm");
	// model.setChild("torso", "upperRightArm");
	// model.setChild("torso", "upperLeftLeg");
	// model.setChild("torso", "upperRightLeg");

	// model.setRoot("torso");

	animationId = "walking";

	try {
		anims.load(animationId, "assets/test.hgl");
		anims.play(animationId);
	} catch (const std::exception &e) {
		Engine::log(e.what());
	}

	// body.exportAnimation("assets/test.hgl");
}

TitleScene::~TitleScene()
{
}
