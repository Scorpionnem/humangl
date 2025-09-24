/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:31:40 by mbirou            #+#    #+#             */
/*   Updated: 2025/09/24 10:06:42 by mbatty           ###   ########.fr       */
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
		Part(std::string id);
		Part();
		~Part();
		Part(const Part &copy)
		{
			*this = copy;
		}
		Part	&operator=(const Part &copy)
		{
			if (this != &copy)
			{
				this->_id = copy._id;
				this->_mat = copy._mat;
				this->_pointAnchor = copy._pointAnchor;
				this->_baseAnchor = copy._baseAnchor;
				this->_children = copy._children;
				this->_timeline = copy._timeline;
				this->VAO = copy.VAO;
				this->VBO = copy.VBO;
				this->_color = copy._color;
			}
			return (*this);
		}
	
		void	addChild(Part *child);
		void	exportAnimation(std::string path, Part *root);

		void	setTimeline(Timeline *cpy)
		{
			_timeline = cpy;
		}
		void	updateAnchor(const glm::mat4 &parentMat);
		void	update(const glm::mat4 &parentMat);
		void	draw();
		void	selectDraw();

		void	setPointAnchor(const glm::vec3 &val)
		{
			this->_pointAnchor = val;
		}
		void	setBaseAnchor(const glm::vec3 &val)
		{
			this->_baseAnchor = val;
		}
		void	setColor(const glm::vec3 &val)
		{
			this->_color = val;
		}
		glm::vec3	getAnchor() const;
		glm::vec3	getIdColor() const;
		std::string	id()
		{
			return (this->_id);
		}

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
		std::vector<Part*>	_children;
		Timeline			*_timeline = NULL;
		uint				VAO = 0;
		uint				VBO = 0;
		glm::vec3			_color = glm::vec3(0.0f);
		glm::vec3			_idColor = glm::vec3(0.0f);
		static unsigned int	_index;
};

#endif
