/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnimationManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:45:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/24 10:42:17 by mbatty           ###   ########.fr       */
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
		void	load(const std::string &path, Model *model)
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
					if (!(line >> object))
						throw std::runtime_error("define: No argument given");
					std::cout << "define " << object;// << std::endl;

					_timelines.insert({object, new Timeline()});

					model->addPart(new Part(object));
				}
				else if (word == "object")
				{
					std::string	object;
					if (!(line >> object))
						throw std::runtime_error("object: No argument given");
					std::cout << "object " << object << std::endl;

					current = object;
				}
				else if (word == "root")
				{
					std::string	object;
					if (!(line >> object))
						throw std::runtime_error("root: No argument given");
					std::cout << "root " << object << std::endl;

					if (!_rooted)
						model->setRoot(object);
					_rooted = true;
				}
				else if (word == "children")
				{
					std::string	object;
					if (!(line >> object))
						throw std::runtime_error("children: No argument given");
					std::cout << "children " << object << std::endl;

					if (current.empty())
						throw std::runtime_error("children: Object not set");
					model->getPart(current)->addChild(model->getPart(object));
				}
				else if (word == "kft")
				{
					_addKeyFrame(KeyFrameType::TRANSLATION, current, line);
				}
				else if (word == "kfr")
				{
					_addKeyFrame(KeyFrameType::ROTATION, current, line);
				}
				else if (word == "kfs")
				{
					_addKeyFrame(KeyFrameType::SCALE, current, line);
				}
				else if (word == "color")
				{
					float	x;
					float	y;
					float	z;

					if (!(line >> x >> y >> z))
						throw std::runtime_error("color: Failed to parse");
					model->getPart(current)->setColor(glm::vec3(x, y, z) / 255.f);
				}
				else if (word == "banchor")
				{
					float	x;
					float	y;
					float	z;

					if (!(line >> x >> y >> z))
						throw std::runtime_error("banchor: Failed to parse");
					model->getPart(current)->setBaseAnchor(glm::vec3(x, y, z));
				}
				else if (word == "panchor")
				{
					float	x;
					float	y;
					float	z;

					if (!(line >> x >> y >> z))
						throw std::runtime_error("panchor: Failed to parse");
					model->getPart(current)->setPointAnchor(glm::vec3(x, y, z));
				}
			}
		}
		~Animation() {}

		Timeline	*get(const std::string &id)
		{
			if (_timelines.find(id) == _timelines.end())
				return (NULL);
			return (_timelines[id]);
		}
		void	addKeyFrame(std::string part, KeyFrameType type, KeyFrame<glm::vec3> keyframe)
		{
			Timeline	*tml = get(part);
			if (!tml)
				return ;
			tml->addKeyFrame(type, keyframe);
		}
	private:
		void	_addKeyFrame(KeyFrameType type, const std::string &id, std::istringstream &line)
		{
			float	time;
			float	x;
			float	y;
			float	z;

			if (!(line >> time >> x >> y >> z))
				throw std::runtime_error("kf*: Failed to parse");
			std::cout << id << " " << (int)type << " " << x << " " << y << " " << z << std::endl;
			if (id.empty())
				throw std::runtime_error("no defined object");
			_timelines[id]->addKeyFrame(type, KeyFrame(time, glm::vec3(x, y, z), type));
		}
		std::map<std::string, Timeline*>	_timelines;
		bool								_rooted = false;
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
			_animations.insert({id, {new Animation(), new Model()}});
			_animations[id].first->load(path, _animations[id].second);
		}
		//Plays the animation to the model
		void	play(const std::string &id)
		{
			std::map<std::string, Part*>	&parts = _animations[id].second->getParts();
			_current = _animations[id].first;

			for (auto part : parts)
			{
				part.second->setTimeline(_current->get(part.second->id()));
				std::cout << part.second->id() << " changed timeline" << std::endl;
			}
			std::cout << "Now playing " << id << std::endl;
		}

		Model	*getAnimationModel(const std::string &id)
		{
			return (_animations[id].second);
		}
		Animation	*getAnimation(const std::string &id)
		{
			return (_animations[id].first);
		}
	private:
		std::map<std::string, std::pair<Animation*, Model*> >	_animations;
		Animation												*_current = nullptr;
};

#endif
