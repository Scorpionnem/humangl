#include "Timeline.hpp"

void	Timeline::draw(const char *label)
{
	if (ImGui::Begin(label))
	{
		if (ImGui::BeginListBox("Scale Keyframes"))
		{
			int i = 0;
			for (auto &k : _scaleKeyframes)
			{
				ImGui::Checkbox(std::to_string(i++).c_str(), &k.selected);
				if (k.selected)
				{
					ImGui::InputFloat("time", &k.time);
					ImGui::DragFloat3("v", &k.v.x);
				}
			}
			ImGui::EndListBox();
		}
		if (ImGui::BeginListBox("Translation Keyframes"))
		{
			int i = 0;
			for (auto &k : _translationKeyframes)
			{
				ImGui::Checkbox(std::to_string(i++).c_str(), &k.selected);
				if (k.selected)
				{
					ImGui::InputFloat("time", &k.time);
					ImGui::DragFloat3("v", &k.v.x);
				}
			}
			ImGui::EndListBox();
		}
		if (ImGui::BeginListBox("Rotation Keyframes"))
		{
			int i = 0;
			for (auto &k : _rotationKeyframes)
			{
				ImGui::Checkbox(std::to_string(i++).c_str(), &k.selected);
				if (k.selected)
				{
					ImGui::InputFloat("time", &k.time);
					ImGui::DragFloat3("v", &k.v.x);
				}
			}
			ImGui::EndListBox();
		}

	}
	ImGui::End();
}
