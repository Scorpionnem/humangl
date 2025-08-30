
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:57:06 by mbatty            #+#    #+#             */
/*   Updated: 2025/07/17 11:00:29 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TitleScene_HPP
# define TitleScene_HPP

# include "Scene.hpp"

class	TitleScene : public Scene
{
	public:
		TitleScene();
		~TitleScene();

		bool	getDebug() {return (this->_debug);}
		void	setDebug(bool state) {this->_debug = state;}
	private:
		bool	_debug = false;
};

#endif
