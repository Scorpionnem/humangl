/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 22:29:09 by mbirou            #+#    #+#             */
/*   Updated: 2025/09/13 15:51:27 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libs.hpp>
#include <Part.hpp>

# include "AnimationManager.hpp"

class Model
{
	public:
		Model() {}
		~Model() {}
	
		void	render();
		void	update();
	private:
		std::vector<Part>	_parts;
		AnimationManager	_animations;
};
