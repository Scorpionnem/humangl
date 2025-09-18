/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timeline.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:47:06 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/18 17:15:22 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Timeline.hpp"

Timeline::Timeline(const Timeline &copy)
{
	*this = copy;
}

Timeline	&Timeline::operator=(const Timeline &copy)
{
	if (this != &copy)
	{
		this->_translationKeyframes = copy._translationKeyframes;
		this->_rotationKeyframes = copy._rotationKeyframes;
		this->_scaleKeyframes = copy._scaleKeyframes;
		this->_time = copy._time;
		this->_loop = copy._loop;
	}
	return (*this);
}

void	Timeline::update(float deltaTime)
{
	_time += deltaTime;
	if (_loop && _time > getBiggestTime())
		_time = 0;
}

void	Timeline::addKeyFrame(KeyFrameType type, const KeyFrame<glm::vec3> &keyframe)
{
	switch (type)
	{
		case KeyFrameType::TRANSLATION:
			_translationKeyframes.push_back(keyframe);
			_sort(_translationKeyframes);
			break ;
		case KeyFrameType::ROTATION:
			_rotationKeyframes.push_back(keyframe);
			_sort(_rotationKeyframes);
			break ;
		case KeyFrameType::SCALE:
			_scaleKeyframes.push_back(keyframe);
			_sort(_scaleKeyframes);
			break ;
	}
}

glm::vec3	Timeline::getValue(KeyFrameType type)
{
	switch (type)
	{
		case KeyFrameType::TRANSLATION:
			return (_getValue(_translationKeyframes));
		case KeyFrameType::ROTATION:
			return (_getValue(_rotationKeyframes));
		case KeyFrameType::SCALE:
			return (_getValue(_scaleKeyframes));
	}
}

void	Timeline::exportTimeline(std::ofstream &file)
{
	for (auto &keyframe : _translationKeyframes)
		file << "kft " << keyframe.getTime() << " " << keyframe.getValue().x << " " << keyframe.getValue().y << " " << keyframe.getValue().z << std::endl;
	for (auto &keyframe : _rotationKeyframes)
		file << "kfr " << keyframe.getTime() << " " << keyframe.getValue().x << " " << keyframe.getValue().y << " " << keyframe.getValue().z << std::endl;
	for (auto &keyframe : _scaleKeyframes)
		file << "kfs " << keyframe.getTime() << " " << keyframe.getValue().x << " " << keyframe.getValue().y << " " << keyframe.getValue().z << std::endl;
	(void)file;
}

std::vector<KeyFrame<glm::vec3>>	&Timeline::getKeyFrames(KeyFrameType type)
{
	switch (type)
	{
		case KeyFrameType::TRANSLATION:
			return (_translationKeyframes);
		case KeyFrameType::ROTATION:
			return (_rotationKeyframes);
		case KeyFrameType::SCALE:
			return (_scaleKeyframes);
	}
}

float	Timeline::getBiggestTime()
{
	float res = 0;
	float	translationMax = _translationKeyframes.size() ? _translationKeyframes.back().getTime() : 0;
	float	rotationMax = _rotationKeyframes.size() ? _rotationKeyframes.back().getTime() : 0;
	float	scaleMax = _scaleKeyframes.size() ? _scaleKeyframes.back().getTime() : 0;
	
	res = std::max(translationMax, std::max(rotationMax, scaleMax));
	return (res);
}

glm::vec3	Timeline::_getValue(std::vector<KeyFrame<glm::vec3>> &keyframes)
{
	if (!keyframes.size())
		return (glm::vec3(0));
	if (_time <= keyframes.front().getTime())
		return (keyframes.front().getValue());
	if (_time >= keyframes.back().getTime())
		return (keyframes.back().getValue());

	for (size_t i = 0; i < keyframes.size() - 1; i++)
	{
		const KeyFrame<glm::vec3>& p1 = keyframes[i];
		const KeyFrame<glm::vec3>& p2 = keyframes[i + 1];
	
		if (_time >= p1.getTime() && _time <= p2.getTime())
		{
			float t = (_time - p1.getTime()) / (p2.getTime() - p1.getTime());
			return (glm::mix(p1.getValue(), p2.getValue(), t));
		}
	}
	return (glm::vec3(0.0f));
}

void	Timeline::_sort(std::vector<KeyFrame<glm::vec3>> &keyframes)
{
	std::sort(keyframes.begin(), keyframes.end(),
		[](const KeyFrame<glm::vec3> &cp1, const KeyFrame<glm::vec3> &cp2)
		{
			return (cp1.getTime() < cp2.getTime());
		});
}
