/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterfaceManager.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:59:17 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 12:00:20 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InterfaceManager.hpp"
#include "Engine.hpp"

bool	InterfaceManager::erase(const std::string &name)
{
	std::unordered_map<std::string, Interface *>::iterator	finder;
	finder = _interfaces.find(name);
	if (finder == _interfaces.end())
	{
		Engine::log("WARNING Tried to unload an interface thats not loaded: " + name, LogSeverity::WARNING);
		return (0);
	}
	_interfaces.erase(finder);
	return (1);
}

Interface	*InterfaceManager::load(const std::string &name)
{
	if (_interfaces.find(name) != _interfaces.end())
	{
		Engine::log("WARNING Tried to load an interface thats already loaded (will be using the existing interface): " + name, LogSeverity::WARNING);
		return (this->get(name));
	}
	return (_interfaces.insert(std::make_pair(name, new Interface())).first->second);
}

Interface	*InterfaceManager::get(const std::string &name)
{
	std::unordered_map<std::string, Interface *>::iterator	finder = _interfaces.find(name);
	if (finder == _interfaces.end())
	{
		Engine::log("ERROR Tried to access an interface thats not loaded, might cause a crash: " + name, LogSeverity::ERROR);
		return (NULL);
	}
	return (finder->second);
}
