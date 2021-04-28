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
	Button(const sf::Vector2f& hitbox_size, const sf::Vector2f& hitbox_pos, const sf::Vector2f& size, const sf::Vector2f& pos, const std::string& texture);
private:
	sf::Texture m_texture;
};