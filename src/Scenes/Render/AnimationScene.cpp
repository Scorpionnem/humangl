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
#include "ImprovedTextField.hpp"

void	AnimationScene::_selectPart(Part *part)
{
	_selectedPart = part;
	if (!_selectedPart)
		static_cast<ImprovedTextField*>(_editorPanel.get("selected_part_field"))->resetInput();
	else
		static_cast<ImprovedTextField*>(_editorPanel.get("selected_part_field"))->setInput(_selectedPart->id());
}

void	AnimationScene::_addPart(const std::string &name)
{
	if (name.size() && !_model.getPart(name) && _selectedPart)
	{
		std::cout << "added part" << std::endl;
		Timeline	*tml = new Timeline();
		Part		*part = new Part(name);
	
		_animation.addTimeLine(name, tml);
		_model.setChild(_selectedPart, part);

		part->setTimeline(tml);
		part->setBaseAnchor(glm::vec3(0));
		part->setPointAnchor(glm::vec3(0));
		part->setColor(glm::vec3(1, 0, 1));
	
		_animation.get(name)->addKeyFrame(KeyFrameType::SCALE, KeyFrame<glm::vec3>(0, glm::vec3(1, 1, 1), KeyFrameType::SCALE));

		_selectPart(part);
	}
	static_cast<ImprovedTextField*>(_editorPanel.get("add_part_field"))->resetInput();		
}

void	AnimationScene::_createEditorInterface()
{
	ShaderManager &shaders = _game->getShaders();
	TextureManager &textures = _game->getTextures();

	_editorPanel.clear();

	UIElement *tmp = NULL;

	_editorPanel.add("timeline_background", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, 60), glm::vec2(0.5, 1), glm::vec2(0), glm::vec2(1, 0), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));
	tmp = _editorPanel.add("timeline_text", new ImprovedText("Timeline", 1, glm::vec2(0.5, 1.0), glm::vec2(0, -62), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedText*>(tmp)->setColor(glm::vec4(glm::vec3(0.7), 1.0));

	_editorPanel.add("keyframe_background", new ImprovedBackgroundImage(glm::vec2(80, 196), glm::vec2(1, 0.5), glm::vec2(0), glm::vec2(0, 0), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));
	tmp = _editorPanel.add("keyframe_text", new ImprovedText("Keyframe", 1, glm::vec2(1.0, 0.5), glm::vec2(0, -104), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedText*>(tmp)->setColor(glm::vec4(glm::vec3(0.7), 1.0));

	_editorPanel.add("body_part_background", new ImprovedBackgroundImage(glm::vec2(80, 196), glm::vec2(0, 0.5), glm::vec2(0), glm::vec2(0, 0), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));
	tmp = _editorPanel.add("body_part_text", new ImprovedText("Body Parts", 1, glm::vec2(0.0, 0.5), glm::vec2(0, -104), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedText*>(tmp)->setColor(glm::vec4(glm::vec3(0.7), 1.0));

	tmp = _editorPanel.add("selected_part_field", new ImprovedTextField(glm::vec2(78, 20), glm::vec2(0.0, 0.5), glm::vec2(1, -79), shaders.get("font"), shaders.get("image"), textures.get(TX_PATH_ASCII), textures.get(TX_PATH_TEXT_FIELD), textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED)));
	_editorPanel.add("selected_part_text", new ImprovedText("Selected Part", 1, glm::vec2(0.0, 0.5), glm::vec2(3, -93), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedTextField*>(tmp)->setClickFunc(
	[this](std::string str)
	{
		_selectPart(_model.getPart(str));
	});

	tmp = _editorPanel.add("add_part_field", new ImprovedTextField(glm::vec2(78, 20), glm::vec2(0.0, 0.5), glm::vec2(1, -44), shaders.get("font"), shaders.get("image"), textures.get(TX_PATH_ASCII), textures.get(TX_PATH_TEXT_FIELD), textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED)));
	static_cast<ImprovedTextField*>(tmp)->setClickFunc(
	[this](std::string str)
	{
		_addPart(hgl::formatName(str));
	});
	_editorPanel.add("add_part_text", new ImprovedText("Add Part", 1, glm::vec2(0.0, 0.5), glm::vec2(3, -58), shaders.get("font"), textures.get(TX_PATH_ASCII)));
}

void	AnimationScene::_updateEditorInterface()
{
}

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

	_createEditorInterface();

	UIElement *tmp = _pausePanel.add("back", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(0, -8), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new TitleScene(_game));
		});
	_pausePanel.add("back_text", new ImprovedText("Save and Quit", 1, glm::vec2(0.5, 1.0), glm::vec2(0, -14), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	_panel.add("fps", new ImprovedText("0 fps", 1, glm::vec2(0), glm::vec2(0), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	_loaded = true;
}

void	AnimationScene::onExit()
{
	// _model.exportAnimation(_animPath);
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
	_editorPanel.handleEvents(events);
	_pausePanel.handleEvents(events);

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
	_editorPanel.draw(window.getSize());
	_pausePanel.draw(window.getSize());
}
