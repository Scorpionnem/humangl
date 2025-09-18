/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timeline.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:41:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/18 17:16:16 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMELINE_HPP
# define TIMELINE_HPP

# include "libs.hpp"
# include "Engine.hpp"
# include "KeyFrame.hpp"

enum class KeyFrameType
{
	TRANSLATION,
	ROTATION,
	SCALE
};

class	Timeline
{
	public:
		Timeline() {}
		~Timeline() {}
		Timeline(const Timeline &copy);
		Timeline	&operator=(const Timeline &copy);
		void	update(float deltaTime);
		void	addKeyFrame(KeyFrameType type, const KeyFrame<glm::vec3> &keyframe);
		glm::vec3	getValue(KeyFrameType type);
		void	exportTimeline(std::ofstream &file);
		std::vector<KeyFrame<glm::vec3>>	&getKeyFrames(KeyFrameType type);
		float		getBiggestTime();
	private:
		glm::vec3	_getValue(std::vector<KeyFrame<glm::vec3>> &keyframes);
		void	_sort(std::vector<KeyFrame<glm::vec3>> &keyframes);
		std::vector<KeyFrame<glm::vec3>>	_translationKeyframes;
		std::vector<KeyFrame<glm::vec3>>	_rotationKeyframes;
		std::vector<KeyFrame<glm::vec3>>	_scaleKeyframes;
		float					_time = 0;
		bool					_loop = true;
};

#endif
