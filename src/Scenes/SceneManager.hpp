/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SceneManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:05:13 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 11:24:14 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENEMANAGER_HPP
# define SCENEMANAGER_HPP

# include "Scene.hpp"

class	SceneManager
{
	public:
		SceneManager()
		{

		}
		~SceneManager()
		{
			for (auto &pair : _scenes)
				delete pair.second;
		}
		/*
			Swap is like use() but instead of switching scene instantly it switches on the next frame.
		*/
		void	swap(const std::string &name)
		{
			_swap = get(name);
		}
		void	use(const std::string &name)
		{
			if (_current)
				_current->close();

			_current = get(name);
			_current->open();
		}
		void	update()
		{
			if (_swap)
			{
				if (_current)
					_current->close();

				_current = _swap;
				_swap = NULL;
				_current->open();
			}
			if (_current)
				_current->update();
		}
		void	render()
		{
			if (_current)
				_current->render();
		}
		void	reset()
		{
			_current->close();
			_current = NULL;
		}
		bool	erase(const std::string &name);
		Scene	*load(std::string name, Scene *scene);
		Scene	*get(const std::string &name);
		Scene	*operator[](const std::string &name)
		{
			return (this->get(name));
		}
		Scene	*getCurrent() {return (this->_current);}
	private:
		std::unordered_map<std::string, Scene *>		_scenes;
		Scene								*_current = NULL;
		Scene								*_swap = NULL;
};

#endif
