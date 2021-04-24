#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <cmath>

class Bullet
{
public:
	Bullet(sf::Vector2f nPosition, float nSpeed, float nRange, float nDamage, sf::Vector2i nDirection, sf::Texture nTexture)
	{
		position = nPosition;

	}

protected:
	sf::Vector2f position;
	float speed;
	float range;
	float damage;
	sf::Vector2i direction;

	sf::Texture texture;
	sf::Sprite sprite;
};
