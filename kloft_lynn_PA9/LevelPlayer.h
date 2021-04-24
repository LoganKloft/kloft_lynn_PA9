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
		std::vector<Enemy> enemies;

		int size = 0;
		for (int i = 0; i < 20; i++)
		{
			Enemy tank(tank_texture, 5, 5, .05);
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
			window.display();
		}
	}
private:
	int gold;
	int health;
	bool startNextWave;

	std::string selectLevel();
};