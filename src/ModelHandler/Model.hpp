/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:29:09 by mbirou            #+#    #+#             */
/*   Updated: 2025/09/14 14:12:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libs.hpp>
#include <Part.hpp>

# include "AnimationManager.hpp"

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
	
		void	render();
		void	update();
		void	load(const std::string &id, const std::string &path)
		{
			_animations.load(id, path);
		}
	private:
		std::vector<Part>	_parts;
		AnimationManager	_animations;
};
