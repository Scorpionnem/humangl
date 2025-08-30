/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timeline.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:41:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 13:25:05 by mbatty           ###   ########.fr       */
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
		glm::vec3	getTranslation();
		glm::vec3	getRotation();
		glm::vec3	getScale();
		void	update(float deltaTime)
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
		void	draw()
		{
			Shader	*shader = Engine::Shaders->get("colored_quad");

			float	maxTime = 10;
			float	keyFrameSize = 15;

			for (KeyFrame &keyframe : _keyframes)
			{
				float	posX = Engine::Window->getWidth() * (keyframe.getTime() / maxTime) - (keyFrameSize / 2);
				
				shader->setVec3("color", glm::vec3(0, 0.5, 0.7));
					UIElement::draw(shader, glm::vec2(posX, 0), glm::vec2(16, 16));
			}
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
