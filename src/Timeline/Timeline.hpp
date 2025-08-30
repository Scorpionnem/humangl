/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timeline.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:41:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 18:04:42 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMELINE_HPP
# define TIMELINE_HPP

# include "libs.hpp"
# include "Engine.hpp"
# include "KeyFrame.hpp"

class	Timeline
{
	public:
		Timeline() {}
		~Timeline() {}
		Timeline(const Timeline &copy)
		{
			*this = copy;
		}
		Timeline	&operator=(const Timeline &copy)
		{
			if (this != &copy)
			{
				this->_keyframes = copy._keyframes;
				this->_time = copy._time;
				this->_loop = copy._loop;
			}
			return (*this);
		}
		glm::vec3	getTranslation();
		glm::vec3	getRotation();
		glm::vec3	getScale();
		void	update(float deltaTime)
		{
			_time += deltaTime;
			if (_loop && _keyframes.size() && _time >= _keyframes.back().getTime())
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
		void	draw(float y)
		{
			Shader	*shader = Engine::Shaders->get("colored_quad");

			float	maxTime = 10;
			float	keyFrameSize = 16;
			float	pointerSize = 14;
			float	posX = 0;

			for (KeyFrame &keyframe : _keyframes)
			{
				posX = Engine::Window->getWidth() * (keyframe.getTime() / maxTime) - (keyFrameSize / 2);
				
				shader->setVec3("color", glm::vec3(0, 0.5, 0.7));
					UIElement::draw(shader, glm::vec2(posX, y), glm::vec2(keyFrameSize, keyFrameSize));
			}
			posX = Engine::Window->getWidth() * (_time / maxTime) - (pointerSize / 2);
			shader->setVec3("color", glm::vec3(0.7, 0.0, 0.5));
			UIElement::draw(shader, glm::vec2(posX, y), glm::vec2(pointerSize, pointerSize));
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
