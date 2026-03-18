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

		void	update(float delta)
		{
			_root->update(delta, Mat4f(1.0));
		}
		void	draw(Shader &shader)
		{
			_root->draw(shader);
		}
		void	set_root(std::shared_ptr<Part> part)
		{
			_root = part;
		}
		void	load(const std::string &path)
		{
			std::ifstream	file;
			std::string		fileLine;

			file.open(path);
			if (!file.is_open())
				throw std::runtime_error("Could not open file");

			std::shared_ptr<Part>	current = NULL;
			while (std::getline(file, fileLine))
			{
				std::istringstream	line(fileLine);
				std::string			word;

				line >> word;
				if (word == "define")
				{
					std::string	object;
					std::string	path;
					if (!(line >> object))
						throw std::runtime_error("define: No argument given");

					add_part(std::make_shared<Part>(object));
				}
				else if (word == "object")
				{
					std::string	object;
					if (!(line >> object))
						throw std::runtime_error("object: No argument given");

					current = _parts[object];
				}
				else if (word == "root")
				{
					std::string	object;
					if (!(line >> object))
						throw std::runtime_error("root: No argument given");

					if (!_root)
						set_root(_parts[object]);
				}
				else if (word == "children")
				{
					std::string	object;
					if (!(line >> object))
						throw std::runtime_error("children: No argument given");

					if (!current)
						throw std::runtime_error("children: Object not set");
					current->addChild(_parts[object]);
				}
				else if (word == "color")
				{
					float	x;
					float	y;
					float	z;

					if (!(line >> x >> y >> z))
						throw std::runtime_error("color: Failed to parse");
					current->setColor(Vec3f(x, y, z) / 255.0f);
				}
				else if (word == "banchor")
				{
					float	x;
					float	y;
					float	z;

					if (!(line >> x >> y >> z))
						throw std::runtime_error("banchor: Failed to parse");
					current->setBaseAnchor(Vec3f(x, y, z));
				}
				else if (word == "panchor")
				{
					float	x;
					float	y;
					float	z;

					if (!(line >> x >> y >> z))
						throw std::runtime_error("panchor: Failed to parse");
					current->setPointAnchor(Vec3f(x, y, z));
				}
				else if (word == "kft")
				{
					float	time;
					float	x;
					float	y;
					float	z;

					if (!(line >> time >> x >> y >> z))
						throw std::runtime_error("kf*: Failed to parse");
					if (!current)
						throw std::runtime_error("no defined object");
					current->timeline.addTranslation(Keyframe(time, Vec3f(x, y, z)));
				}
				else if (word == "kfr")
				{
					float	time;
					float	x;
					float	y;
					float	z;

					if (!(line >> time >> x >> y >> z))
						throw std::runtime_error("kf*: Failed to parse");
					if (!current)
						throw std::runtime_error("no defined object");
					current->timeline.addRotation(Keyframe(time, Vec3f(x, y, z)));
				}
				else if (word == "kfs")
				{
					float	time;
					float	x;
					float	y;
					float	z;

					if (!(line >> time >> x >> y >> z))
						throw std::runtime_error("kf*: Failed to parse");
					if (!current)
						throw std::runtime_error("no defined object");
					current->timeline.addScale(Keyframe(time, Vec3f(x, y, z)));
				}
			}
			(void)path;
			(void)_root;
		}
		void	add_part(std::shared_ptr<Part> part)
		{
			_parts.insert({part->id(), part});
		}
		void	export_to(const std::string &path)
		{
			if (!_root)
				throw std::runtime_error("No root in rig");

			std::ofstream	file;

			file.open(path);
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + path);

			file << HGL_HEADER;

			_root->export_define(file);

			file << "root " << _root->id() << "\n\n";

			_root->export_object(file);
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<Part>>	_parts;
		std::shared_ptr<Part>									_root = NULL;
};
