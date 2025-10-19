/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeyFrame.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:25:08 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/23 12:00:58 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYFRAME_HPP
# define KEYFRAME_HPP

#include "libs.hpp"

enum class KeyFrameType
{
	TRANSLATION,
	ROTATION,
	SCALE
};

template	<typename T>
class	KeyFrame
{
	public:
		KeyFrame()
		{
			_time = 0;
		}
		KeyFrame(float time, T value, KeyFrameType type) : KeyFrame()
		{
			this->_time = time;
			_value = value;
			this->_type = type;
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
				this->_value = copy._value;
				this->_type = copy._type;
			}
			return (*this);
		}
		float	getTime() const
		{
			return (_time);
		}
		void	setTime(float val)
		{
			_time = val;
		}
		void	setValue(T val)
		{
			this->_value = val;
		}
		T	getValue() const
		{
			return (_value);
		}
		KeyFrameType	getType()
		{
			return (_type);
		}
	private:
		float			_time;
		T				_value;
		KeyFrameType	_type;
};

#endif
