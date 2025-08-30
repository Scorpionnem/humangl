/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Image.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:43:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 10:37:44 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_HPP
# define IMAGE_HPP

# include "UIElement.hpp"

class	Image : public UIElement
{
	public:
		Image(UIAnchor anchor, glm::vec2 offset, glm::vec2 size)
		{
			type = UIElementType::UITYPE_IMAGE;
			this->offset = offset;
			this->pos = glm::vec2(0);
			this->size = size;
			this->anchor = anchor;

			anchorPos();
		}
		~Image(){}

		void	draw();
		void	update(glm::vec2, bool);

		bool						wasPressedInside = false;
		bool						previousMousePressed = false;

		Texture						*currentTexture = NULL;
};

#endif
