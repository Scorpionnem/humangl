#pragma once

#include <vector>
#include <fstream>

#include "Math.hpp"

struct	Keyframe
{
	Keyframe(float time, Vec3f v)
	{
		this->time = time;
		this->v = v;
	}
	float	time;
	Vec3f	v;
	bool	selected = false;
};

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

class	Timeline
{
	public:
		void	draw(const char *label)
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
		Vec3f	getTranslation()
		{
			return (_get(_translationKeyframes));
		}
		Vec3f	getRotation()
		{
			return (_get(_rotationKeyframes));
		}
		Vec3f	getScale()
		{
			return (_get(_scaleKeyframes));
		}

		void	addTranslation(const Keyframe &k)
		{
			return (_add(k, _translationKeyframes));
		}
		void	addRotation(const Keyframe &k)
		{
			return (_add(k, _rotationKeyframes));
		}
		void	addScale(const Keyframe &k)
		{
			return (_add(k, _scaleKeyframes));
		}
		Vec3f	_get(std::vector<Keyframe> &keyframes)
		{
			if (!keyframes.size())
				return (Vec3f(0));
			if (_time <= keyframes.front().time)
				return (keyframes.front().v);
			if (_time >= keyframes.back().time)
				return (keyframes.back().v);
		
			for (size_t i = 0; i < keyframes.size() - 1; i++)
			{
				const Keyframe& p1 = keyframes[i];
				const Keyframe& p2 = keyframes[i + 1];
			
				if (_time >= p1.time && _time <= p2.time)
				{
					float t = (_time - p1.time) / (p2.time - p1.time);
					return (mix(p1.v, p2.v, t));
				}
			}
			return (Vec3f(0.0f));
		}
		void	_add(const Keyframe &k, std::vector<Keyframe> &keyframes)
		{
			if (k.time > _biggest_time)
				_biggest_time = k.time;
			keyframes.push_back(k);
			_sort(keyframes);
		}
		void	update(float delta)
		{
			_sort(_translationKeyframes);
			_sort(_scaleKeyframes);
			_sort(_rotationKeyframes);
			for (auto &k : _translationKeyframes)
				if (k.time > _biggest_time)
					_biggest_time = k.time;
			for (auto &k : _scaleKeyframes)
				if (k.time > _biggest_time)
					_biggest_time = k.time;
			for (auto &k : _rotationKeyframes)
				if (k.time > _biggest_time)
					_biggest_time = k.time;
			_time += delta;
			if (_time > _biggest_time)
				_time = 0;
		}
		void	export_to(std::ofstream &file)
		{
			for (auto &keyframe : _translationKeyframes)
				file << "kft " << keyframe.time << " " << keyframe.v.x << " " << keyframe.v.y << " " << keyframe.v.z << std::endl;
			for (auto &keyframe : _rotationKeyframes)
				file << "kfr " << keyframe.time << " " << keyframe.v.x << " " << keyframe.v.y << " " << keyframe.v.z << std::endl;
			for (auto &keyframe : _scaleKeyframes)
				file << "kfs " << keyframe.time << " " << keyframe.v.x << " " << keyframe.v.y << " " << keyframe.v.z << std::endl;
			(void)file;
		}
	private:
		void	_sort(std::vector<Keyframe> &keyframes)
		{
			std::sort(keyframes.begin(), keyframes.end(),
				[](const Keyframe &cp1, const Keyframe &cp2)
				{
					return (cp1.time < cp2.time);
				});
		}
		float					_time = 0;
		float					_biggest_time = 0;

		std::vector<Keyframe>	_translationKeyframes;
		std::vector<Keyframe>	_rotationKeyframes;
		std::vector<Keyframe>	_scaleKeyframes;
};
