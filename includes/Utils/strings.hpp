/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:23:39 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/20 12:57:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGS_HPP
# define STRINGS_HPP

# define TX_PATH_ASCII "assets/textures/font/ascii.png"
# define TX_PATH_DIRT "assets/textures/block/dirt.png"

# define TX_PATH_BUTTON_SMALL "assets/textures/ui/button_small.png"
# define TX_PATH_BUTTON_SMALL_HIGHLIGHTED "assets/textures/ui/button_highlighted_small.png"
# define TX_PATH_BUTTON_SMALL_DISABLED "assets/textures/ui/button_disabled_small.png"

# define TX_PATH_BUTTON "assets/textures/ui/button.png"
# define TX_PATH_BUTTON_HIGHLIGHTED "assets/textures/ui/button_highlighted.png"
# define TX_PATH_BUTTON_DISABLED "assets/textures/ui/button_disabled.png"

# define TX_PATH_FT_HUMANGL "assets/textures/ui/humangl.png"

# define TX_PATH_SLIDER "assets/textures/ui/slider.png"
# define TX_PATH_SLIDER_HANDLE "assets/textures/ui/slider_handle.png"
# define TX_PATH_SLIDER_HANDLE_HIGHLIGHTED "assets/textures/ui/slider_handle_highlighted.png"

# define TX_PATH_TEXT_FIELD "assets/textures/ui/text_field.png"
# define TX_PATH_TEXT_FIELD_HIGHLIGHTED "assets/textures/ui/text_field_highlighted.png"

# define EXPORTS_PATH "./exports/"
# define HGL_FORMAT ".hgl"

# define HGL_HEADER "# 42 humangl project export file by mbatty & mbirou\n\
# keys used in the file\n\
#\n\
#	Order is important, if you use a key that needs a used object, an error will be throwed!\n\
#\n\
#	define : string name\n\
#		-Defines an object to be used in the file\n\
#	root : string name\n\
#		-Sets the root object\n\
#\n\
#	object : string name\n\
#		-Sets the object as used\n\
#\n\
#	children : string name\n\
#		-Sets the children of the used object\n\
#\n\
#	banchor : vec3\n\
#		-Sets the base anchor of the used object\n\
#	panchor : vec3\n\
#		-Sets the point anchor of the used object\n\
#	color : vec3\n\
#		-Sets the color of the used object\n\
#\n\
#	kft : float time : vec3 translation\n\
#		-Sets the translation values to the used object\n\
#	kfr : float time : vec3 rotation\n\
#		-Sets the rotation values to the used object\n\
#	kfs : float time : vec3 scale\n\
#		-Sets the scale values to the used object\n\
"

#endif
