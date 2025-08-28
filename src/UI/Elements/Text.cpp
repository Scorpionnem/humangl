/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Text.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:15:41 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 21:59:42 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Text.hpp"
#include "Engine.hpp"

void	Text::draw()
{
	getSize();
	if (this->anchor != UIAnchor::UI_NONE)
		anchorPos();
	Engine::Font->putString(this->_label, glm::vec2(this->pos.x, this->pos.y), this->_scale, this->_color, _rotation, _angle, _drawBG, true);
}
