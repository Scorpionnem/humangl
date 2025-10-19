/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnimationScene.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:55 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/19 09:29:30 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AnimationScene_HPP
# define AnimationScene_HPP

# include "Scene.hpp"
# include "UIElement.hpp"
# include "Panel.hpp"
# include "Part.hpp"
# include "Camera.hpp"
# include "Model.hpp"
# include "Animation.hpp"

class AnimationScene : public Scene
{
	public:
		AnimationScene(Game *game, const std::string &path) : Scene(game)
		{
			_id = "AnimationScene";
			_animPath = path;
		}
		~AnimationScene() {};

		void onEnter();

		void processInput(float deltaTime);
		void update(UIEvent events, float deltaTime);
		void render();

		void onExit();

	private:
		void	_updateCamera(UIEvent &events, float deltaTime);

		Panel		_panel;
		std::string	_animPath;
		
		Camera		_camera;

		Animation				_animation;
		Model					_model;
		// Part					*_selectedPart = NULL;
		// KeyFrame<glm::vec3>	*_selectedKeyframe = NULL;
};

#endif
