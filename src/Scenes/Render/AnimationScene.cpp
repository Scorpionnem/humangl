/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnimationScene.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:02:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 13:03:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AnimationScene.hpp"
#include "Game.hpp"
#include "Quad.hpp"
#include "Game.hpp"
#include "ImprovedButton.hpp"
#include "TitleScene.hpp"
#include "ErrorScene.hpp"
#include "ImprovedText.hpp"

void	AnimationScene::onEnter()
{
	if (_loaded)
		return ;

	_animation.load(_animPath, &_model);

	_camera.setPos(glm::vec3(30, 1, 0));
	_camera.setYaw(180.0f);
	_camera.setPitch(0);

	ShaderManager &shaders = _game->getShaders();
	TextureManager &textures = _game->getTextures();

	UIElement *tmp = _panel.add("back", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(0, -8), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new TitleScene(_game));
		});
	_panel.add("back_text", new ImprovedText(_animPath, 1, glm::vec2(0.5, 1.0), glm::vec2(0, -14), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	_panel.add("fps", new ImprovedText("0 fps", 1, glm::vec2(0), glm::vec2(0), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	_loaded = true;
}

void	AnimationScene::onExit()
{
}

void	AnimationScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	AnimationScene::_updateCamera(UIEvent &events, float deltaTime)
{
	float	cameraSpeed = 1 * deltaTime;
	float	speedBoost = 1.0f;

	if (events.inputs->isKeyDown(GLFW_KEY_LEFT_CONTROL))
		speedBoost = 20.0f;

	if (events.inputs->isKeyDown(GLFW_KEY_LEFT_ALT))
		speedBoost *= 15.0f;

	if (events.inputs->isKeyDown(GLFW_KEY_W))
		_camera.setPos(_camera.getPos() + _camera.getFront() * (cameraSpeed * speedBoost));
	if (events.inputs->isKeyDown(GLFW_KEY_S))
		_camera.setPos(_camera.getPos() - _camera.getFront() * (cameraSpeed * speedBoost));
	if (events.inputs->isKeyDown(GLFW_KEY_SPACE))
		_camera.setPos(_camera.getPos() + _camera.getUp() * (cameraSpeed * speedBoost));
	if (events.inputs->isKeyDown(GLFW_KEY_LEFT_SHIFT))
		_camera.setPos(_camera.getPos() - _camera.getUp() * (cameraSpeed * speedBoost));

	if (events.inputs->isKeyDown(GLFW_KEY_A))
		_camera.setPos(_camera.getPos() - glm::normalize(glm::cross(_camera.getFront(), _camera.getUp())) * (cameraSpeed * speedBoost));
	if (events.inputs->isKeyDown(GLFW_KEY_D))
		_camera.setPos(_camera.getPos() + glm::normalize(glm::cross(_camera.getFront(), _camera.getUp())) * (cameraSpeed * speedBoost));

	_camera.update(deltaTime);
}

void	AnimationScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;
	_panel.handleEvents(events);

	_updateCamera(events, deltaTime);

	_model.update(deltaTime);

	static_cast<ImprovedText*>(_panel.get("fps"))->setText(std::to_string((int)(1.0f / _game->getWindow().getDeltaTime())));
}

void	AnimationScene::render()
{
	Window	&window = _game->getWindow();

	_camera.setViewProjectionMatrix(_game->getShaders().get("cube"), 80, _game->getWindow().getAspectRatio(), 1024);

	glEnable(GL_DEPTH_TEST);
	_model.draw(_game->getShaders().get("cube"));
	glDisable(GL_DEPTH_TEST);

	_panel.draw(window.getSize());
}
