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

		void	update(float delta, const Mat4f &parent)
		{
			(void)parent;

			timeline.update(delta);

			// void	Part::updateAnchor(const glm::mat4 &parentMat)
			// {
			// 	glm::vec3 scale;
			// 	glm::quat rotation;
			// 	glm::vec3 translation;
			// 	glm::vec3 skew;
			// 	glm::vec4 perspective;
			// 	glm::decompose(parentMat, scale, rotation, translation, skew, perspective);

			// 	_mat = glm::scale(_mat, 1.0f / scale);
			// 	_mat = glm::translate(_mat, _pointAnchor * (scale / 2.0f));
			// }

			// _mat = parentMat;
			// updateAnchor(parentMat);

			Vec3f	tml_translation = timeline.getTranslation();
			Vec3f	tml_rotation = timeline.getRotation();
			Vec3f	tml_scale = timeline.getScale();

			_mat = Mat4f(1);

			_mat = _mat * translate(tml_translation - _baseAnchor * (tml_scale / 2.0f));
			_mat = _mat * translate((_baseAnchor / 2.0f) * (tml_scale));
			_mat = _mat * rotate(radians(tml_rotation.x), Vec3f(1, 0, 0));
			_mat = _mat * rotate(radians(tml_rotation.y), Vec3f(0, 1, 0));
			_mat = _mat * rotate(radians(tml_rotation.z), Vec3f(0, 0, 1));
			_mat = _mat * translate((_baseAnchor * -1) * (tml_scale / 2.0f));
			_mat = _mat * scale(tml_scale);

			ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf;
			if (tree_selected)
				flag |= ImGuiTreeNodeFlags_Selected;

			if (ImGui::TreeNodeEx(_id.c_str(), flag))
			{
				if (ImGui::IsItemClicked())
					tree_selected = !tree_selected;

				for (auto child : _children)
					child->update(delta, _mat);
				ImGui::TreePop();
			}
			if (tree_selected)
			{
				if (ImGui::Begin(_id.c_str()))
				{
					ImGui::Text("%s", _id.c_str());
					ImGui::ColorPicker3("color", &_color.x);
					ImGui::DragFloat3("base_anchor", &_baseAnchor.x);
					ImGui::DragFloat3("point_anchor", &_pointAnchor.x);
					char	buf[64] = {0};

					if (ImGui::InputText("new children", buf, 63, ImGuiInputTextFlags_EnterReturnsTrue))
						_children_request = buf;
				}
				ImGui::End();
			}
		}
		void	draw(Shader &shader)
		{
			_upload();

			shader.setMat4f("model", _mat);
			shader.setVec3f("color", _color);

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			for (auto child : _children)
				child->draw(shader);
		}
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
		}
		std::string	id()
		{
			return (_id);
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

		Mat4f				_mat = Mat4f(1);
		Vec3f				_pointAnchor;
		Vec3f				_baseAnchor;
		Vec3f				_color;

		std::string			_id;

		std::string		_children_request;

		bool			tree_selected = false;

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
