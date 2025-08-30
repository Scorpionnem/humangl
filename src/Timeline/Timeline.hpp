/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timeline.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:41:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 12:54:59 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMELINE_HPP
# define TIMELINE_HPP

# include "libs.hpp"

class	KeyFrame
{
	public:
		KeyFrame()
		{
			_time = 0;
			_translation = glm::vec3(0);
			_rotation = glm::vec3(0);
			_scale = glm::vec3(0);
		}
		KeyFrame(float time, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) : KeyFrame()
		{
			this->_time = time;
			this->_translation = translation;
			this->_rotation = rotation;
			this->_scale = scale;
		}
		~KeyFrame()
		{
			
		}
		KeyFrame(const KeyFrame &copy)
		{
			*this = copy;
		}
		KeyFrame	&operator=(const KeyFrame &copy)
		{
			if (this != &copy)
			{
				this->_time = copy._time;
				this->_translation = copy._translation;
				this->_rotation = copy._rotation;
				this->_scale = copy._scale;
			}
			return (*this);
		}
		float	getTime() const
		{
			return (_time);
		}
		glm::vec3	getTranslation() const
		{
			return (_translation);
		}
		glm::vec3	getRotation() const
		{
			return (_rotation);
		}
		glm::vec3	getScale() const
		{
			return (_scale);
		}
	private:
		float		_time;
		glm::vec3		_translation;
		glm::vec3		_rotation;
		glm::vec3		_scale;
};

class	Timeline
{
	public:
		Timeline() {}
		~Timeline() {}
		glm::vec3	getTranslation();
		glm::vec3	getRotation();
		glm::vec3	getScale();
		void	step(float deltaTime)
		{
			_time += deltaTime;
			if (_loop && _time >= _keyframes.back().getTime())
				_time = 0;
		}
		void	addKeyFrame(const KeyFrame &keyframe)
		{
			_keyframes.push_back(keyframe);
			_sort();
		}
		void	addKeyFrames(const std::vector<KeyFrame> &keyframes)
		{
			_keyframes.insert(_keyframes.end(), keyframes.begin(), keyframes.end());
			_sort();
		}
	private:
		void	_sort()
		{
			std::sort(_keyframes.begin(), _keyframes.end(),
				[](const KeyFrame &cp1, const KeyFrame &cp2)
				{
					return (cp1.getTime() < cp2.getTime());
				});
		}
		std::vector<KeyFrame>	_keyframes;
		float					_time = 0;
		bool					_loop = true;
};

#endif
