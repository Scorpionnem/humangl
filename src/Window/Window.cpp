/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:11:45 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 11:03:30 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Window.hpp"
#include "Camera.hpp"
#include "FrameBuffer.hpp"
#include "Engine.hpp"

void	Window::close()
{
	if (!this->up())
		return ;
	glfwSetWindowShouldClose(this->data(), true);
	Engine::log("Closing window.", LogSeverity::WARNING);
}

static void resize_hook(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
	Engine::Window->setWidth(width);
	Engine::Window->setHeight(height);
	Engine::FrameBuffer->resize(width, height);
}

Window::Window() : _lastFrame(0)
{
	Engine::log("Creating window...", LogSeverity::NORMAL);
	//Inits GLFW settings
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize glfw");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->_width = DEFAULT_WINDOW_WIDTH;
	this->_height = DEFAULT_WINDOW_HEIGHT;

	// Creates and opens window
	GLFWmonitor	*monitor = NULL;
	#if FULL_SCREEN
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode	*monitorInfos = glfwGetVideoMode(monitor);
		this->_height = monitorInfos->height;
		this->_width = monitorInfos->width;
	#endif
	_windowData = glfwCreateWindow(this->_width, this->_height, WIN_NAME, monitor, NULL);
	if (!_windowData)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}
	glfwMakeContextCurrent(_windowData);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("Failed to init GLAD");
	}
	glViewport(0, 0, this->_width, this->_height);

	glfwSetFramebufferSizeCallback(_windowData, resize_hook);
	glfwSetKeyCallback(_windowData, Engine::keyboardKeyHook);
	glfwSetCharCallback(_windowData, Engine::keyboardCharHook);
	glfwSetInputMode(this->data(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPosCallback(this->data(), Engine::moveMouseHook);
	glfwSetMouseButtonCallback(this->data(), Engine::clickMouseHook);
	glfwSetScrollCallback(this->data(), Engine::scrollMouseHook);

	glClearColor(0.6, 0.8, 1.0, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->center();
	this->setIcon(MBATTY_TEXTURE_PATH);
	Engine::log("Creating window, done.", LogSeverity::SUCCESS);
}

Window::~Window()
{
	glfwDestroyWindow(this->_windowData);
	glfwTerminate();
}

GLFWwindow	*Window::data(void)
{
	return (this->_windowData);
}

void		Window::loopStart(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_currentFrame = glfwGetTime();
	_deltaTime = _currentFrame - _lastFrame;
}

void		Window::loopEnd()
{
	glfwSwapBuffers(_windowData);
	glfwPollEvents();

	_lastFrame = _currentFrame;
}

bool	Window::up(void)
{
	return (!glfwWindowShouldClose(_windowData));
}

void		Window::setIcon(const char *path)
{
	Texture	tex(path);
	GLFWimage	image[1];
	image[0].pixels = tex.getData().data();
	image[0].width = tex.getWidth();
	image[0].height = tex.getHeight();
	glfwSetWindowIcon(_windowData, 1, image);
}

void		Window::center()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwSetWindowPos(_windowData, (mode->width / 2) - (this->_width / 2), (mode->height / 2) - (this->_height / 2));
}

void	Window::setDefaultMousePos()
{
	double	x,y;
	glfwSetInputMode(this->_windowData, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(this->_windowData, &x, &y);
	lastMouseX = x;
	lastMouseY = y;
}

float		Window::getDeltaTime(void)
{
	return (this->_deltaTime);
}
