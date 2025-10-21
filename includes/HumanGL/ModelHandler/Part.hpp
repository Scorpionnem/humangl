/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:31:40 by mbirou            #+#    #+#             */
/*   Updated: 2025/10/19 14:22:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PART_HPP
# define PART_HPP

#include "libs.hpp"
#include "Shader.hpp"
#include "Timeline.hpp"
#include <vector>

class Part
{
	public:
		Part(std::string id);
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
		void	update(const glm::mat4 &parentMat, float deltaTime);
		void	draw(Shader *shader);
		void	selectDraw(Shader *shader);

		void	setPointAnchor(const glm::vec3 &val)
		{
			this->_pointAnchor = val;
		}
		void	setPointAnchorx(float val) {this->_pointAnchor.x = val;}
		void	setPointAnchory(float val) {this->_pointAnchor.y = val;}
		void	setPointAnchorz(float val) {this->_pointAnchor.z = val;}
		void	setBaseAnchorx(float val) {this->_baseAnchor.x = val;}
		void	setBaseAnchory(float val) {this->_baseAnchor.y = val;}
		void	setBaseAnchorz(float val) {this->_baseAnchor.z = val;}
		void	setColorr(float val) {this->_color.x = val;}
		void	setColorg(float val) {this->_color.y = val;}
		void	setColorb(float val) {this->_color.z = val;}
		void	setBaseAnchor(const glm::vec3 &val)
		{
			this->_baseAnchor = val;
		}
		void	setColor(const glm::vec3 &val)
		{
			this->_color = val;
		}
		glm::vec3	getPointAnchor() const;
		glm::vec3	getBaseAnchor() {return (this->_baseAnchor);}
		glm::vec3	getColor() {return (this->_color);}
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
