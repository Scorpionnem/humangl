/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Toggle.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:07:11 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/20 16:07:36 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOGGLE_HPP
# define TOGGLE_HPP

# include "UIElement.hpp"

struct	ToggleInfo
{
	void				*data;
	bool				pressed;
	const std::string	id;
	const std::string	label;
	class Toggle		*toggle;
};

class	Toggle : public UIElement
{
	public:
		Toggle(UIAnchor anchor, std::string label, glm::vec2 offset, glm::vec2 size, std::function<void(ToggleInfo)> onClick, std::function<void(ToggleInfo)> onUpdate, void *clickData)
		{
			type = UIElementType::UITYPE_TOGGLE;
			this->label = label;
			this->offset = offset;
			this->pos = glm::vec2(0);
			this->size = size;
			this->onClick = onClick;
			this->onUpdate = onUpdate;
			this->clickData = clickData;
			this->anchor = anchor;

			anchorPos();
		}
		~Toggle(){}

		void	draw();
		void	update(glm::vec2 mousePos, bool mousePressed);
		void	setClickData(void *data)
		{
			this->clickData = data;
		}
		void	click()
		{
			pressed = !pressed;
			if (onClick)
				this->onClick({clickData, pressed, id, label, this});
		}
		void	setOffset(glm::vec2 val)
		{
			this->offset = val;
		}
		glm::vec2	getSize(void)
		{
			return (this->size);
		}
		glm::vec2	getOffset(void)
		{
			return (this->offset);
		}

		bool						wasPressedInside = false;
		bool						previousMousePressed = false;
		bool						pressed = false;

		std::function<void(ToggleInfo)>	onClick = NULL;
		std::function<void(ToggleInfo)>	onUpdate = NULL;
		void						*clickData = NULL;

		std::string					label;

		Texture						*currentTexture = NULL;
};

#endif
