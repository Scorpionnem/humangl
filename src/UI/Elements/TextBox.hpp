/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextBox.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:17:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/22 12:28:14 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTBOX_HPP
# define TEXTBOX_HPP

# include "UIElement.hpp"

struct	TextBoxInfo
{
	void				*data;
	const std::string	input;
	const std::string	id;
};

class	TextBox : public UIElement
{
	public:
		TextBox(UIAnchor anchor, std::string label, glm::vec2 offset, glm::vec2 size, std::function<void(TextBoxInfo)> onClick, void *clickData)
		{
			type = UIElementType::UITYPE_TEXTBOX;
			this->offset = offset;
			this->pos = glm::vec2(0);
			this->size = size;
			this->onClick = onClick;
			this->clickData = clickData;
			this->anchor = anchor;
			this->label = label;

			anchorPos();
		}
		~TextBox(){}

		void	draw();
		void	update(glm::vec2 mousePos, bool mousePressed);
		void	setClickData(void *data)
		{
			this->clickData = data;
		}
		void	charInput(unsigned int key)
		{
			(void)key;
			if (!pressed)
				return ;

			if (key >= 32 && key <= 126)
				input.insert(input.end(), 1, (char)key);
		}
		void	specialInput(int key, int action)
		{
			(void)key;(void)action;
			if (!pressed || (action != GLFW_PRESS && action != GLFW_REPEAT))
				return ;

			if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_ENTER)
				validate();
			if (key == GLFW_KEY_BACKSPACE && input.size() > 0)
				input.pop_back();
		}
		void	validate()
		{
			pressed = false;
			if (onClick)
				this->onClick({clickData, input, id});
		}
		std::string	getInput()
		{
			return (this->input);
		}
		void	clear()
		{
			input.clear();
		}
		void	putString(std::string str);

		bool						wasPressedInside = false;
		bool						previousMousePressed = false;
		bool						pressed = false;
		std::string					input;
		std::string					label;

		std::function<void(TextBoxInfo)>	onClick = NULL;
		void						*clickData = NULL;

		Texture						*currentTexture = NULL;
};

#endif
