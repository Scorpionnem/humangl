/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Image.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:21:08 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/20 16:44:55 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Image.hpp"
#include "Engine.hpp"

Image::Image(UIAnchor anchor, glm::vec2 offset, glm::vec2 size, std::string path)
{
	type = UIElementType::UITYPE_IMAGE;
	this->offset = offset;
	this->pos = glm::vec2(0);
	this->size = size;
	this->anchor = anchor;

	this->currentTexture = Engine::Textures->get(path);

	anchorPos();
}

void	Image::update(glm::vec2, bool)
{
	if (this->anchor != UIAnchor::UI_NONE)
		anchorPos();
}

void	Image::draw()
{
	if (!this->currentTexture)
		return ;

	initButtonQuad();

	if (this->anchor != UIAnchor::UI_NONE)
		anchorPos();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(this->pos.x, this->pos.y, 0.0f));
	model = glm::scale(model, glm::vec3(this->size.x, this->size.y, 1.0f));
	glm::mat4 projection = glm::ortho(0.0f, Engine::Window->getWidth(), Engine::Window->getHeight(), 0.0f);

	_shader->use();
	_shader->setMat4("model", model);
	_shader->setMat4("projection", projection);

	this->currentTexture->use(0);

	glBindVertexArray(UIquadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
