#pragma once

#include "Math.hpp"
#include "Timeline.hpp"
#include "Shader.hpp"
#include <vector>
#include <fstream>
#include <memory>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

class	Part
{
	public:
		Part(const std::string &id): _id(id) {}
		~Part() {}

		void	update(float delta, const Mat4f &parent);
		void	draw(Shader &shader);
		void	setColor(const Vec3f &color)
		{
			_color = color;
		}
		void	setBaseAnchor(const Vec3f &baseAnchor)
		{
			_baseAnchor = baseAnchor;
		}
		void	setPointAnchor(const Vec3f &pointAnchor)
		{
			_pointAnchor = pointAnchor;
		}
		void	addChild(std::shared_ptr<Part> child)
		{
			_children.push_back(child);
			child->set_parent(this);
		}
		std::string	id()
		{
			return (_id);
		}

		bool	hasChild(const std::string &id)
		{
			for (auto &c : _children)
				if (c->id() == id || c->hasChild(id))
					return (true);
			return (false);
		}
		bool	hasParent(const std::string &id)
		{
			if (_parent)
			{
				if (_parent->id() == id)
					return (true);
			}
			return (false);
		}
		void	export_object(std::ofstream &file)
		{
			file << "object " << _id << std::endl;
			export_children(file);

			file << "banchor " << _baseAnchor.x << " " << _baseAnchor.y << " " << _baseAnchor.z << '\n';
			file << "panchor " << _pointAnchor.x << " " << _pointAnchor.y << " " << _pointAnchor.z << '\n';
			file << "color " << _color.x << " " << _color.y << " " << _color.z << '\n';

			timeline.export_to(file);
			file << '\n';

			for (auto &part : _children)
				part->export_object(file);
		}
		void	export_children(std::ofstream &file)
		{
			for (auto &part : _children)
				file << "children " << part->_id << std::endl;
		}
		void	export_define(std::ofstream &file)
		{
			for (auto &part : _children)
				part->export_define(file);
			file << "define " << _id << std::endl;
		}
		std::string	children_request()
		{
			std::string	tmp = _children_request;
			_children_request.clear();
			return (tmp);
		}
		Timeline			timeline;

		void	set_parent(Part *p)
		{
			_parent = p;
		}
	private:
		static void	_upload()
		{
			if (_VAO != 0)
				return ;

			glGenVertexArrays(1, &_VAO);
			glGenBuffers(1, &_VBO);
			glBindVertexArray(_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
		}
		std::vector<std::shared_ptr<Part>>	_children;
		Part								*_parent = NULL;

		Mat4f				_mat = Mat4f(1);
		Vec3f				_pointAnchor;
		Vec3f				_baseAnchor;
		Vec3f				_color;

		std::string			_id;

		std::string		_children_request;

		bool			tree_selected = false;
		bool			show_timeline = true;

		static uint				_VAO;
		static uint				_VBO;
		static constexpr const float	cube_vertices[108] = {
							// front face
							-0.5f, -0.5f,  0.5f,
							0.5f,  0.5f,  0.5f,
							0.5f, -0.5f,  0.5f,

							0.5f,  0.5f,  0.5f,
							-0.5f, -0.5f,  0.5f,
							-0.5f,  0.5f,  0.5f,

							// back face
							-0.5f, -0.5f, -0.5f,
							0.5f, -0.5f, -0.5f,
							0.5f,  0.5f, -0.5f,

							0.5f,  0.5f, -0.5f,
							-0.5f,  0.5f, -0.5f,
							-0.5f, -0.5f, -0.5f,

							// left face
							-0.5f,  0.5f,  0.5f,
							-0.5f, -0.5f, -0.5f,
							-0.5f,  0.5f, -0.5f,

							-0.5f, -0.5f, -0.5f,
							-0.5f,  0.5f,  0.5f,
							-0.5f, -0.5f,  0.5f,

							// right face
							0.5f,  0.5f,  0.5f,
							0.5f,  0.5f, -0.5f,
							0.5f, -0.5f, -0.5f,

							0.5f, -0.5f, -0.5f,
							0.5f, -0.5f,  0.5f,
							0.5f,  0.5f,  0.5f,

							// bottom face
							-0.5f, -0.5f, -0.5f,
							0.5f, -0.5f,  0.5f,
							0.5f, -0.5f, -0.5f,

							0.5f, -0.5f,  0.5f,
							-0.5f, -0.5f, -0.5f,
							-0.5f, -0.5f,  0.5f,

							// top face
							-0.5f,  0.5f, -0.5f,
							0.5f,  0.5f, -0.5f,
							0.5f,  0.5f,  0.5f,

							0.5f,  0.5f,  0.5f,
							-0.5f,  0.5f,  0.5f,
							-0.5f,  0.5f, -0.5f
						};
};
