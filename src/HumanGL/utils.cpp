/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 08:46:47 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/20 08:48:22 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

std::string	hgl::formatName(const std::string str)
{
	std::string	res;

	for (char c : str)
	{
		if (c == ' ')
			res += '_';
		else
			res += c;
	}
	return (res);
}
