/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:57:53 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 21:59:16 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Interface.hpp"
#include "Engine.hpp"

UIElement	*Interface::addElement(const std::string &key, UIElement *elem)
{
	std::map<std::string, UIElement *>::iterator	finder;

	finder = _elements.find(key);
	if (finder != _elements.end())
	{
		Engine::log("ERROR UIElement already exists in this interface", LogSeverity::ERROR);
		return (finder->second);
	}
	elem->setID(key);
	_elements.insert({key, elem});
	return (elem);
}

UIElement	*Interface::getElement(const std::string &key)
{
	std::map<std::string, UIElement *>::iterator	finder;

	finder = _elements.find(key);
	if (finder == _elements.end())
	{
		Engine::log("WARNING UIElement does not exist in this interface", LogSeverity::WARNING);
		return (NULL);
	}
	return (finder->second);
}

void	Interface::update()
{
	if (_onUpdate)
		_onUpdate(this);

	double mouseX, mouseY;
	bool mousePressed = glfwGetMouseButton(Engine::Window->getWindowData(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	glfwGetCursorPos(Engine::Window->getWindowData(), &mouseX, &mouseY);
	for (auto &element : _elements)
		element.second->update(glm::vec2(mouseX, mouseY), mousePressed);
}
