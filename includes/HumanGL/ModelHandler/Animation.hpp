/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animation.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 09:22:28 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/19 10:27:18 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATION_HPP
# define ANIMATION_HPP

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
					std::cout << "define " << object;

					Timeline	*tml = new Timeline();
					_timelines.insert({object, tml});

					Part	*part = new Part(object);
					part->setTimeline(tml);
					model->addPart(part);
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
		~Animation()
		{
			for (auto &it : _timelines)
				delete it.second;
		}

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
		void	addTimeLine(const std::string &part, Timeline *tml)
		{
			_timelines.insert({part, tml});
		}
		void	update(float deltaTime)
		{
			for (auto &it : _timelines)
				it.second->update(deltaTime);
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

#endif
