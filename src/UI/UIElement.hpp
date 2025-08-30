/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIElement.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 12:42:04 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 10:38:34 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UIELEMENT_HPP
# define UIELEMENT_HPP

# include "libs.hpp"
# include "Shader.hpp"
# include "Texture.hpp"

extern unsigned int	UIquadVAO;
extern unsigned int	UIquadVBO;

enum UIAnchor
{
	UI_NONE, //Not anchored
	UI_TOP_LEFT, //Anchored on the top left of the screen
	UI_TOP_RIGHT, //Anchored on the top right of the screen
	UI_TOP_CENTER, //Anchored on the top center of the screen
	UI_TOP_CENTER_HALF, //Anchored on the half top of the screen
	UI_BOTTOM_CENTER_HALF, //Anchored on the half bottom of the screen
	UI_CENTER, //Anchored on the center of the screen
	UI_CENTER_LEFT, //Anchored on the center left of the screen
	UI_CENTER_RIGHT, //Anchored on the center right of the screen
	UI_BOTTOM_LEFT, //Anchored on the bottom left of the screen
	UI_BOTTOM_RIGHT, //Anchored on the bottom left of the screen
	UI_BOTTOM_CENTER, //Anchored on the bottom left of the screen
	UI_CENTER_HALF_LEFT, //Anchored on the half left of the screen
	UI_CENTER_HALF_RIGHT //Anchored on the half right of the screen
};

enum UIElementType
{
	UITYPE_UNSET,
	UITYPE_BUTTON,
	UITYPE_IMAGE,
	UITYPE_SLIDER,
	UITYPE_TEXT,
	UITYPE_TEXTBOX,
	UITYPE_TOGGLE,
};

class	UIElement
{
	public:
		UIElement();
		virtual ~UIElement(){}
		virtual void	draw() = 0;
		/*
			@brief update button's variables

			Updates button's variables like its current used texture (normal/pressed)
		*/
        virtual void	update(glm::vec2 mousePos, bool mousePressed) = 0;
		/*
			Returns wether the mouse is in the button or not
		*/
		bool	isInside(glm::vec2 buttonPos, glm::vec2 size, glm::vec2 mousePos);
		/*
			Simple function to init the quad "model" used for all buttons
			If the quad is already loaded, just returns
		*/
		static void	initButtonQuad();
		static uint	getQuadVAO()
		{
			initButtonQuad();
			return (UIquadVAO);
		}
		void	anchorPos();
		void	setID(const std::string &id)
		{
			this->id = id;
		}
		/*
			Simply draws a quad using any shader. Its up the the caller to define the uniforms used by the shader outside of this function
		*/
		static void	draw(Shader *shader, glm::vec2 pos, glm::vec2 size);

		glm::vec2	offset;
		glm::vec2	pos;
		glm::vec2	size;
		std::string	id; //ID of the ui element inside of an interface (Set by the interface).
		UIAnchor	anchor = UIAnchor::UI_NONE;
		Shader		*_shader;
		UIElementType	type;
};

# include "Button.hpp"
# include "Text.hpp"
# include "Slider.hpp"
# include "Image.hpp"
# include "Toggle.hpp"
# include "TextBox.hpp"

#endif
