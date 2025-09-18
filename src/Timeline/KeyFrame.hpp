/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KeyFrame.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:25:08 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/18 17:12:13 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYFRAME_HPP
# define KEYFRAME_HPP

#include "libs.hpp"

template	<typename T>
class	KeyFrame
{
	public:
		KeyFrame()
		{
			_time = 0;
		}
		KeyFrame(float time, T value) : KeyFrame()
		{
			this->_time = time;
			_value = value;
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
		T	getValue() const
		{
			return (_value);
		}
	private:
		float		_time;
		T			_value;
};

#endif
