#include "Rig.hpp"

void	Rig::update(float delta)
{
	ImGui::Begin("RIG");
		_root->update(delta, Mat4f(1.0));
	ImGui::End();

	for (auto p : _parts)
	{
		std::string	req = p.second->children_request();
		if (_parts.find(req) != _parts.end())
			std::cerr << "DUPLICATE ID" << std::endl;
		else if (!req.empty())
		{
			add_part(std::make_shared<Part>(req));
			p.second->addChild(_parts[req]);
		}
	}
}

void	Rig::load(const std::string &path)
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
			current->setColor(Vec3f(x, y, z));
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

void	Rig::export_to(const std::string &path)
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
