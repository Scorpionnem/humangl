/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SceneManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:00:41 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 12:01:34 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SceneManager.hpp"
#include "Engine.hpp"

bool	SceneManager::erase(const std::string &name)
{
	std::unordered_map<std::string, Scene *>::iterator	finder;
	finder = _scenes.find(name);
	if (finder == _scenes.end())
	{
		Engine::log("WARNING Tried to unload a scene thats not loaded: " + name, LogSeverity::WARNING);
		return (0);
	}
	_scenes.erase(finder);
	return (1);
}

Scene	*SceneManager::load(std::string name, std::function<void(Scene *)> build, std::function<void(Scene *)> destructor, std::function<void(Scene*)> onRender, std::function<void(Scene*)> onUpdate)
{
	if (_scenes.find(name) != _scenes.end())
	{
		Engine::log("WARNING Tried to load a scene thats already loaded (will be using the existing scene): " + name, LogSeverity::WARNING);
		return (this->get(name));
	}
	return (_scenes.insert(std::make_pair(name, new Scene(build, destructor, onRender, onUpdate))).first->second);
}

Scene	*SceneManager::get(const std::string &name)
{
	std::unordered_map<std::string, Scene *>::iterator	finder = _scenes.find(name);
	if (finder == _scenes.end())
	{
		Engine::log("ERROR Tried to access a scene thats not loaded, might cause a crash: " + name, LogSeverity::ERROR);
		return (NULL);
	}
	return (finder->second);
}
