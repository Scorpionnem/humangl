/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterfaceManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:19:36 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 12:00:22 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACEMANAGER_HPP
# define INTERFACEMANAGER_HPP

# include "Interface.hpp"

class	InterfaceManager
{
	public:
		InterfaceManager()
		{

		}
		~InterfaceManager()
		{
			for (auto &pair : _interfaces)
				delete pair.second;
		}
		void	use(const std::string &name)
		{
			_current = get(name);
		}
		void	update()
		{
			if (_current)
				_current->update();
		}
		void	draw()
		{
			if (_current)
				_current->draw();
		}
		void	reset()
		{
			_current = NULL;
		}
		bool	erase(const std::string &name);
		Interface	*load(const std::string &name);
		Interface	*get(const std::string &name);
		Interface	*operator[](const std::string &name)
		{
			return (this->get(name));
		}
		Interface	*getCurrent() {return (this->_current);}
	private:
		std::unordered_map<std::string, Interface *>	_interfaces;
		Interface							*_current = NULL;
};

#endif
