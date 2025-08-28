/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interface.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 22:01:22 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 12:27:21 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_HPP
# define INTERFACE_HPP

# include "libs.hpp"
# include "UIElement.hpp"

class	Interface
{
	public:
		Interface() {}
		~Interface()
		{
			for (auto &element : _elements)
				delete element.second;
		}

		void	clear()
		{
			for (auto &element : _elements)
				delete element.second;
			_elements.clear();
		}

		void	draw()
		{
			if (_onDraw)
				_onDraw(this);

			for (auto &element : _elements)
				element.second->draw();
		}
		void	update();

		void	setUpdateFunc(std::function<void(Interface*)> func)
		{
			this->_onUpdate = func;
		}
		void	setDrawFunc(std::function<void(Interface*)> func)
		{
			this->_onDraw = func;
		}

		UIElement	*addElement(const std::string &key, UIElement *elem);
		UIElement	*getElement(const std::string &key);
		void	charInput(unsigned int key)
		{
			UIElement	*elementPTR = NULL;

			for (auto &element : _elements)
			{
				elementPTR = element.second;
				if (elementPTR->type == UIElementType::UITYPE_TEXTBOX)
					static_cast<TextBox*>(elementPTR)->charInput(key);
			}
		}
		void	specialInput(int key, int action)
		{
			UIElement	*elementPTR = NULL;

			for (auto &element : _elements)
			{
				elementPTR = element.second;
				if (elementPTR->type == UIElementType::UITYPE_TEXTBOX)
					static_cast<TextBox*>(elementPTR)->specialInput(key, action);
			}
		}
	private:
		std::map<std::string, UIElement *>		_elements;
		std::function<void(Interface*)>			_onUpdate = NULL;
		std::function<void(Interface*)>			_onDraw = NULL;
};

#endif
