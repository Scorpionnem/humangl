/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:51:55 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/30 11:01:41 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_HPP
# define ENGINE_HPP

#include "libs.hpp"
#include "Window.hpp"
#include "TextureManager.hpp"
#include "libs.hpp"
#include "Font.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "FrameBuffer.hpp"
#include "InterfaceManager.hpp"
#include "UIElement.hpp"
#include "Terminal.hpp"
#include "SceneManager.hpp"

enum	LogSeverity
{
	NORMAL,
	SUCCESS,
	WARNING,
	ERROR,
	MELTDOWN
};

#define YELLOW "\033[33m"
#define RED "\033[31m"
#define RED_FULL "\033[29;41m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

class	Engine
{
	public:
		static void				log(const std::string &str, LogSeverity severity);
		static void				log(const std::string &str);
		
		static void				init();
		static void				destroy();
		
		static Window			*Window;
		static ShaderManager	*Shaders;
		static TextureManager	*Textures;
		static Font				*Font;
		static SceneManager		*Scenes;
		static FrameBuffer		*FrameBuffer;

		static void	keyboardKeyHook(GLFWwindow *window, int key, int, int action, int);
		static void	keyboardCharHook(GLFWwindow *, unsigned int key);
		static void	clickMouseHook(GLFWwindow*, int button, int action, int mods);
		static void scrollMouseHook(GLFWwindow *, double xoffset, double yoffset);
		static void	moveMouseHook(GLFWwindow*, double xpos, double ypos);

	private:
		Engine(){}
};

#endif
