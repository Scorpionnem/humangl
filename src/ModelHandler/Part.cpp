/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 19:43:35 by mbirou            #+#    #+#             */
/*   Updated: 2025/09/02 20:10:05 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Part.hpp>

Part::Part(const Timeline &timeLine, const glm::vec3 &anchor, const glm::vec3 &color)
{
	_timeLine = timeLine;
	_anchor = 1.0f - anchor;
	_color = color;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Part::defaultCube.size() * sizeof(float), Part::defaultCube.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

Part::Part()
{}

Part::~Part()
{}


void	Part::addChild(const Part &child)
{
	_children.push_back(child);
}

void	Part::updateAnchor(const glm::mat4 &parentMat)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(parentMat, scale, rotation, translation, skew, perspective);

	// _mat = glm::rotate(_mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	// _mat = glm::rotate(_mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	// _mat = glm::rotate(_mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	_mat = glm::scale(_mat, 1.0f / scale);
	_mat = glm::translate(_mat, -_anchor * scale + scale / 2.0f - translation); //-_anchor * scale + 
}

void	Part::update(const glm::mat4 &parentMat)
{
	_timeLine.update(Engine::Window->getDeltaTime());

	_mat = parentMat;
	updateAnchor(parentMat);

	glm::vec3	translation = _timeLine.getTranslation();
	glm::vec3	rotation = _timeLine.getRotation();
	glm::vec3	scale = _timeLine.getScale();

	_mat = glm::translate(_mat, translation);// - glm::vec3(1.0f, 1.0f, 1.0f)
	_mat = glm::rotate(_mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	_mat = glm::rotate(_mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	_mat = glm::rotate(_mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	_mat = glm::scale(_mat, scale);

	for (Part &part : _children)
		part.update(_mat);
}

void	Part::draw()
{
	Engine::Shaders->get("cube")->setMat4("model", _mat);
	Engine::Shaders->get("cube")->setVec3("color", _color);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	for (Part &part : _children)
		part.draw();
}

std::vector<float> Part::defaultCube = {
	// front face
	-0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,

	0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	// back face
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,

	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	// left face
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	// right face
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,

	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,

	// bottom face
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f, -0.5f,

	0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,

	// top face
	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,

	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};