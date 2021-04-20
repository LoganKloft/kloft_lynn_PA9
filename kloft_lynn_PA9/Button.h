#pragma once


#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>

class Button : public sf::IntRect, public sf::RectangleShape 
{
public:
	Button(const sf::Vector2f& size, const sf::Vector2f& pos, const std::string& texture) : sf::IntRect((int) pos.x, (int) pos.y, (int) size.x, (int) size.y), sf::RectangleShape(size)
	{
		if (!m_texture.loadFromFile(texture))
		{
			std::cout << "Failed to load " << texture << " in class Button" << std::endl;
		}
		this->setPosition(pos);
		this->setTexture(&m_texture);
	}
private:
	sf::Texture m_texture;
};