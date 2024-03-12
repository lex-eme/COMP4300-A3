#include "Assets.h"

#include <iostream>

void Assets::loadFromFile(const std::string& path)
{
}

void Assets::addTexture(const std::string& name, const std::string& path)
{
	if (m_Textures.find(name) != m_Textures.end())
	{
		std::cout << "Texture '" << name << "' already exists" << std::endl;
		return;
	}

	if (!m_Textures[name].loadFromFile(path))
	{
		std::cerr << "Could not load texture '" << name << "' from path '" << path << "'" << std::endl;
	}
}

void Assets::addAnimation(const std::string& name, Animation& anim)
{
	if (m_Animations.find(name) != m_Animations.end())
	{
		std::cout << "Animation '" << name << "' already exists" << std::endl;
		return;
	}

	m_Animations[name] = anim;
}

void Assets::addSound(const std::string& name, const std::string& path)
{
}

void Assets::addFont(const std::string& name, const std::string& path)
{
	if (m_Fonts.find(name) != m_Fonts.end())
	{
		std::cout << "Font '" << name << "' already exists" << std::endl;
		return;
	}

	if (!m_Fonts[name].loadFromFile(path))
	{
		std::cerr << "Could not load font '" << name << "' from path '" << path << "'" << std::endl;
	}
}

const sf::Texture& Assets::getTexture(const std::string& name) const
{
	auto it = m_Textures.find(name);
	if (it != m_Textures.end())
	{
		return it->second;
	}

	std::cerr << "Trying to use unknown texture called " << name << std::endl;
	exit(-1);
}

const Animation& Assets::getAnimation(const std::string& name) const
{
	auto it = m_Animations.find(name);
	if (it != m_Animations.end())
	{
		return it->second;
	}

	std::cerr << "Trying to use unknown animation called " << name << std::endl;
	exit(-1);
}

const sf::Sound& Assets::getSound(const std::string& name) const
{
	auto it = m_Sounds.find(name);
	if (it != m_Sounds.end())
	{
		return it->second;
	}

	std::cerr << "Trying to use unknown sound called " << name << std::endl;
	exit(-1);
}

const sf::Font& Assets::getFont(const std::string& name) const
{
	auto it = m_Fonts.find(name);
	if (it != m_Fonts.end())
	{
		return it->second;
	}

	std::cerr << "Trying to use unknown font called " << name << std::endl;
	exit(-1);
}
