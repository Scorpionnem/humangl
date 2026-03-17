#include "App.hpp"
#include "Shader.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

#include <stdexcept>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

std::vector<float> cube_vertices = {
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

void	App::_loop()
{
	uint	VAO;
	uint	VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(float), cube_vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	Shader	shader;

	shader.load(GL_VERTEX_SHADER, "assets/shaders/mesh.vs");
	shader.load(GL_FRAGMENT_SHADER, "assets/shaders/mesh.fs");
	shader.link();

	Camera	cam;

	while (_window.is_open())
	{
		const Window::Events	&events = _window.pollEvents();
		if (events.getKey(SDLK_ESCAPE))
		{
			_window.close();
			break ;
		}

		shader.use();

		float	speed = 50 * events.getDeltaTime();

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
			cam.pitch += speed * 2;
		if (events.getKey(SDLK_DOWN))
			cam.pitch -= speed * 2;
		if (events.getKey(SDLK_RIGHT))
			cam.yaw += speed * 2;
		if (events.getKey(SDLK_LEFT))
			cam.yaw -= speed * 2;

		cam.update(events.getDeltaTime(), events.getAspectRatio());

		shader.setMat4f("model", Mat4f(1.0));
		shader.setMat4f("view", cam.getViewMatrix());
		shader.setMat4f("projection", perspective<float>(90.0, events.getAspectRatio(), 0.01, 1000.0));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// if (ImGui::Begin("App"))
		// {
		// 	ImGui::Text("%f", 1.0 / events.getDeltaTime());
		// }
		// ImGui::End();

		_window.render();
	}
}

void	App::_init()
{
	_window.open("I Love Voxels", 512, 384);
}

void	App::run(void)
{
	_init();

	_loop();
}
