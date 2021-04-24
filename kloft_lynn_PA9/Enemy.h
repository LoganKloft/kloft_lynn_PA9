#pragma once

#include <iostream>
#include <queue>

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Level.h" // Gives access to constants

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

class Enemy : public sf::Sprite
{
public:
	Enemy()
	{
		health = 0;
		damage = 0;
		speed = 0;
	}

	Enemy(const sf::Texture& texture, int health, float damage, float speed, int gold) : sf::Sprite(texture)
	{
		setOrigin(32, 32);
		this->health = health;
		this->damage = damage;
		this->speed = speed;
		this->gold = gold;
		m_texture = texture;
	}

	Enemy(const Enemy& copy)
	{
		this->health = copy.getHealth();
		this->damage = copy.getDamate();
		this->speed = copy.getSpeed();
		this->m_texture = copy.getTexture();
		this->waypoints = copy.getWaypoints();
		this->setTexture(m_texture);
		this->setOrigin(copy.getOrigin());
		this->setPosition(copy.getPosition());
	}

	void setHealth(float health)
	{
		this->health = health;
	}
	void setDamage(float damage)
	{
		this->damage = damage;
	}
	void setSpeed(float speed)
	{
		this->speed = speed;
	}
	void setTexture(sf::Texture& texture)
	{
		m_texture = texture;
		((sf::Sprite*)this)->setTexture(m_texture);
	}
	void setGold(float gold)
	{
		this->gold = gold;
	}
	void setWaypoints(std::deque<sf::Vector2f> waypoints)
	{
		this->waypoints = waypoints;
	}

	// returns gold value of enemy if dead, 0 otherwise
	int hit(float damage)
	{
		health -= damage;
		if (health <= 0)
		{
			return die();
		}
		return 0;
	}

	// removes enemy from the screen and returns gold value of enemy
	int die()
	{
		waypoints.clear();
		setPosition(-32, -32);
		return gold;
	}

	// width should be 22
	// height should be 16
	// waypoints last element stores TILE_END or invalid tile / deadend
	bool calcWaypoints(int* tiles)
	{
		waypoints.clear();

		// starting tile location in tiles array
		sf::Vector2i start(-1, -1);

		// find starting location in tiles array
		bool flag = false;
		for (int i = 0; i < MAX_LEVEL_HEIGHT; i++)
		{
			for (int j = 0; j < MAX_LEVEL_WIDTH; j++)
			{
				if (tiles[MAX_LEVEL_WIDTH * i + j] == TILE_START)
				{
					flag = true;
					start.x = j;
					start.y = i;
					break;
				}
			}
			if (flag) break;
		}

		// check if did not find a starting point
		if (!flag)
		{
			std::cout << "Did not find a starting point for enemy" << std::endl;
			return false;
		}
		else
		{
			// add starting location to waypoints
			waypoints.push_back(sf::Vector2f(start.x * 64 + 32, start.y * 64 + 32));
			// move to starting location
			setPosition(waypoints.front());
		}

		// now search for end //
		int i = start.y, j = start.x;
		bool success = true;
		// - find first tile
		int previous = -1;
		bool cont = false;
		if (j + 1 < MAX_LEVEL_WIDTH)
		{
			int tileNumber = tiles[i * MAX_LEVEL_WIDTH + j + 1];
			if (tileNumber == TILE_HORIZONTAL || tileNumber == TILE_BOTTOMR_CORNER
				|| tileNumber == TILE_TOPR_CORNER)
			{
				j++;
				cont = false;
				previous = RIGHT;
				direction.x = 1;
				direction.y = 0;
			}
			else
			{
				cont = true;
			}
		}
		if (j - 1 >= 0 && cont)
		{
			int tileNumber = tiles[i * MAX_LEVEL_WIDTH + j - 1];
			if (tileNumber == TILE_HORIZONTAL || tileNumber == TILE_BOTTOML_CORNER
				|| tileNumber == TILE_TOPL_CORNER)
			{
				j--;
				cont = false;
				previous = LEFT;
				direction.x = -1;
				direction.y = 0;
			}
			else
			{
				cont = true;
			}
		}
		if (i + 1 < MAX_LEVEL_HEIGHT && cont)
		{
			int tileNumber = tiles[(i + 1) * MAX_LEVEL_WIDTH + j];
			if (tileNumber == TILE_VERTICAL || tileNumber == TILE_BOTTOMR_CORNER
				|| tileNumber == TILE_BOTTOML_CORNER)
			{
				i++;
				cont = false;
				previous = DOWN;
				direction.x = 0;
				direction.y = 1;
			}
			else
			{
				cont = true;
			}
		}
		if (cont)
		{
			int tileNumber = tiles[(i - 1) * MAX_LEVEL_WIDTH + j];
			if (tileNumber == TILE_VERTICAL || tileNumber == TILE_TOPR_CORNER
				|| tileNumber == TILE_TOPL_CORNER)
			{
				i--;
				previous = UP;
				direction.x = 0;
				direction.y = -1;
			}
			else
			{
				std::cout << "No connecting tile to green square" << std::endl;
				return false;
			}
		}

		while (tiles[i * MAX_LEVEL_WIDTH + j] != TILE_END)
		{
			if (i < 0 || j < 0 || i > MAX_LEVEL_HEIGHT || j > MAX_LEVEL_WIDTH)
			{
				success = false;
				break;
			}
			else
			{
				waypoints.push_back(sf::Vector2f(j * 64 + 32, i * 64 + 32));
			}

			switch (tiles[i * MAX_LEVEL_WIDTH + j])
			{
			case TILE_HORIZONTAL:
				if (previous == LEFT)
				{
					j--;
					previous = LEFT;
				}
				else if (previous == RIGHT)
				{
					j++;
					previous = RIGHT;
				}
				else
				{
					// came from illegal direction
					success = false;
				}
				break;
			case TILE_VERTICAL:
				if (previous == UP)
				{
					i--;
					previous = UP;
				}
				else if (previous == DOWN)
				{
					i++;
					previous = DOWN;
				}
				else
				{
					success = false;
				}
				break;
			case TILE_BOTTOML_CORNER:
				if (previous == DOWN)
				{
					j++;
					previous = RIGHT;
				}
				else if (previous == LEFT)
				{
					i--;
					previous = UP;
				}
				else
				{
					success = false;
				}
				break;
			case TILE_BOTTOMR_CORNER:
				if (previous == DOWN)
				{
					j--;
					previous = LEFT;
				}
				else if (previous == RIGHT)
				{
					i--;
					previous = UP;
				}
				else
				{
					success = false;
				}
				break;
			case TILE_TOPL_CORNER:
				if (previous == UP)
				{
					j++;
					previous = RIGHT;
				}
				else if (previous == LEFT)
				{
					i++;
					previous = DOWN;
				}
				else
				{
					success = false;
				}
				break;
			case TILE_TOPR_CORNER:
				if (previous == UP)
				{
					j--;
					previous = LEFT;
				}
				else if (previous == RIGHT)
				{
					i++;
					previous = DOWN;
				}
				break;
			case TILE_END:
				break;
			default:
				success = false;
				break;
			}
			if (!success)
			{
				std::cout << "Failed pathfinding" << std::endl;
				return false;
			}
		}
		waypoints.push_back(sf::Vector2f(j * 64 + 32, i * 64 + 32));
	}

	int getHealth() const
	{
		return health;
	}
	float getDamate() const
	{
		return damage;
	}
	float getSpeed() const
	{
		return speed;
	}
	int getGold() const
	{
		return gold;
	}
	bool isWaypointsEmpty() const
	{
		return waypoints.empty();
	}
	sf::Texture getTexture() const
	{
		return m_texture;
	}
	std::deque<sf::Vector2f> getWaypoints() const
	{
		return waypoints;
	}
	sf::Vector2f getDirection() const
	{
		return direction;
	}

	int move()
	{
		if (waypoints.empty())
		{
			std::cout << "Waypoints is empty" << std::endl;
			die();
			return damage;
		}

		if (getPosition() == waypoints.front())
		{
			waypoints.pop_front();

			if (waypoints.empty())
			{

				std::cout << "Waypoints is empty" << std::endl;
				die();
				return damage;
			}

			direction.x = (waypoints.front().x - getPosition().x) / 64;
			direction.y = (waypoints.front().y - getPosition().y) / 64;

			if (direction.x > 0) setRotation(0);
			else if (direction.x < 0) setRotation(180);
			else if (direction.y < 0) setRotation(270);
			else setRotation(90);
		}
		else
		{
			// check if past the waypoint, otherwise move
			if (direction.x > 0 && getPosition().x > waypoints.front().x)
			{
				setPosition(waypoints.front());
			}
			else if (direction.x < 0 && getPosition().x < waypoints.front().x)
			{
				setPosition(waypoints.front());
			}
			else if (direction.y > 0 && getPosition().y > waypoints.front().y)
			{
				setPosition(waypoints.front());
			}
			else if (direction.y < 0 && getPosition().y < waypoints.front().y)
			{
				setPosition(waypoints.front());
			}
			else
			{
				((sf::Sprite*)this)->move(direction.x * speed, direction.y * speed);
			}
		}
		return 0;
	}

	void printWaypoints()
	{
		for (int i = 0; i < waypoints.size(); i++)
		{
			std::cout << i << ": " << waypoints[i].x << " " << waypoints[i].y << std::endl;
		}
	}

private:
	int health;
	float damage;
	float speed;
	int gold;
	sf::Vector2f direction;
	sf::Texture m_texture;
	std::deque<sf::Vector2f> waypoints; // final element is either TILE_END or invalid tile / deadend
};