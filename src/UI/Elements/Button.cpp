/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Button.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:21:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 10:35:13 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Button.hpp"
#include "Engine.hpp"

void	Button::update(glm::vec2 mousePos, bool mousePressed)
{
	bool inside = isInside(this->pos, this->size, mousePos);

	this->currentTexture = Engine::Textures->get(STONE_TEXTURE_PATH);;

	if (this->anchor != UIAnchor::UI_NONE)
		anchorPos();

	if (mousePressed && !this->previousMousePressed)
		this->wasPressedInside = inside;

	if (mousePressed)
	{
		if (inside && this->wasPressedInside)
			this->currentTexture = Engine::Textures->get(COBBLESTONE_TEXTURE_PATH);
	}
	else
	{
		if (this->wasPressedInside && inside && onClick)
			this->onClick({clickData, id, label, this});
		this->wasPressedInside = false;
	}

	this->previousMousePressed = mousePressed;
}

void	Button::draw()
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

	float	labelWidth = this->label.size() * 16;
	float	labelHeight = 16;

	glm::vec2	buttonCenter;
	buttonCenter.x = (this->pos.x + this->size.x / 2.f) - labelWidth / 2.f;
	buttonCenter.y = (this->pos.y + this->size.y / 2.f) - labelHeight / 2.f;

	Engine::Font->putString(this->label, buttonCenter, glm::vec2(1, 1), glm::vec3(1), false, false);
}
