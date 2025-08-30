/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeyFrame.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:25:08 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 13:25:27 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYFRAME_HPP
# define KEYFRAME_HPP

#include "libs.hpp"

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

#endif
