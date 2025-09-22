/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:11:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/22 13:15:15 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP

# include "libs.hpp"
# include "Shader.hpp"
# include "Font.hpp"

# define DEFAULT_WINDOW_WIDTH 1290
# define DEFAULT_WINDOW_HEIGHT 780

class	Window
{
	public:
		Window();
		~Window();
		bool	up(void);
		void		loopStart(void);
		void		loopEnd();
		void		setIcon(const char *path);
		void		center();
		void		close();
		
		GLFWwindow	*data(void);
		float		getDeltaTime(void);
		void		setLastMouseX(float val){this->lastMouseX = val;}
		void		setLastMouseY(float val){this->lastMouseY = val;}
		float		getLastMouseX(){return (this->lastMouseX);}
		float		getLastMouseY(){return (this->lastMouseY);}
		void		setDefaultMousePos();

		void		setWidth(float val){_width = val;}
		void		setHeight(float val){_height = val;}
		float		getWidth(){return (this->_width);}
		float		getHeight(){return (this->_height);}
	private:
		float		_width;
		float		_height;

		float		_deltaTime;
		float		_lastFrame;
		float		_currentFrame;
		GLFWwindow	*_windowData;
		float		lastMouseX = 0;
		float		lastMouseY = 0;
};

#endif
