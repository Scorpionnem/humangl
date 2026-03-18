#include "Part.hpp"

uint				Part::_VAO = 0;
uint				Part::_VBO = 0;

void	Part::draw(Shader &shader)
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

void	Part::update(float delta, const Mat4f &parent)
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
			ImGui::DragFloat3("base_anchor", &_baseAnchor.x);
			ImGui::DragFloat3("point_anchor", &_pointAnchor.x);
			ImGui::Checkbox("show timeline", &show_timeline);

			char	buf[64] = {0};					
			if (ImGui::InputText("new children", buf, 63, ImGuiInputTextFlags_EnterReturnsTrue))
				_children_request = buf;

			ImGui::ColorPicker3("color", &_color.x);
		}
		ImGui::End();
	}

	if (show_timeline && tree_selected)
		timeline.draw(("timeline_" + _id).c_str());
}
