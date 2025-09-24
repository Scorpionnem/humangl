/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:29:09 by mbirou            #+#    #+#             */
/*   Updated: 2025/09/24 10:02:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_HPP
# define MODEL_HPP

#include <libs.hpp>
#include <Part.hpp>

/*

	How it would work:

	Model can load an animation, when loading, it checks that the model has all the parts.
		(It just needs the ids to be the same, it can have more objects than in the animation in this case, parts will not move)
	In the animation, all timelines are stored in a map [object, timeline]
	
	To export the file, it would be from the model, since each part has access to the animation manager of its model, it can just export from the root node.

*/

class Model
{
	public:
		Model() {}
		~Model() {}
	
		void	draw()
		{
			if (!_root)
				std::cout << "ERROR ROOT IS NULL, id: " << std::endl;
			_root->draw();
		}
		void	selectDraw()
		{
			if (!_root)
				std::cout << "ERROR ROOT IS NULL, id: " << std::endl;
			_root->selectDraw();
		}
		Part	*getSelected(const glm::vec3 &idColor)
		{
			for (auto part : _parts)
			{
				if (part.second->getIdColor() == idColor)
					return (part.second);
			}
			return (NULL);
		}
		void	update()
		{
			_root->update(glm::mat4(1.0f));
		}
		// Will load the model
		// void	load(const std::string &id, const std::string &path)
		// {
		// 	_animations.load(id, path);
		// }
		std::map<std::string, Part*>	&getParts()
		{
			return (_parts);
		}
		Part	*getPart(const std::string &name)
		{
			std::map<std::string, Part*>::iterator part = _parts.find(name);
			if (part == _parts.end())
				return (NULL);
			return (part->second);
		}
		void	setRoot(Part *root)
		{
			_root = root;
		}
		void	setRoot(const std::string &id)
		{
			_root = _parts[id];
			// if (!_root)
			std::cout << "ERROR ROOT IS NULL, id: " << id << std::endl;
		}
		void	addPart(Part *part)
		{
			_parts.insert({part->id(), part});
		}
		void	setChild(const std::string &parentID, Part *part)
		{
			if (!part)
				return ; // needs an error
			_parts[parentID]->addChild(part);
			addPart(part);
		}
		void	setChild(const std::string &parentID, const std::string &childID)
		{
			_parts[parentID]->addChild(_parts[childID]);
		}
		void	exportAnimation(const std::string &path)
		{
			if (_root)
				_root->exportAnimation(path, _root);
		}
	private:
		std::map<std::string, Part*>	_parts;
		Part							*_root;
};

#endif
