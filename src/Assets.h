#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Animation.h"

class Assets
{
	std::unordered_map<std::string, sf::Texture>	m_Textures;
	std::unordered_map<std::string, Animation>		m_Animations;
	std::unordered_map<std::string, sf::Sound>		m_Sounds;
	std::unordered_map<std::string, sf::Font>		m_Fonts;

public:
	void loadFromFile(const std::string& path);

	void addTexture(const std::string& name, const std::string& path);
	void addAnimation(const std::string& name, Animation& anim);
	void addSound(const std::string& name, const std::string& path);
	void addFont(const std::string& name, const std::string& path);

	const sf::Texture& getTexture(const std::string& name) const;
	const Animation& getAnimation(const std::string& name) const;
	const sf::Sound& getSound(const std::string& name) const;
	const sf::Font& getFont(const std::string& name) const;
};

