/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:31:40 by mbirou            #+#    #+#             */
/*   Updated: 2025/09/01 21:09:56 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PART_HPP
# define PART_HPP

#include <libs.hpp>
#include "Engine.hpp"
#include <Timeline.hpp>
#include <vector>

class Part
{
	public:
		Part(const Timeline &timeLine, const glm::vec3 &anchor);
		Part();
		~Part();
	
		void	addChild(const Part &child);

		void	updateAnchor(const glm::mat4 &parentMat);
		void	update(const glm::mat4 &parentMat);
		void	draw();

		static std::vector<float> defaultCube;
	
	private:
		glm::mat4			_mat;
		glm::vec3			_anchor = glm::vec3(0.0f);
		std::vector<Part>	_children;
		Timeline			_timeLine;
		uint				VAO = 0;
		uint				VBO = 0;
};

#endif
