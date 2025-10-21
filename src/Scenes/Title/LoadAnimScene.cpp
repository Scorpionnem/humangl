/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadAnimScene.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:01:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 18:00:06 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LoadAnimScene.hpp"

#include "TitleScene.hpp"
#include "AnimationScene.hpp"
#include "Game.hpp"
#include "ImprovedBackgroundImage.hpp"
#include "ImprovedButton.hpp"
#include "ImprovedText.hpp"
#include "ImprovedTextField.hpp"

void	LoadAnimScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	if (_loaded)
		return ;

	_loaded = true;

	// Decoration of the scene
	_panel.add("background_dark", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, REFERENCE_HEIGHT), glm::vec2(0), glm::vec2(0), glm::vec2(1), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));

	// Cancel button, just goes back to the previous page
	UIElement *tmp = _panel.add("cancel", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5), glm::vec2(0, 84), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	_panel.add("cancel_text", new ImprovedText("Cancel", 1, glm::vec2(0.5, 0.5), glm::vec2(0, 84), shaders.get("font"), textures.get(TX_PATH_ASCII)));	
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new TitleScene(_game));
		});
		
	// Text field to enter the animation's name
	tmp = _panel.add("name_text_field", new ImprovedTextField(glm::vec2(200, 20), glm::vec2(0.5), glm::vec2(0, 0), shaders.get("font"), shaders.get("image"), textures.get(TX_PATH_ASCII), textures.get(TX_PATH_TEXT_FIELD), textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED)));
	static_cast<ImprovedTextField*>(tmp)->setClickFunc(
	[this](std::string str)
	{
		this->_name = str;
	});
	tmp = _panel.add("name_text_field_text", new ImprovedText("Animation Name", 1, glm::vec2(0.5, 0.5), glm::vec2(0, -20), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedText*>(tmp)->setColor(glm::vec4(glm::vec3(0.7), 1.0));

	tmp = _panel.add("info_text", new ImprovedText("Animations will be loaded/created in \"./exports/\"", 1, glm::vec2(0.5, 0.5), glm::vec2(0, 40), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedText*>(tmp)->setColor(glm::vec4(glm::vec3(0.7), 1.0));

	// Done text to create a server entry and go back to multiplayer scene
	tmp = _panel.add("done", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5), glm::vec2(0, 62), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	_panel.add("done_text", new ImprovedText("Load/Create", 1, glm::vec2(0.5, 0.5), glm::vec2(0, 62), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new AnimationScene(_game, hgl::formatName(EXPORTS_PATH + _name + HGL_FORMAT)));
		});
	static_cast<ImprovedButton*>(tmp)->disable();
}

void	LoadAnimScene::onExit()
{
}

void	LoadAnimScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	LoadAnimScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;

	_panel.handleEvents(events);

	// Only allow creation of a server entry if both text fields have something in them
	if (!_name.empty())
	{
		static_cast<ImprovedText*>(_panel.get("done_text"))->setColor(glm::vec4(glm::vec3(1.0), 1.0));
		static_cast<ImprovedButton*>(_panel.get("done"))->enable();
	}
	else
	{
		static_cast<ImprovedText*>(_panel.get("done_text"))->setColor(glm::vec4(glm::vec3(0.7), 1.0));
		static_cast<ImprovedButton*>(_panel.get("done"))->disable();
	}
}

void	LoadAnimScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
}
