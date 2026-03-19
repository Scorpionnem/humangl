#include "App.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Rig.hpp"

#include <stdexcept>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include "WavFile.hpp"

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

void	App::_loop(std::string path)
{
	Shader	shader;

	shader.load(GL_VERTEX_SHADER, "assets/shaders/mesh.vs");
	shader.load(GL_FRAGMENT_SHADER, "assets/shaders/mesh.fs");
	shader.link();

	Camera	cam;

	std::unique_ptr<Rig>	rig = std::make_unique<Rig>();
	rig->load(path);

	WavFile	wav;

	constexpr	int	MUSICS_COUNT = 3;
	constexpr const char	*MUSICS[MUSICS_COUNT] =
	{
		"assets/musics/golden_brown.wav",
		"assets/musics/gas_gas_gas.wav",
		"assets/musics/tokyo_drift.wav",
	};

	srand(std::time(NULL));

	std::string	msc = MUSICS[rand() % MUSICS_COUNT];

	std::cout << "Playing: " << msc << std::endl;

	wav.load(msc);
	wav.play();

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

		wav.update(events.getDeltaTime());

		rig->update(events.getDeltaTime());
		rig->draw(shader);

		ImGui::Begin("Rig File");
			char	buf[64] = {0};

			if (ImGui::InputText(".hgl file", buf, 63, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				std::string	inpt = buf;
				rig->export_to(path);
				rig = std::make_unique<Rig>();
				try {
					rig->load(inpt);
					path = inpt;
				} catch (const std::exception &e) {
					std::cerr << "Failed to load file " << e.what() << std::endl;
					rig = std::make_unique<Rig>();
					rig->load(path);
				}
			}
		ImGui::End();

		_window.render();

		if (events.getKey(SDLK_LCTRL) && events.getKeyPressed(SDLK_r))
			shader.reload();
	}
	rig->export_to("export.hgl");
}

void	App::_init()
{
	_window.open("humanGL", 1024, 768);
}

void	App::run(const std::string &path)
{
	_init();

	_loop(path);
}
