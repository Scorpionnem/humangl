#include "App.hpp"
#include "Shader.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"
#include "Rig.hpp"

#include <stdexcept>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

void	updateCamera(Camera &cam, const Window::Events &events)
{
	float	speed = 25 * events.getDeltaTime();
	float	sensibility = 50 * events.getDeltaTime();

	if (events.getKey(SDLK_w))
		cam.pos = cam.pos + cam.front * speed;
	if (events.getKey(SDLK_s))
		cam.pos = cam.pos - cam.front * speed;
	if (events.getKey(SDLK_SPACE))
		cam.pos = cam.pos + cam.up * speed;
	if (events.getKey(SDLK_LSHIFT))
		cam.pos = cam.pos - cam.up * speed;
	if (events.getKey(SDLK_a))
		cam.pos = cam.pos - normalize(cross(cam.front, cam.up)) * speed;
	if (events.getKey(SDLK_d))
		cam.pos = cam.pos + normalize(cross(cam.front, cam.up)) * speed;
	if (events.getKey(SDLK_UP))
		cam.pitch += sensibility * 2;
	if (events.getKey(SDLK_DOWN))
		cam.pitch -= sensibility * 2;
	if (events.getKey(SDLK_RIGHT))
		cam.yaw += sensibility * 2;
	if (events.getKey(SDLK_LEFT))
		cam.yaw -= sensibility * 2;

	cam.update(events.getDeltaTime(), events.getAspectRatio());
}

void	App::_loop(const std::string &path)
{
	Shader	shader;

	shader.load(GL_VERTEX_SHADER, "assets/shaders/mesh.vs");
	shader.load(GL_FRAGMENT_SHADER, "assets/shaders/mesh.fs");
	shader.link();

	Camera	cam;

	Rig	rig;

	rig.load(path);

	while (_window.is_open())
	{
		const Window::Events	&events = _window.pollEvents();
		if (events.getKey(SDLK_ESCAPE))
		{
			_window.close();
			break ;
		}

		updateCamera(cam, events);

		shader.use();
		shader.setMat4f("view", cam.getViewMatrix());
		shader.setMat4f("projection", perspective<float>(90.0, events.getAspectRatio(), 0.01, 1000.0));

		rig.update(events.getDeltaTime());
		rig.draw(shader);

		// if (ImGui::Begin("App"))
		// {
		// 	ImGui::Text("%f", 1.0 / events.getDeltaTime());
		// }
		// ImGui::End();

		_window.render();
	}
	rig.export_to("export.hgl");
}

void	App::_init()
{
	_window.open("I Love Voxels", 512 * 2, 384 * 2);
}

void	App::run(const std::string &path)
{
	_init();

	_loop(path);
}
