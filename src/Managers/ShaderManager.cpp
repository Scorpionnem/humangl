/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 10:23:08 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/28 22:01:59 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShaderManager.hpp"
#include "Engine.hpp"

ShaderManager::ShaderManager()
{
}

void	ShaderManager::setup()
{
	Engine::log("Building shaders...", LogSeverity::NORMAL);
	Shader *textShader = Engine::Shaders->load({"text", TEXT_VERT_SHADER_PATH, TEXT_FRAG_SHADER_PATH});
	Shader *skyboxShader = Engine::Shaders->load({"skybox", SKYBOX_VERT_SHADER_PATH, SKYBOX_FRAG_SHADER_PATH});
	Shader *postShader = Engine::Shaders->load({"post", POST_VERT_SHADER_PATH, POST_FRAG_SHADER_PATH});
	Shader *guiShader = Engine::Shaders->load({"gui", GUI_VERT_SHADER_PATH, GUI_FRAG_SHADER_PATH});
	Shader *titleBackground = Engine::Shaders->load({"title_bg", BACKGROUND_VERT_SHADER_PATH, BACKGROUND_FRAG_SHADER_PATH});
	Engine::Shaders->load({"colored_quad", "shaders/coloredquad.vs", "shaders/coloredquad.fs"});

	guiShader->setInt("tex0", 0);

	Texture::use("tex0", 0, 0, textShader);

	Texture::use("skybox", 0, 0, skyboxShader);

	Texture::use("screenTexture", 0, 0, postShader);
	Texture::use("depthTex", 0, 1, postShader);

	Texture::use("screenTexture", 0, 0, titleBackground);

	Engine::log("Finished building shaders", LogSeverity::SUCCESS);
}

ShaderManager::~ShaderManager()
{
	for (auto it = shaders.begin(); it != shaders.end(); it++)
		delete it->second;
}

bool	ShaderManager::erase(const std::string &name)
{
	std::unordered_map<std::string, Shader *>::iterator	finder;
	finder = shaders.find(name);
	if (finder == shaders.end())
	{
		Engine::log("WARNING Tried to unload a shader thats not loaded: " + name, LogSeverity::WARNING);
		return (0);
	}
	shaders.erase(finder);
	return (1);
}

Shader	*ShaderManager::load(ShaderInfos infos)
{
	if (shaders.find(infos.name) != shaders.end())
	{
		Engine::log("WARNING Tried to load a shader thats already loaded (will be using the existing shader): " + infos.name, LogSeverity::WARNING);
		return (this->get(infos.name));
	}
	return (shaders.insert(std::make_pair(infos.name, new Shader(infos.vertex, infos.fragment))).first->second);
}

Shader	*ShaderManager::get(const std::string &name)
{
	std::unordered_map<std::string, Shader *>::iterator	finder = shaders.find(name);
	if (finder == shaders.end())
	{
		Engine::log("ERROR Tried to access a shader thats not loaded, might cause a crash: " + name, LogSeverity::ERROR);
		return (NULL);
	}
	return (finder->second);
}

Shader	*ShaderManager::operator[](const std::string &name)
{
	return (this->get(name));
}
