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

#define WINDOW_WIDTH 1408
#define WINDOW_HEIGHT 1024

enum turrets
{
	BUNNY, SKUNK, CHIPMUNK, HEDGEHOG, RACCOON
};

class LevelPlayer
{
public:
	LevelPlayer()
	{
		BunnySelected = SkunkSelected = ChipmunkSelected = 
			HedgehogSelected = RaccoonSelected = Pause = 
			StartNextWave = WaveInProgress = false;

		TowerMenuActive = true;

		currentWave = 0;

		gold = 500;
		health = 100;
	}

	void run(sf::RenderWindow& window)
	{
		// Test
		sf::CircleShape shape(40);

		// Load map
		std::string map_name = selectLevel(window);
		if (map_name == "exit") return;

		Level map;
		if (!map.load("tileSet_1.png", sf::Vector2u(64, 64), map_name, 22, 16))
		{
			std::cout << "Failed to load map in LevelPlayer object" << std::endl;
		}
		
		// Load Enemy textures
		sf::Texture tank_texture;
		if(!tank_texture.loadFromFile("tank.png"))
		{
			std::cout << "Failed to load tank texture" << std::endl;
		}

		// Generate Tower Buttons
		Button towerMenuButton(sf::Vector2f(32, 64),
			sf::Vector2f(window.getSize().x - 32, window.getSize().y / 2 - 32), "vertical_3lines.png");

		int towersSize = 5;
		Button towers[5] = { 
			Button(sf::Vector2f(128,128), sf::Vector2f(0,0), "bunny.png"),
			Button(sf::Vector2f(128,128), sf::Vector2f(0,204), "skunk.png"),
			Button(sf::Vector2f(128,128), sf::Vector2f(0,408), "chipmunk.png"),
			Button(sf::Vector2f(128,128), sf::Vector2f(0,612), "hedgehog.png"),
			Button(sf::Vector2f(128,128), sf::Vector2f(0,816), "raccoon.png") };
		
		// Generate Play/Pause Buttons
		Button startWaveButton(sf::Vector2f(128, 128), sf::Vector2f(window.getSize().x - 128, window.getSize().y - 128), "play_button.png");

		// Create Gold's and Health's texts and sprites
		sf::Font font;
		if (!font.loadFromFile("Arialic Hollow.ttf"))
		{
			std::cout << "Failed to load font in LevelPlayer (71)" << std::endl;
		}
		sf::Text gold_text(std::to_string(gold), font, 50);
		sf::Text health_text(std::to_string(health), font, 50);
		gold_text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, 0));
		health_text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, 50));

		// Create wave
		std::vector<std::vector<int>> waves;
		waves.push_back({ 20, 0, 0 });

		int enemiesCurrentSize = 0;
		int enemiesMaxSize = waves[0][0] + waves[0][1] + waves[0][2];
		std::vector<Enemy> enemies;
		for (int i = 0; i < waves[0][0]; i++) // easy enemies
		{
			Enemy tank(tank_texture, 5, 5, .05, 5);
			tank.calcWaypoints(map.getTiles());
			enemies.push_back(tank);
		}
		for (int i = 0; i < waves[0][1]; i++) // medium enemies
		{

		}
		for (int i = 0; i < waves[0][2]; i++) // hard enemies
		{

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
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						if (TowerMenuActive)
						{
							for (int i = 0; i < towersSize; i++)
							{
								if (towers[i].contains(event.mouseButton.x, event.mouseButton.y))
								{
									switch (i)
									{
									case BUNNY:
										BunnySelected = !BunnySelected;
										SkunkSelected = ChipmunkSelected = HedgehogSelected = RaccoonSelected = false;
										break;
									case SKUNK:
										SkunkSelected = !SkunkSelected;
										BunnySelected = ChipmunkSelected = HedgehogSelected = RaccoonSelected = false;
										break;
									case CHIPMUNK:
										ChipmunkSelected = !ChipmunkSelected;
										SkunkSelected = BunnySelected = HedgehogSelected = RaccoonSelected = false;
										break;
									case HEDGEHOG:
										HedgehogSelected = !HedgehogSelected;
										ChipmunkSelected = SkunkSelected = BunnySelected = RaccoonSelected = false;
										break;
									case RACCOON:
										RaccoonSelected = !RaccoonSelected;
										HedgehogSelected = ChipmunkSelected = SkunkSelected = BunnySelected = false;
										break;
									}
								}
							}
						}
						if (towerMenuButton.contains(event.mouseButton.x, event.mouseButton.y)) TowerMenuActive = !TowerMenuActive;
						if (startWaveButton.contains(event.mouseButton.x, event.mouseButton.y)) StartNextWave = !StartNextWave;
					}
				}
			}

			if (StartNextWave && !WaveInProgress)
			{
				WaveInProgress = true;
			}

			if (enemiesCurrentSize < 20 && clock.getElapsedTime().asSeconds() >= 1 && WaveInProgress)
			{
				enemiesCurrentSize++;
				clock.restart();
			}

			for (int i = 0; i < enemiesCurrentSize; i++)
			{
				if (!enemies[i].isDead())
				{
					health -= enemies[i].move();
					health_text.setString(std::to_string(health));
				}
			}

			window.clear();
			window.draw(map);
			window.draw(gold_text);
			window.draw(health_text);
			window.draw(towerMenuButton);
			window.draw(startWaveButton);
			if (TowerMenuActive)
			{
				for (int i = 0; i < towersSize; i++)
				{
					window.draw(towers[i]);
				}
			}

			if (BunnySelected)
			{
				shape.setPosition(sf::Mouse::getPosition(window).x - 32, sf::Mouse::getPosition(window).y - 32);
				window.draw(shape);
			}
			else if (SkunkSelected)
			{

			}
			else if (ChipmunkSelected)
			{

			}
			else if (HedgehogSelected)
			{

			}
			else if (RaccoonSelected)
			{

			}
			for (int i = 0; i < enemiesCurrentSize; i++)
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

	// Tower Menu
	bool TowerMenuActive;
	bool BunnySelected;
	bool SkunkSelected;
	bool ChipmunkSelected;
	bool HedgehogSelected;
	bool RaccoonSelected;

	// Wave
	bool StartNextWave;
	bool WaveInProgress;
	int currentWave;

	// Misc
	bool Pause;

	std::string selectLevel(sf::RenderWindow& window)
	{
		std::vector<std::string> level_list;

		std::ifstream infile("level_list.txt");
		if (!infile.is_open())
		{
			std::cout << "Failed to open level list in LevelPlayer (202)" << std::endl;
		}

		std::string lvl = "", str = "";
		while (std::getline(infile, lvl, ','))
		{
			level_list.push_back(lvl);
			std::cout << lvl << std::endl;
		}

		infile.close();

		std::vector<sf::Text> levelListText;
		sf::Font font;
		if (!font.loadFromFile("Arialic Hollow.ttf"))
		{
			std::cout << "Failed to load font in LevelPlayer (219)" << std::endl;
		}

		for (int i = 0; i < level_list.size(); i++)
		{
			sf::Text tmp(level_list[i], font, 50);
			tmp.move(WINDOW_WIDTH / 3 - 100, WINDOW_HEIGHT / 2 - 100);
			levelListText.push_back(tmp);
		}

		Button exit_button(sf::Vector2f(64, 64), sf::Vector2f(0, 0), "x_mark.png");
		sf::Event event;
		bool flag = true;
		int i = 0;
		sf::Text next("next", font, 50);
		next.move(2 * (WINDOW_WIDTH / 3), WINDOW_HEIGHT / 2 - 100);
		while (flag)
		{
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						if (exit_button.contains(event.mouseButton.x, event.mouseButton.y))
						{
							// exit
							return "exit";
						}
						else if (event.mouseButton.x < WINDOW_WIDTH / 2)
						{
							// select level
							str = level_list[i];
							flag = false;
						}
						else if (i == levelListText.size() - 1)
						{
							// wrap around to beginning
							i = 0;
						}
						else
						{
							// cycle to next level
							i++;
						}
					}
					break;

				default:
					break;
				}
			}
			window.clear();
			window.draw(levelListText[i]);
			window.draw(next);
			window.draw(exit_button);
			window.display();
		}

		return str;
	}
};