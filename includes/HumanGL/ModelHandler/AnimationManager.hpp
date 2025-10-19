/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnimationManager.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:45:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/19 09:23:02 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATIONMANAGER_HPP
# define ANIMATIONMANAGER_HPP

# include "libs.hpp"
# include "Timeline.hpp"
# include "Model.hpp"

# include "Animation.hpp"

/*

	Stores Animations, key of the map is the name of the file

*/
class	AnimationManager
{
	public:
		AnimationManager() {}
		~AnimationManager() {}

		void	load(const std::string &id, const std::string &path)
		{
			_animations.insert({id, {new Animation(), new Model()}});
			_animations[id].first->load(path, _animations[id].second);
		}
		//Plays the animation to the model
		void	play(const std::string &id)
		{
			std::map<std::string, Part*>	&parts = _animations[id].second->getParts();
			_current = _animations[id].first;

			for (auto part : parts)
			{
				part.second->setTimeline(_current->get(part.second->id()));
				std::cout << part.second->id() << " changed timeline" << std::endl;
			}
			std::cout << "Now playing " << id << std::endl;
		}

		Model	*getAnimationModel(const std::string &id)
		{
			return (_animations[id].second);
		}
		Animation	*getAnimation(const std::string &id)
		{
			return (_animations[id].first);
		}
	private:
		std::map<std::string, std::pair<Animation*, Model*> >	_animations;
		Animation												*_current = nullptr;
};

#endif
