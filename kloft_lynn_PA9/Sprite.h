#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <fstream>
#include <string>

class Sprite : public sf::Sprite
{
public:
	Sprite(const std::string& texture, sf::Vector2f pos)
	{
		if (!m_texture.loadFromFile(texture))
		{
			std::cout << "Failed to open texture for sprite" << std::endl;
		}

		this->setTexture(m_texture);
		this->setPosition(pos);
		this->setOrigin(32, 32);
	}
private:
	sf::Texture m_texture;
};