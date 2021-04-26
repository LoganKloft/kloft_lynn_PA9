#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <cmath>
#include <iostream>

#include "Enemy.h"
#include "LevelEditor.h"

class Bullet
{
public:
	Bullet(sf::Vector2f nPosition, float nSpeed, float nRange, float nDamage, int rotation, sf::Texture nTexture)
	{
		position = nPosition;
		speed = nSpeed;
		range = nRange;
		damage = nDamage;
		texture = nTexture;

		//calculate direction vector
		direction.y = sin((rotation-90) * 3.14159 / 180.0)*10*speed;
		direction.x = cos((rotation - 90) * 3.14159 / 180.0)*10*speed;

		sprite.setRotation(rotation);
		sprite.setPosition(position.x, position.y);
		sprite.setTexture(texture);
		sprite.setOrigin(32, 32);
	}

	bool needsRemoval()
	{
		bool success = false;

		if (range <= 0 || damage <= 0)
			success = true;
		if (position.x > WINDOW_WIDTH || position.x < 0)
			success = true;
		if (position.y > WINDOW_HEIGHT || position.y < 0)
			success = true;

		return success;
	}

	void update(std::vector<Enemy>& enemies, int &gold, int &enemiesDead, int &enemiesCurrentSize)
	{
		range -= sqrt(pow(direction.x, 2) + pow(direction.y, 2));		

		position.x += direction.x;
		position.y += direction.y;
		sprite.setPosition(position.x, position.y);

		//Collision
		for (int i = 0; i < enemiesCurrentSize; i++)
		{
			if (sqrt((pow(enemies[i].getPosition().x - position.x, 2))
				+ (pow(enemies[i].getPosition().y - position.y, 2))) <= 32)
			{
				while (damage > 0)
				{
					enemies[i].setHealth(enemies[i].getHealth() - 1);
					damage--;

					if (enemies[i].getHealth() <= 0)
					{
						int enemyGold = enemies[i].getGold();
						gold += enemyGold;
						enemiesDead++;
						enemiesCurrentSize--;
						enemies.erase(enemies.begin() + i);
						i--;
						break;
					}
				}
			}
		}
	}

	void renderBullet(sf::RenderWindow& cWindow) { cWindow.draw(sprite); }

protected:
	sf::Vector2f position;
	float speed;
	float range;
	float damage;
	sf::Vector2f direction;

	sf::Texture texture;
	sf::Sprite sprite;
};
