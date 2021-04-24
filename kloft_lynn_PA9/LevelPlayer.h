#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "Level.h"
#include "Button.h"
#include "Enemy.h"

#include "Tower.h"

class LevelPlayer
{
public:
	void run(sf::RenderWindow& window)
	{
		Level map;
		if (!map.load("tileSet_1.png", sf::Vector2u(64, 64), "level1.txt", 22, 16))
		{
			std::cout << "Failed to load map in LevelPlayer object" << std::endl;
		}
		
		sf::Texture tank_texture;
		if(!tank_texture.loadFromFile("tank.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		sf::Texture hedgehog;
		if (!hedgehog.loadFromFile("sprites/hedgehog.png"))
		{
			std::cout << "Failed to load hedgehog texture" << std::endl;
		}

		std::vector<Enemy> enemies;

		std::vector<base_tower> towers;


		sf::Sprite tower_sprite;
		tower_sprite.setTexture(hedgehog, true);

		
		base_tower test(1000, 1000, 300, tower_sprite, tower_sprite);
		test.setPosition({ 8,6 });

		towers.push_back(test);

		test.setPosition({ 4, 5 });

		towers.push_back(test);

		test.setPosition({ 7, 12 });

		towers.push_back(test);

		test.setPosition({ 12, 6 });

		towers.push_back(test);


		

		int size = 0;
		for (int i = 0; i < 20; i++)
		{
			Enemy tank(tank_texture, 5, 5, 5);
			tank.calcWaypoints(map.getTiles());
			enemies.push_back(tank);
		}

		sf::Clock clock;

		sf::Event event;
		while (window.isOpen())
		{
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					window.close();
					break;
				}
			}

			if (size < 20 && clock.getElapsedTime().asSeconds() >= 1)
			{
				size++;
				clock.restart();
			}

			for (int i = 0; i < size; i++)
			{
				if(!enemies[i].isDead())
					enemies[i].move();
			}
			window.clear();
			window.draw(map);
			for (int i = 0; i < size; i++)
			{
				window.draw(enemies[i]);
			}

			test.targetEnemy(enemies);

			for (int i = 0; i < towers.size(); i++)
			{
				towers[i].targetEnemy(enemies);
				towers[i].renderTower(window);
			}


			window.display();
		}
	}
private:
	int gold;
	int health;
	bool startNextWave;

	std::string selectLevel();
};