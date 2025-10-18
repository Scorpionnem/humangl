/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadAnimScene.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:01:48 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 17:59:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOADANIMSCENE_HPP
# define LOADANIMSCENE_HPP

# include "Panel.hpp"
# include "Scene.hpp"
# include "UIElement.hpp"

class	LoadAnimScene : public Scene
{
	public:
		LoadAnimScene(Game *game) : Scene(game)
		{
			_id = "LoadAnimScene";
		}
		~LoadAnimScene() {};

		void onEnter();

		void processInput(float deltaTime);
		void update(UIEvent events, float deltaTime);
		void render();

		void onExit();

	private:
		std::string	_name;
		Panel	_panel;
};

#endif
