#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <cmath>


#include "Enemy.h"
#include "Bullet.h"

class base_tower
{
public:
	//Constructor
	base_tower(sf::Vector2i newPos) 
	{
		shootTimer = 0;
		rotation = 0;
		this->setPosition({newPos.x,newPos.y});
		//bulletSprite.setOrigin(32, 32);
		towerSprite.setOrigin(32, 32);
	}

	//Getters and Setters
	int getPrice() { return sellPrice; }
	float getDamage() { return damage; }
	float getRange() { return range; }
	sf::Sprite getTowerSprite() { return towerSprite; }
	//sf::Sprite getBulletSprite() { return bulletSprite; }
	sf::Vector2i getPosition() { return { 32 + 64 * position.x ,32 + 64 * position.y }; }
	int getRotation() { return rotation; }

	void setPrice(int nSellPrice) { sellPrice = nSellPrice; }
	void setDamage(float nDamage) { damage = nDamage; }
	void setRange(float nRange) { range = nRange; }
	void setTowerSprite(sf::Sprite nTowerSprite) { towerSprite = nTowerSprite; }
	//void setBulletSprite(sf::Sprite nBulletSprite) { bulletSprite = nBulletSprite; }
	void setPosition(sf::Vector2i nPosition) { position = nPosition; towerSprite.setPosition(32 + 64*position.x, 32+64*position.y); }
	void setRotation(int nRotation) { rotation = nRotation; towerSprite.setRotation(rotation); }

	//main methods
	void targetEnemy(std::vector<Enemy>& enemies, std::vector<Bullet*>& bullets)
	{
		for (int i = 0; i < enemies.size(); i++)
		{	
			if (sqrt((pow(enemies[i].getPosition().x - this->getPosition().x, 2))
				+ (pow(enemies[i].getPosition().y - this->getPosition().y, 2))) <= range)
			{
				int newRotation = atan((enemies[i].getPosition().x - this->getPosition().x) / (enemies[i].getPosition().y - this->getPosition().y)) * -180 / 3.1415;

				if (enemies[i].getPosition().y - this->getPosition().y >= 0)
					this->setRotation(newRotation+180);
				else
					this->setRotation(newRotation);

				shootCheck(bullets);
				break;
			}
		}
	}

	void sell(int& currency) { currency += sellPrice; delete this; }

	void renderTower(sf::RenderWindow& cWindow) { cWindow.draw(towerSprite); };

	void shootCheck(std::vector<Bullet*>& bullets)
	{
		if (shootTimer == 0)
		{
			shootTimer = shootSpeed;
			shoot(bullets);
		}
		else
			shootTimer--;
	}

	//Abstract
	virtual void shoot(std::vector<Bullet*>& bullets) = 0;

protected:
	int sellPrice;
	float damage;
	float range;
	int shootSpeed;
	int shootTimer;

	sf::Vector2i position;
	int rotation;

	sf::Sprite towerSprite;
	sf::Texture bulletTexture;
	sf::Texture towerTexture;
};

class hedgehog : public base_tower
{
public:
	hedgehog(sf::Vector2i nPos) : base_tower(nPos)
	{
		sellPrice = 250;
		damage = 20;
		range = 128;
		shootSpeed = 50;
		
		if (!towerTexture.loadFromFile("sprites/hedgehog.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		if (!bulletTexture.loadFromFile("sprites/quill.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		towerSprite.setTexture(towerTexture, true);
	}


	void shoot(std::vector<Bullet*>& bullets)
	{
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation+45, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation+90, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation+135, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation+180, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation-45, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation-90, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation-135, bulletTexture));
	}
};

class bunny : public base_tower
{
public:
	bunny(sf::Vector2i nPos) : base_tower(nPos)
	{
		sellPrice = 150;
		damage = 5;
		range = 320;
		shootSpeed = 30;

		if (!towerTexture.loadFromFile("sprites/bunny.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		if (!bulletTexture.loadFromFile("sprites/poop.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		towerSprite.setTexture(towerTexture, true);
	}


	void shoot(std::vector<Bullet*>& bullets)
	{
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 2, range, damage, rotation, bulletTexture));
	}
};

class raccoon : public base_tower
{
public:
	raccoon(sf::Vector2i nPos) : base_tower(nPos)
	{
		sellPrice = 300;
		damage = 30;
		range = 96;
		shootSpeed = 40;

		if (!towerTexture.loadFromFile("sprites/raccoon.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		if (!bulletTexture.loadFromFile("sprites/scratch.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		towerSprite.setTexture(towerTexture, true);
	}

	void shoot(std::vector<Bullet*>& bullets)
	{
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 0.6, range, damage, rotation + 8, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 0.6, range, damage, rotation - 8, bulletTexture));
	}
};

class skunk : public base_tower
{
public:
	skunk(sf::Vector2i nPos) : base_tower(nPos)
	{
		sellPrice = 200;
		damage = 30;
		range = 128;
		shootSpeed = 120;

		if (!towerTexture.loadFromFile("sprites/skunk.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		if (!bulletTexture.loadFromFile("sprites/spray.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		towerSprite.setTexture(towerTexture, true);
	}

	void shoot(std::vector<Bullet*>& bullets)
	{
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation+10, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation, bulletTexture));
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 1, range, damage, rotation-10, bulletTexture));
	}
};

class chipmunk : public base_tower
{
public:
	chipmunk(sf::Vector2i nPos) : base_tower(nPos)
	{
		sellPrice = 175;
		damage = 4;
		range = 384;
		shootSpeed = 20;

		if (!towerTexture.loadFromFile("sprites/chipmunk.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		if (!bulletTexture.loadFromFile("sprites/acorn.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		towerSprite.setTexture(towerTexture, true);
	}


	void shoot(std::vector<Bullet*>& bullets)
	{
		bullets.push_back(new Bullet((sf::Vector2f)getPosition(), 2, range, damage, rotation, bulletTexture));
	}
};