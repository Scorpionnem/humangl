/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnimationScene.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:55 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/20 08:31:18 by mbatty           ###   ########.fr       */
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
		void	_createEditorInterface();
		void	_updateEditorInterface();
		
		void	_selectPart(Part *part);
		void	_addPart(const std::string &name);
		void	_updateCamera(UIEvent &events, float deltaTime);

		Panel		_panel;
		Panel		_editorPanel;
		Panel		_pausePanel;
		std::string	_animPath;
		
		Camera		_camera;

		Animation				_animation;
		Model					_model;
		Part					*_selectedPart = NULL;
		// KeyFrame<glm::vec3>	*_selectedKeyframe = NULL;
};

#endif
