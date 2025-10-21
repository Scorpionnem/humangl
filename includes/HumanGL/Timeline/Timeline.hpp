/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timeline.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:41:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/19 10:26:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMELINE_HPP
# define TIMELINE_HPP

# include "libs.hpp"
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
				this->_translationKeyframes = copy._translationKeyframes;
				this->_rotationKeyframes = copy._rotationKeyframes;
				this->_scaleKeyframes = copy._scaleKeyframes;
				this->_time = copy._time;
				this->_loop = copy._loop;
			}
			return (*this);
		}
		void	update(float deltaTime)
		{
			_time += deltaTime;
			if (_loop && _time > getBiggestTime())
				_time = 0;
		}
		void	addKeyFrame(KeyFrameType type, const KeyFrame<glm::vec3> &keyframe)
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
		void	removeKeyFrame(KeyFrameType type, float time)
		{
			std::vector<KeyFrame<glm::vec3>>	keyframes;
			
			switch (type)
			{
				case KeyFrameType::ROTATION:
					return (_removeKeyFrame(time, _rotationKeyframes));
				case KeyFrameType::SCALE:
					return (_removeKeyFrame(time, _scaleKeyframes));
				case KeyFrameType::TRANSLATION:
					return (_removeKeyFrame(time, _translationKeyframes));
			}
		}
		void	_removeKeyFrame(float time, std::vector<KeyFrame<glm::vec3>> &keyframes)
		{
			for (auto it = keyframes.begin(); it != keyframes.end(); it++)
			{
				if (it->getTime() == time)
				{
					keyframes.erase(it);
					return ;
				}
			}
		}
		glm::vec3	getValue(KeyFrameType type)
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
		void	exportTimeline(std::ofstream &file)
		{
			for (auto &keyframe : _translationKeyframes)
				file << "kft " << keyframe.getTime() << " " << keyframe.getValue().x << " " << keyframe.getValue().y << " " << keyframe.getValue().z << std::endl;
			for (auto &keyframe : _rotationKeyframes)
				file << "kfr " << keyframe.getTime() << " " << keyframe.getValue().x << " " << keyframe.getValue().y << " " << keyframe.getValue().z << std::endl;
			for (auto &keyframe : _scaleKeyframes)
				file << "kfs " << keyframe.getTime() << " " << keyframe.getValue().x << " " << keyframe.getValue().y << " " << keyframe.getValue().z << std::endl;
			(void)file;
		}
		float		getBiggestTime()
		{
			_sort(_scaleKeyframes);
			_sort(_rotationKeyframes);
			_sort(_translationKeyframes);
			
			float res = 0;
			float	translationMax = _translationKeyframes.size() ? _translationKeyframes.back().getTime() : 0;
			float	rotationMax = _rotationKeyframes.size() ? _rotationKeyframes.back().getTime() : 0;
			float	scaleMax = _scaleKeyframes.size() ? _scaleKeyframes.back().getTime() : 0;
			
			res = std::max(translationMax, std::max(rotationMax, scaleMax));
			return (res);
		}
		float		getBiggestTime(KeyFrameType type)
		{
			std::vector<KeyFrame<glm::vec3>>	&keyframes = getKeyFrames(type);

			_sort(keyframes);

			return (keyframes.size() ? keyframes.back().getTime() : 0);
		}
		std::vector<KeyFrame<glm::vec3>>	&getKeyFrames(KeyFrameType type)
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
	private:
		glm::vec3	_getValue(std::vector<KeyFrame<glm::vec3>> &keyframes)
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
		void	_sort(std::vector<KeyFrame<glm::vec3>> &keyframes)
		{
			std::sort(keyframes.begin(), keyframes.end(),
				[](const KeyFrame<glm::vec3> &cp1, const KeyFrame<glm::vec3> &cp2)
				{
					return (cp1.getTime() < cp2.getTime());
				});
		}
		std::vector<KeyFrame<glm::vec3>>	_translationKeyframes;
		std::vector<KeyFrame<glm::vec3>>	_rotationKeyframes;
		std::vector<KeyFrame<glm::vec3>>	_scaleKeyframes;
		float					_time = 0;
		bool					_loop = true;
};

#endif
