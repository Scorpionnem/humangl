/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnimationManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:45:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/13 16:01:01 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATIONMANAGER_HPP
# define ANIMATIONMANAGER_HPP

# include "libs.hpp"
# include "Timeline.hpp"

/*

	Stores timelines, key of the map is the id of the body part

*/
class	Animation
{
	public:
		Animation() {}
		Animation(const std::string &path)
		{
			std::ifstream	file;
			std::string		fileLine;

			file.open(path);
			if (!file.is_open())
				throw std::runtime_error("Could not open file");

			while (std::getline(file, fileLine))
			{
				std::istringstream	line(fileLine);
				std::string			word;

				line >> word;
				if (word == "define")
				{
					std::cout << "define" << std::endl;
				}
			}
		}
		~Animation() {}

		Timeline	&get(const std::string &id)
		{
			return (_timelines[id]);
		}
		Timeline	&operator[](const std::string &name)
		{
			return (this->get(name));
		}
	private:
		std::map<std::string, Timeline>	_timelines;
};

/*

	Stores Animations, key of the map is the name of the file

*/
class	AnimationManager
{
	public:
		AnimationManager() {}
		~AnimationManager() {}

		void	load(const std::string &id, const std::string &path)
		{
			_animations.insert({id, Animation(path)});
		}
		Animation	&get(const std::string &id)
		{
			return (_animations[id]);
		}
		Animation	&operator[](const std::string &name)
		{
			return (this->get(name));
		}
	private:
		std::map<std::string, Animation>	_animations;
};

#endif
