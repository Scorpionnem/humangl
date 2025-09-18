/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnimationManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:45:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/18 14:17:07 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATIONMANAGER_HPP
# define ANIMATIONMANAGER_HPP

# include "libs.hpp"
# include "Timeline.hpp"
# include "Model.hpp"

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

			std::cout << "Loading " << path << std::endl;

			std::string			current;
			while (std::getline(file, fileLine))
			{
				std::istringstream	line(fileLine);
				std::string			word;

				line >> word;
				if (word == "define")
				{
					std::string	object;
					std::string	path;
					line >> object;
					std::cout << "define " << object << std::endl;

					_timelines.insert({object, new Timeline()});
				}
				if (word == "object")
				{
					std::string	object;
					line >> object;
					std::cout << "object " << object << std::endl;

					current = object;
				}
				if (word == "children")
				{
					std::string	object;
					line >> object;
					std::cout << "children " << object << std::endl;
					
				}
				if (word == "kft")
				{
					_addKeyFrame(KeyFrameType::TRANSLATION, current, line);
				}
				if (word == "kfr")
				{
					_addKeyFrame(KeyFrameType::ROTATION, current, line);
				}
				if (word == "kfs")
				{
					_addKeyFrame(KeyFrameType::SCALE, current, line);
				}
			}
		}
		~Animation() {}

		Timeline	*get(const std::string &id)
		{
			return (_timelines[id]);
		}
		std::map<std::string, Timeline*>	&getTimelines()
		{
			return (this->_timelines);
		}
	private:
		void	_addKeyFrame(KeyFrameType type, const std::string &id, std::istringstream &line)
		{
			float	time;
			float	x;
			float	y;
			float	z;

			line >> time >> x >> y >> z;
			std::cout << id << " " << (int)type << " " << x << " " << y << " " << z << std::endl;
			if (id.empty())
				throw std::runtime_error("no defined object");
			_timelines[id]->addKeyFrame(type, KeyFrame(time, glm::vec3(x, y, z)));
		}
		std::map<std::string, Timeline*>	_timelines;
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
			_animations.insert({id, new Animation(path)});
		}
		//Plays the animation to the model
		void	play(const std::string &id, Model &model)
		{
			std::map<std::string, Part*>	&parts = model.getParts();
			_current = _animations[id];

			for (auto part : parts)
			{
				part.second->setTimeline(_current->get(part.second->id()));
				std::cout << part.second->id() << " changed timeline" << std::endl;
			}
		}
		Animation	*getCurrent()
		{
			return (_current);
		}
	private:
		Animation							*_current = NULL;
		std::map<std::string, Animation*>	_animations;
};

#endif
