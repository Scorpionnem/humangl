/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Button.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 18:34:43 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 12:22:08 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUTTON_HPP
# define BUTTON_HPP

#include "UIElement.hpp"

struct	ButtonInfo
{
	void				*data;
	const std::string	id;
	const std::string	label;
	class Button				*button;
};

/*
	@brief	Represents a simple clickable UI element

	When initialized the button will use basic textures/shaders from the Texture/ShaderManager, use the setter functions to use custom shaders
*/
class	Button : public UIElement
{
	public:
		/*
			@brief	Simple anchored button

			Constructor for anchored buttons, the pos argument will be added to the anchor point so
			use positive/negative values to move the button around its anchor

			@param	anchor      Where the button should be anchored
			@param	label       Label of the button to be displayed
			@param	offset      Offset from anchor
			@param	size        Size of the button
			@param	onClick     function to be called when button is clicked, NULL to do nothing
			@param	clickData   Data passed to the onClick function
		*/
		Button(UIAnchor anchor, std::string label, glm::vec2 offset, glm::vec2 size, std::function<void(ButtonInfo)> onClick, void *clickData)
		{
			type = UIElementType::UITYPE_BUTTON;
			this->label = label;
			this->offset = offset;
			this->pos = glm::vec2(0);
			this->size = size;
			this->onClick = onClick;
			this->clickData = clickData;
			this->anchor = anchor;

			anchorPos();
		}
		~Button(){}

		void	draw();
		void	update(glm::vec2 mousePos, bool mousePressed);
		void	setClickData(void *data)
		{
			this->clickData = data;
		}

		bool						wasPressedInside = false;
		bool						previousMousePressed = false;

		std::function<void(ButtonInfo)>	onClick = NULL;
		void						*clickData = NULL;

		std::string					label;

		Texture						*currentTexture = NULL;
};

#endif
