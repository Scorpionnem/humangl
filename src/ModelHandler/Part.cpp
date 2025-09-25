/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 19:43:35 by mbirou            #+#    #+#             */
/*   Updated: 2025/09/25 13:50:20 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Part.hpp>

unsigned int	Part::_index = 1;

Part::Part(std::string id)
{
	_pointAnchor = glm::vec3(0);
	_baseAnchor = glm::vec3(0);
	_color = glm::vec3(0);
	_id = id;
	_idColor = glm::vec3((float)((_index / 255255) % 255), (float)((_index / 255) % 255), (float)(_index % 255));
	_index ++;
	_mat = glm::mat4(1.0f);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Part::defaultCube.size() * sizeof(float), Part::defaultCube.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

Part::~Part()
{
}

glm::vec3	Part::getPointAnchor() const
{
	return (_pointAnchor);
}

glm::vec3	Part::getIdColor() const
{
	return (_idColor);
}

void	Part::addChild(Part *child)
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

	_mat = glm::scale(_mat, 1.0f / scale);
	_mat = glm::translate(_mat, _pointAnchor * (scale / 2.0f));
}

void	Part::update(const glm::mat4 &parentMat)
{
	if (!_timeline)
		return ;
	_timeline->update(Engine::Window->getDeltaTime());

	_mat = parentMat;
	updateAnchor(parentMat);

	glm::vec3	translation = _timeline->getValue(KeyFrameType::TRANSLATION);
	glm::vec3	rotation = _timeline->getValue(KeyFrameType::ROTATION);
	glm::vec3	scale = _timeline->getValue(KeyFrameType::SCALE);

	_mat = glm::translate(_mat, translation - _baseAnchor * (scale / 2.0f));
	_mat = glm::translate(_mat, (_baseAnchor / 2.0f) * (scale));
	_mat = glm::rotate(_mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	_mat = glm::rotate(_mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	_mat = glm::rotate(_mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	_mat = glm::translate(_mat, -_baseAnchor * (scale / 2.0f));
	_mat = glm::scale(_mat, scale);

	for (Part *part : _children)
		part->update(_mat);
}

void	Part::selectDraw()
{
	Engine::Shaders->get("cube")->setMat4("model", _mat);
	Engine::Shaders->get("cube")->setVec3("color", _idColor / 255.0f);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	for (Part *part : _children)
		part->selectDraw();
}

void	Part::draw()
{
	Engine::Shaders->get("cube")->setMat4("model", _mat);
	Engine::Shaders->get("cube")->setVec3("color", _color);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	for (Part *part : _children)
		part->draw();
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

void	Part::exportAnimation(std::string path, Part *root)
{
	std::ofstream	file;

	file.open(path);
	if (!file.is_open())
		return ;

	_addHeader(file);

	_defineExport(file);
	file << std::endl;
	
	if (root)
		file << "root " << root->_id << std::endl;
	file << std::endl;

	_exportObject(file);
}

void	Part::_addHeader(std::ofstream &file)
{
	file << HGL_HEADER << std::endl;
}

void	Part::_exportObject(std::ofstream &file)
{
	std::cout << "object " << _id << std::endl;

	file << "object " << _id << std::endl;
	_childrenExport(file);

	file << "banchor " << _baseAnchor.x << " " << _baseAnchor.y << " " << _baseAnchor.z << std::endl;
	file << "panchor " << _pointAnchor.x << " " << _pointAnchor.y << " " << _pointAnchor.z << std::endl;
	file << "color " << _color.x * 255.f << " " << _color.y * 255.f << " " << _color.z * 255.f << std::endl;

	_timeline->exportTimeline(file);
	file << std::endl;

	for (auto &part : _children)
		part->_exportObject(file);
}

void	Part::_defineExport(std::ofstream &file)
{
	for (auto &part : _children)
		part->_defineExport(file);
	file << "define " << _id << std::endl;
}

void	Part::_childrenExport(std::ofstream &file)
{
	for (auto &part : _children)
		file << "children " << part->_id << std::endl;
}
