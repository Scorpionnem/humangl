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
uint	VAO;
uint	VBO;

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

class	Part
{
	public:
		Part() {}
		~Part() {}

		void	update(const Mat4f &parent)
		{
			(void)parent;

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

			// glm::vec3	translation = _timeline->getValue(KeyFrameType::TRANSLATION);
			// glm::vec3	rotation = _timeline->getValue(KeyFrameType::ROTATION);
			// glm::vec3	scale = _timeline->getValue(KeyFrameType::SCALE);

			// _mat = glm::translate(_mat, translation - _baseAnchor * (scale / 2.0f));
			// _mat = glm::translate(_mat, (_baseAnchor / 2.0f) * (scale));
			// _mat = glm::rotate(_mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
			// _mat = glm::rotate(_mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
			// _mat = glm::rotate(_mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
			// _mat = glm::translate(_mat, -_baseAnchor * (scale / 2.0f));
			// _mat = glm::scale(_mat, scale);

			for (Part *child : _children)
				child->update(_mat);
		}
		void	draw(Shader &shader)
		{
			shader.setMat4f("model", _mat);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			for (Part *child : _children)
				child->draw(shader);
		}
		void	addChild(Part *child)
		{
			_children.push_back(child);
		}
	private:
		std::vector<Part*>	_children;

		Mat4f				_mat = Mat4f(1);
		Vec3f				_pointAnchor;
		Vec3f				_baseAnchor;
};

void	App::_loop()
{
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

	Part	part;

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

		part.draw(shader);

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
