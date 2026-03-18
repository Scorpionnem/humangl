#pragma once

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
#		-Sets the scale values to the used object\n\n\
"

#include "Part.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <memory>

class	Rig
{
	public:
		Rig() {}
		~Rig() {}

		void	update(float delta);
		void	draw(Shader &shader)
		{
			_root->draw(shader);
		}
		void	set_root(std::shared_ptr<Part> part)
		{
			_root = part;
		}
		void	load(const std::string &path);
		void	add_part(std::shared_ptr<Part> part)
		{
			_parts.insert({part->id(), part});
		}
		void	export_to(const std::string &path);
	private:
		std::unordered_map<std::string, std::shared_ptr<Part>>	_parts;
		std::shared_ptr<Part>									_root = NULL;
};
