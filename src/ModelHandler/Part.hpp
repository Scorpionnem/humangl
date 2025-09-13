/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:31:40 by mbirou            #+#    #+#             */
/*   Updated: 2025/09/13 15:45:22 by mbatty           ###   ########.fr       */
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
		Part(std::string id, const Timeline &timeLine, const glm::vec3 &pointAnchor, const glm::vec3 &baseAnchor, const glm::vec3 &color);
		Part();
		~Part();
	
		void	addChild(const Part &child);
		void	exportAnimation(std::string path);

		void	updateAnchor(const glm::mat4 &parentMat);
		void	update(const glm::mat4 &parentMat);
		void	draw();

		glm::vec3	getAnchor() const;

		static std::vector<float> defaultCube;
	
	private:
		void	_addHeader(std::ofstream &file);
		void	_exportObject(std::ofstream &file);
		void	_defineExport(std::ofstream &file);
		void	_childrenExport(std::ofstream &file);
		std::string			_id;
		glm::mat4			_mat;
		glm::vec3			_pointAnchor = glm::vec3(0.0f);
		glm::vec3			_baseAnchor = glm::vec3(0.0f);
		std::vector<Part>	_children;
		Timeline			_timeLine;
		uint				VAO = 0;
		uint				VBO = 0;
		glm::vec3			_color = glm::vec3(0.0f);
};

#endif
