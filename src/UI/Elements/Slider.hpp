/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Slider.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:36:46 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 12:20:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SLIDER_HPP
# define SLIDER_HPP

# include "UIElement.hpp"

/*
	@brief	Represents a simple slider UI element

	When initialized the slider will use basic textures/shaders from the Texture/ShaderManager, use the setter functions to use custom shaders
*/
class	Slider : public UIElement
{
	public:
		Slider(UIAnchor anchor, std::string label, glm::vec2 offset, glm::vec2 size, std::function<void(float)> onChange, std::function<void(Slider *)> onUpdate, float startValue);
		~Slider(){}
		void	draw();
		void	update(glm::vec2 mousePos, bool mousePressed);
		void    setSlider(float value)
		{
			value = glm::clamp(value, 0.0f, 1.0f);
			this->value = value;

			float minCenter = pos.x + (sliderWidth / 2);
			float maxCenter = pos.x + size.x - (sliderWidth / 2);
			float centerX = minCenter + value * (maxCenter - minCenter);

			sliderPos.x = centerX - (sliderWidth / 2);
			sliderPos.y = pos.y;
		}

		void	setLabel(const std::string &newLabel)
		{
			this->label = newLabel;
		}

		bool						wasPressedInside = false;
		bool						previousMousePressed = false;
		bool						dragging = false;

		std::function<void(float)>	onChange = NULL;
		std::function<void(Slider *)>	onUpdate = NULL;
		void								*clickData = NULL;

		Texture						*currentTexture;
		Texture						*backgroundTexture;

		glm::vec2						sliderPos;
		float						sliderWidth = 30;
		float						value = 0;
		std::string					label;

		Shader						*_buttonShader;
};

#endif
