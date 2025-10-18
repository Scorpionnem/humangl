/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 18:17:05 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TitleScene.hpp"
#include "Game.hpp"
#include "OptionsScene.hpp"
#include "ErrorScene.hpp"
#include "LoadAnimScene.hpp"
#include "SingleplayerScene.hpp"

constexpr uint SPLASH_TEXT_COUNT = 2;
constexpr double SPLASH_TEXT_INTERVAL = 10.0;

constexpr const char	*SPLASH_TEXT_TITLES[SPLASH_TEXT_COUNT] =
{
	"42 Angouleme",
	"It's all but humans",
};

#include "ImprovedButton.hpp"
#include "ImprovedText.hpp"
#include "ImprovedBackgroundImage.hpp"
#include "ImprovedImage.hpp"
#include "ImprovedToggle.hpp"
#include "ImprovedTextField.hpp"
#include "ImprovedSlider.hpp"
#include "Quad.hpp"

void	TitleScene::onEnter()
{
	if (_loaded)
		return ;

	_loaded = true;
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	// Scene decoration
	_panel.add("background", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, REFERENCE_HEIGHT), glm::vec2(0), glm::vec2(0), glm::vec2(1), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));

	_panel.add("icon", new ImprovedImage(
		glm::vec2(textures.get(TX_PATH_FT_HUMANGL)->getWidth() * 0.25, textures.get(TX_PATH_FT_HUMANGL)->getHeight() * 0.25),
		glm::vec2(0.5, 0.5), glm::vec2(0, -70), shaders.get("image"), textures.get(TX_PATH_FT_HUMANGL)));

	_panel.add("credits", new ImprovedText("By mbatty & mbirou", 1, glm::vec2(1), glm::vec2(0), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	UIElement *tmp = _panel.add("splash_text", new ImprovedText(SPLASH_TEXT_TITLES[rand() % SPLASH_TEXT_COUNT], 1, glm::vec2(0.5), glm::vec2(130, -60), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedText*>(tmp)->setAngle(-15);
	static_cast<ImprovedText*>(tmp)->setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	// Options button to access options scene
	tmp = _panel.add("options", new ImprovedButton(glm::vec2(98, 20), glm::vec2(0.5), glm::vec2(-51, 40), shaders.get("image"), textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED)));
	_panel.add("options_text", new ImprovedText("Options", 1, glm::vec2(0.5), glm::vec2(-51, 40), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new OptionsScene(_game));
		});

	// Quit button to leave the game
	tmp = _panel.add("quit", new ImprovedButton(glm::vec2(98, 20), glm::vec2(0.5), glm::vec2(51, 40), shaders.get("image"), textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED)));
	_panel.add("quit_text", new ImprovedText("Quit", 1, glm::vec2(0.5), glm::vec2(51, 40), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			_game->setRunning(false);
		});

	// Button to go to the singleplayer world selection scene
	tmp = _panel.add("singpleplayer", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5), glm::vec2(0), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	_panel.add("singleplayer_text", new ImprovedText("Load/Create Animation", 1, glm::vec2(0.5), glm::vec2(0), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			_requestScene(new LoadAnimScene(_game));
		});
}

void	TitleScene::onExit()
{
}

void	TitleScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	TitleScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;

	_panel.handleEvents(events);

	// Update splash text
	ImprovedText *splashText = static_cast<ImprovedText*>(_panel.get("splash_text"));
	splashText->setScale(1.0 + abs(cos(glfwGetTime() * 4) / 6));
	if (glfwGetTime() - _lastSplashTextUpdate > SPLASH_TEXT_INTERVAL)
	{
		splashText->setText(SPLASH_TEXT_TITLES[rand() % SPLASH_TEXT_COUNT]);

		_lastSplashTextUpdate = glfwGetTime();
	}
}

void	TitleScene::render()
{
	Window	&window = _game->getWindow();

	glDisable(GL_DEPTH_TEST);

	_panel.draw(window.getSize());
}
