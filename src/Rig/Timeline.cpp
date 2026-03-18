#include "Timeline.hpp"

void	Timeline::draw(const char *label)
{
	int i = 0;

	if (ImGui::Begin(label))
	{
		if (ImGui::Button("Add Scale Keyframe"))
			_scaleKeyframes.push_back(Keyframe(biggestTime(_scaleKeyframes) + 1, Vec3f(0)));
		if (ImGui::BeginListBox("Scale Keyframes"))
		{
			for (auto it = _scaleKeyframes.begin(); it != _scaleKeyframes.end(); it++)
			{
				std::string	id = std::to_string(i++);
				ImGui::Checkbox(id.c_str(), &it->selected);
				if (it->selected)
				{
					if (ImGui::Button((id + "delete").c_str()))
					{
						it = _scaleKeyframes.erase(it);
						break ;
					}
					ImGui::InputFloat((id + "time").c_str(), &it->time);
					ImGui::DragFloat3((id + "v").c_str(), &it->v.x);
				}
			}
			ImGui::EndListBox();
		}
		if (ImGui::Button("Add Translation Keyframe"))
			_translationKeyframes.push_back(Keyframe(biggestTime(_translationKeyframes) + 1, Vec3f(0)));
		if (ImGui::BeginListBox("Translation Keyframes"))
		{
			for (auto it = _translationKeyframes.begin(); it != _translationKeyframes.end(); it++)
			{
				std::string	id = std::to_string(i++);
				ImGui::Checkbox(id.c_str(), &it->selected);
				if (it->selected)
				{
					if (ImGui::Button((id + "delete").c_str()))
					{
						it = _translationKeyframes.erase(it);
						break ;
					}
					ImGui::InputFloat((id + "time").c_str(), &it->time);
					ImGui::DragFloat3((id + "v").c_str(), &it->v.x);
				}
			}
			ImGui::EndListBox();
		}
		if (ImGui::Button("Add Rotation Keyframe"))
			_rotationKeyframes.push_back(Keyframe(biggestTime(_rotationKeyframes) + 1, Vec3f(0)));
		if (ImGui::BeginListBox("Rotation Keyframes"))
		{
			for (auto it = _rotationKeyframes.begin(); it != _rotationKeyframes.end(); it++)
			{
				std::string	id = std::to_string(i++);
				ImGui::Checkbox(id.c_str(), &it->selected);
				if (it->selected)
				{
					if (ImGui::Button((id + "delete").c_str()))
					{
						it = _rotationKeyframes.erase(it);
						break ;
					}
					ImGui::InputFloat((id + "time").c_str(), &it->time);
					ImGui::DragFloat3((id + "v").c_str(), &it->v.x);
				}
			}
			ImGui::EndListBox();
		}

	}
	ImGui::End();
}
