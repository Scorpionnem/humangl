/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timeline.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:47:06 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 18:09:40 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Timeline.hpp"

glm::vec3	Timeline::getTranslation()
{
	if (!_keyframes.size())
		return (glm::vec3(0));
	if (_time <= _keyframes.front().getTime())
		return (_keyframes.front().getTranslation());
	if (_time >= _keyframes.back().getTime())
		return (_keyframes.back().getTranslation());

	for (size_t i = 0; i < _keyframes.size() - 1; i++)
	{
		const KeyFrame& p1 = _keyframes[i];
		const KeyFrame& p2 = _keyframes[i + 1];
	
		if (_time >= p1.getTime() && _time <= p2.getTime())
		{
			float t = (_time - p1.getTime()) / (p2.getTime() - p1.getTime());
			return (glm::mix(p1.getTranslation(), p2.getTranslation(), t));
		}
	}
	return (glm::vec3(0.0f));
}

glm::vec3	Timeline::getRotation()
{
	if (!_keyframes.size())
		return (glm::vec3(0));
	if (_time <= _keyframes.front().getTime())
		return (_keyframes.front().getRotation());
	if (_time >= _keyframes.back().getTime())
		return (_keyframes.back().getRotation());

	for (size_t i = 0; i < _keyframes.size() - 1; i++)
	{
		const KeyFrame& p1 = _keyframes[i];
		const KeyFrame& p2 = _keyframes[i + 1];
	
		if (_time >= p1.getTime() && _time <= p2.getTime())
		{
			float t = (_time - p1.getTime()) / (p2.getTime() - p1.getTime());
			return (glm::mix(p1.getRotation(), p2.getRotation(), t));
		}
	}
	return (glm::vec3(0.0f));
}

glm::vec3	Timeline::getScale()
{
	if (!_keyframes.size())
		return (glm::vec3(0));
	if (_time <= _keyframes.front().getTime())
		return (_keyframes.front().getScale());
	if (_time >= _keyframes.back().getTime())
		return (_keyframes.back().getScale());

	for (size_t i = 0; i < _keyframes.size() - 1; i++)
	{
		const KeyFrame& p1 = _keyframes[i];
		const KeyFrame& p2 = _keyframes[i + 1];
	
		if (_time >= p1.getTime() && _time <= p2.getTime())
		{
			float t = (_time - p1.getTime()) / (p2.getTime() - p1.getTime());
			return (glm::mix(p1.getScale(), p2.getScale(), t));
		}
	}
	return (glm::vec3(0.0f));
}
