/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Image.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:21:08 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 21:59:29 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Image.hpp"
#include "Engine.hpp"

void	Image::update(glm::vec2, bool)
{
	this->currentTexture = Engine::Textures->get(TITLE_TEXTURE_PATH);;
	if (this->anchor != UIAnchor::UI_NONE)
		anchorPos();
}
