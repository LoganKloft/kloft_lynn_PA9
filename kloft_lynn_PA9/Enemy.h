#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

class Enemy : public sf::Sprite
{
public:
	Enemy(const sf::Vector2f& location, const sf::Texture& texture);
private:
	int health;
	int damage;
	sf::Texture m_texture;
};