/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Toggle.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:12:17 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/18 15:16:26 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Toggle.hpp"
#include "Engine.hpp"

void	Toggle::update(glm::vec2 mousePos, bool mousePressed)
{
	bool inside = isInside(this->pos, this->size, mousePos);

	if (mousePressed && !inside && pressed)
		click();

	if (pressed)
		this->currentTexture = Engine::Textures->get(COBBLESTONE_TEXTURE_PATH);
	else
		this->currentTexture = Engine::Textures->get(STONE_TEXTURE_PATH);

	if (this->anchor != UIAnchor::UI_NONE)
		anchorPos();

	if (mousePressed && !this->previousMousePressed)
		this->wasPressedInside = inside;

	else
	{
		if (this->wasPressedInside && inside)
			click();
		this->wasPressedInside = false;
	}

	this->previousMousePressed = mousePressed;
}

void	Toggle::draw()
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

	float	labelWidth = this->label.size() * 15;
	float	labelHeight = 15;

	glm::vec2	buttonCenter;
	buttonCenter.x = (this->pos.x + this->size.x / 2.f) - labelWidth / 2.f;
	buttonCenter.y = (this->pos.y + this->size.y / 2.f) - labelHeight / 2.f;

	Engine::Font->putString(this->label, buttonCenter, glm::vec2(1, 1), glm::vec3(1), false, false);
}
