#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <cmath>


#include "Enemy.h"	//

class base_tower
{
public:
	//Constructor
	base_tower(int nSellPrice, float nDamage, float nRange, sf::Sprite nTowerSprite, sf::Sprite nBulletSprite) 
	{
		sellPrice = nSellPrice;
		damage = nDamage;
		range = nRange;
		towerSprite = nTowerSprite;
		bulletSprite = nBulletSprite;

		rotation = 0;
		this->setPosition({0,0});
		bulletSprite.setOrigin(32, 32);
		towerSprite.setOrigin(32, 32);
	}

	//Destructor
	~base_tower() {}

	//Getters and Setters
	int getPrice() { return sellPrice; }
	float getDamage() { return damage; }
	float getRange() { return range; }
	sf::Sprite getTowerSprite() { return towerSprite; }
	sf::Sprite getBulletSprite() { return bulletSprite; }
	sf::Vector2i getPosition() { return { 32 + 64 * position.x ,32 + 64 * position.y }; }
	int getRotation() { return rotation; }

	void setPrice(int nSellPrice) { sellPrice = nSellPrice; }
	void setDamage(float nDamage) { damage = nDamage; }
	void setRange(float nRange) { range = nRange; }
	void setTowerSprite(sf::Sprite nTowerSprite) { towerSprite = nTowerSprite; }
	void setBulletSprite(sf::Sprite nBulletSprite) { bulletSprite = nBulletSprite; }
	void setPosition(sf::Vector2i nPosition) { position = nPosition; towerSprite.setPosition(32 + 64*position.x, 32+64*position.y); }
	void setRotation(int nRotation) { rotation = nRotation; towerSprite.setRotation(rotation); }

	void targetEnemy(std::vector<Enemy>& enemies) 
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
				break;
			}
		}
	}

	void sell(int& currency) { currency += sellPrice; delete this; }

	void renderTower(sf::RenderWindow& cWindow) { cWindow.draw(towerSprite); };

protected:
	int sellPrice;
	float damage;
	float range;

	sf::Vector2i position;
	int rotation;

	sf::Sprite towerSprite;
	sf::Sprite bulletSprite;
};

/*Methods:
targetEnemy() // 
sell() // destroys tower and returns portion of cost
upgrade() // maybe interacts with an upgrade class that returns type of upgrade and
	     // stores into an upgrades array (think Black Ops zombies perks like upgrades). OR 
     // upgrades could be like level 1,2,3 or a tree
renderTower() // pure virtual function - each inherited class looks different
Data Members:
Damage // damage dealt to enemies
Range // radius for circle range - could look into other shapes
BulletType // encapsulates object of type Bullet
Upgrades // array that stores all upgrades on tower
*/