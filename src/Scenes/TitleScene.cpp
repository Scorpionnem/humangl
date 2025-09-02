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

Timeline	testtimeline;
Timeline	testtimeline2;
Timeline	testtimeline3;

void	_draw2D(TitleScene *scene)
{
    glDisable(GL_DEPTH_TEST);

	FrameBuffer::drawFrame(Engine::Shaders->get("title_bg"), Engine::Textures->get(SAND_TEXTURE_PATH)->getID());
	scene->getInterfaceManager()->draw();

	if (scene->getDebug())
		scene->getInterfaceManager()->get("debug")->draw();
	
	Shader	*shader = Engine::Shaders->get("colored_quad");

	shader->setVec3("color", glm::vec3(0, 1, 0));
		UIElement::draw(shader, glm::vec2(testtimeline.getTranslation()), glm::vec2(50, 50));
	shader->setVec3("color", glm::vec3(1, 0, 0));
		UIElement::draw(shader, glm::vec2(testtimeline.getTranslation() + testtimeline2.getTranslation()), glm::vec2(50, 50));

	shader->setVec3("color", glm::vec3(0, 0, 1));
	UIElement::draw(shader, glm::vec2(testtimeline.getTranslation() + testtimeline2.getTranslation() + testtimeline3.getTranslation()), glm::vec2(50, 50));

	testtimeline.draw(Engine::Window->getHeight() - 16);
	testtimeline2.draw(Engine::Window->getHeight() - 32);

	glEnable(GL_DEPTH_TEST);
}

static void	_render(Scene *ptr)
{
	TitleScene	*scene = static_cast<TitleScene*>(ptr);

	// _draw2D(scene);

	Shader	*shader2 = Engine::Shaders->get("cube");

	shader2->use();
	camera.setViewMatrix(*shader2);

	scene->torso.draw();

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
	scene->torso.update(glm::mat4(1.0));

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

TitleScene::TitleScene()
{
	camera.pos = glm::vec3(-10, -10, 10);

	_buildInterface(this);
	this->setKeyHook(_keyHookFunc);
	this->setCharHook(_charHookFunc);

	this->setRender(_render);
	this->setUpdate(_update);
	this->setOpen(_open);
	this->setClose(_close);

	this->setMoveMouseHook(_moveMouseHookFunc);

	testtimeline.addKeyFrame(KeyFrame(0 * 6.0f, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}));
	// testtimeline.addKeyFrame(KeyFrame(0.8 * 3.0f, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(0.8 * 6.0f, {0, 0, 0}, {0, 0, 0}, {10, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(1.6 * 6.0f, {0, 0, 0}, {360, 0, 0}, {1, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(2.4 * 6.0f, {0, 0, 0}, {360, 0, 0}, {1, 1, 16}));
	testtimeline.addKeyFrame(KeyFrame(3.2 * 6.0f, {0, 0, 0}, {360, 0, 360}, {1, 1, 1}));
	testtimeline.addKeyFrame(KeyFrame(4.0 * 6.0f, {0, 0, 0}, {360, 0, 360}, {1, 16, 1}));
	testtimeline.addKeyFrame(KeyFrame(4.8 * 6.0f, {0, 0, 0}, {360, 360, 360}, {1, 1, 1}));

	torso = Part(testtimeline, glm::vec3(0.0f), glm::vec3(0.2f, 0.8f, 0.2f));

	Part	leftLeg = Part(testtimeline, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.8f));
	leftLeg.addChild(Part(testtimeline, glm::vec3(1.0f, 0.0f, 0.5f), glm::vec3(0.8f, 0.2f, 0.2f)));
	// Part	rightLeg = Part(testtimeline, glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.2f, 0.2f, 0.8f));
	// rightLeg.addChild(Part(testtimeline, glm::vec3(0.0f), glm::vec3(0.2f, 0.2f, 0.8f)));

	// Part	leftLeg = Part(testtimeline, glm::vec3(0.0f, 0.0f, -0.5f), glm::vec3(0.2f, 0.2f, 0.8f));
	// leftLeg.addChild(Part(testtimeline, glm::vec3(0.0f), glm::vec3(0.2f, 0.2f, 0.8f)));
	// Part	rightLeg = Part(testtimeline, glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.2f, 0.2f, 0.8f));
	// rightLeg.addChild(Part(testtimeline, glm::vec3(0.0f), glm::vec3(0.2f, 0.2f, 0.8f)));

	torso.addChild(leftLeg);

	// std::vector<float> cube_vertices = {
    // // front face
	// 	-0.5f, -0.5f,  0.5f,
	// 	0.5f,  0.5f,  0.5f,
	// 	0.5f, -0.5f,  0.5f,

	// 	0.5f,  0.5f,  0.5f,
	// 	-0.5f, -0.5f,  0.5f,
	// 	-0.5f,  0.5f,  0.5f,

	// 	// back face
	// 	-0.5f, -0.5f, -0.5f,
	// 	0.5f, -0.5f, -0.5f,
	// 	0.5f,  0.5f, -0.5f,

	// 	0.5f,  0.5f, -0.5f,
	// 	-0.5f,  0.5f, -0.5f,
	// 	-0.5f, -0.5f, -0.5f,

	// 	// left face
	// 	-0.5f,  0.5f,  0.5f,
	// 	-0.5f, -0.5f, -0.5f,
	// 	-0.5f,  0.5f, -0.5f,

	// 	-0.5f, -0.5f, -0.5f,
	// 	-0.5f,  0.5f,  0.5f,
	// 	-0.5f, -0.5f,  0.5f,

	// 	// right face
	// 	0.5f,  0.5f,  0.5f,
	// 	0.5f,  0.5f, -0.5f,
	// 	0.5f, -0.5f, -0.5f,

	// 	0.5f, -0.5f, -0.5f,
	// 	0.5f, -0.5f,  0.5f,
	// 	0.5f,  0.5f,  0.5f,

	// 	// bottom face
	// 	-0.5f, -0.5f, -0.5f,
	// 	0.5f, -0.5f,  0.5f,
	// 	0.5f, -0.5f, -0.5f,

	// 	0.5f, -0.5f,  0.5f,
	// 	-0.5f, -0.5f, -0.5f,
	// 	-0.5f, -0.5f,  0.5f,

	// 	// top face
	// 	-0.5f,  0.5f, -0.5f,
	// 	0.5f,  0.5f, -0.5f,
	// 	0.5f,  0.5f,  0.5f,

	// 	0.5f,  0.5f,  0.5f,
	// 	-0.5f,  0.5f,  0.5f,
	// 	-0.5f,  0.5f, -0.5f
	// };


	// 	glGenVertexArrays(1, &VAO);
	// 	glGenBuffers(1, &VBO);
	// 	glBindVertexArray(VAO);
	
	// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 	glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(float), cube_vertices.data(), GL_STATIC_DRAW);
	
	// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// 	glEnableVertexAttribArray(0);
	
	// 	glBindVertexArray(0);
}

TitleScene::~TitleScene()
{
}
