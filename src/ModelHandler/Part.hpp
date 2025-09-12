/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:31:40 by mbirou            #+#    #+#             */
/*   Updated: 2025/09/12 12:01:45 by mbatty           ###   ########.fr       */
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
		void	exportTimeline(std::string path)
		{
			std::ofstream	file;

			file.open(path);
			if (!file.is_open())
				return ;

			_addHeader(file);

			_defineExport(file);
			file << std::endl;
			
			for (auto &part : _children)
				part._exportObject(file);
			_exportObject(file);
		}

		void	updateAnchor(const glm::mat4 &parentMat);
		void	update(const glm::mat4 &parentMat);
		void	draw();

		glm::vec3	getAnchor() const;

		static std::vector<float> defaultCube;
	
	private:
		void	_addHeader(std::ofstream &file)
		{
			file << "# 42 humangl project export file by mbatty & mbirou\n# keys used in the file\n#	define : string name\n#		-Defines an object to be used in the file\n#	object : string name\n#		-Sets the object as used\n#	children : string name\n#		-Sets the children of the used object\n#	kft : float time : vec3 translation\n#		-Sets the translation values to the used object\n#	kfr : float time : vec3 rotation\n#		-Sets the rotation values to the used object\n#	kfs : float time : vec3 scale\n#		-Sets the scale values to the used object\n" << std::endl;
		}
		void	_exportObject(std::ofstream &file)
		{
			file << "object " << _id << std::endl;
			_childrenExport(file);
			_timeLine.exportTimeline(file);
			file << std::endl;
		}
		void	_defineExport(std::ofstream &file)
		{
			for (auto &part : _children)
				part._defineExport(file);
			file << "define " << _id << std::endl;
		}
		void	_childrenExport(std::ofstream &file)
		{
			for (auto &part : _children)
				file << "children " << part._id << std::endl;
		}
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
