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

	_mat = parent;
	Vec3f	Scale;
	Vec3f	skew;

	Mat4f	tpMat = parent; //used to extract the final scale of the parent

	// normalizing it
	for(int i = 0; i < 4; ++i)
		for(int ii = 0; ii < 4; ++ii)
			tpMat(i, ii) /= tpMat(3, 3);

	// getting scale and skew
	Vec3f	rows[3];
	for (int i = 0; i < 3; ++i)
	{
		rows[i].x = tpMat(i, 0);
		rows[i].y = tpMat(i, 1);
		rows[i].z = tpMat(i, 2);
	}

	Scale.x = rows[0].length();
	rows[0] = (rows[0] * 1.f / Scale.x);
	skew.z = dot(rows[0], rows[1]);
	rows[1] = (rows[1] * 1) + (rows[0] * -skew.z);

	Scale.y = rows[1].length();
	rows[1] = (rows[1] * 1.f / Scale.y);
	skew.z /= Scale.y;
	skew.y = dot(rows[0], rows[2]);
	rows[2] = (rows[2] * 1) + (rows[0] * -skew.y);
	skew.x = dot(rows[1], rows[2]);
	rows[2] = (rows[2] * 1) + (rows[1] * -skew.y);

	Scale.z = length(rows[2]);
	rows[2] = (rows[2] * 1.f / Scale.z);
	skew.y /= Scale.z;
	skew.x /= Scale.z;


	_mat = _mat * scale(Vec3f(1.0f) / Scale);
	_mat = _mat * translate(_pointAnchor * (Scale / 2.0f));

	Vec3f	tml_translation = timeline.getTranslation();
	Vec3f	tml_rotation = timeline.getRotation();
	Vec3f	tml_scale = timeline.getScale();


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
