/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/08/27 18:06:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LIBS_HPP
# define LIBS_HPP

# define DEBUG 0
# define WIN_NAME "humangl"

# define FULL_SCREEN 0

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>
# include <istream>
# include <vector>
# include <map>
# include <unordered_map>
# include <cmath>
# include <unistd.h>
# include <exception>
# include <functional>
# include <array>
# include <algorithm>
# include <deque>
# include <memory>
# include <csignal>
# include <thread>
# include <mutex>
# include <atomic>
# include <iomanip>
# include <fstream>
# include <iostream>
# include <filesystem>

# include "strings.hpp"

# include "glad.h"
# include "glfw3.h"

# include "glm.hpp"
# include "gtc/matrix_transform.hpp"
# include "gtc/type_ptr.hpp"

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

inline void	consoleLog(std::string str, LogSeverity severity)
{
	switch (severity)
	{
		case NORMAL:
			std::cout << "[" << glfwGetTime() << "] " << str << std::endl;
			break ;
		case SUCCESS:
			std::cout << GREEN << "[" << glfwGetTime() << "] " << str << RESET << std::endl;
			break ;
		case WARNING:
			std::cerr << YELLOW << "[" << glfwGetTime() << "] " << str << RESET << std::endl;
			break ;
		case ERROR:
			std::cerr << RED << "[" << glfwGetTime() << "] " << str << RESET << std::endl;
			break ;
		case MELTDOWN:
			std::cerr << RED_FULL << "[" << glfwGetTime() << "] " << str << RESET << std::endl;
			break ;
	}
}

inline void	consoleLog(std::string str)
{
	consoleLog(str, LogSeverity::NORMAL);
}

#endif
