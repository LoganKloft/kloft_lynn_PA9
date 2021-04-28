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
#include "Bullet.h"
#include "Sprite.h"

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
			StartNextWave = WaveInProgress = BunnyHoverMenu =
			SkunkHoverMenu = ChipmunkHoverMenu = HedgehogHoverMenu =
			RaccoonHoverMenu = CanRecycle = false;

		TowerMenuActive = GenerateWave = true;

		currentWave = -1;

		gold = 500;
		health = 100;
	}

	void reset()
	{
		BunnySelected = SkunkSelected = ChipmunkSelected =
			HedgehogSelected = RaccoonSelected = Pause =
			StartNextWave = WaveInProgress = BunnyHoverMenu =
			SkunkHoverMenu = ChipmunkHoverMenu = HedgehogHoverMenu =
			RaccoonHoverMenu = CanRecycle = false;

		TowerMenuActive = GenerateWave = true;

		currentWave = -1;

		gold = 500;
		health = 100;
	}

	~LevelPlayer()
	{
		delete [] available;
	}

	void run(sf::RenderWindow& window)
	{
		// Test

		// Load map
		std::string map_name = selectLevel(window);
		if (map_name == "exit") return;

		Level map;
		if (!map.load("tileset.png", sf::Vector2u(64, 64), map_name, 22, 16))
		{
			std::cout << "Failed to load map in LevelPlayer object" << std::endl;
		}
		map_tiles = map.getTiles();
		setAvailable(&available, map_tiles);

		// Load Enemy textures
		sf::Texture easyEnemyTexture;
		if(!easyEnemyTexture.loadFromFile("sprites/easy_enemy.png"))
		{
			std::cout << "Failed to load easy enemy texture" << std::endl;
		}
		sf::Texture mediumEnemyTexture;
		if (!mediumEnemyTexture.loadFromFile("sprites/medium_enemy.png"))
		{
			std::cout << "Failed to load medium enemy texture" << std::endl;
		}
		sf::Texture hardEnemyTexture;
		if (!hardEnemyTexture.loadFromFile("sprites/hard_enemy.png"))
		{
			std::cout << "Failed to load hard enemy texture" << std::endl;
		}

		// Generate Tower Buttons and Temporary Sprites
		Button towerMenuButton(sf::Vector2f(64, 64), sf::Vector2f(window.getSize().x - 64, window.getSize().y / 2 - 64), sf::Vector2f(64, 64),
			sf::Vector2f(window.getSize().x - 64, window.getSize().y / 2 - 64), "sprites/GUI/menu.png");

		int towersSize = 5;
		Button towerButtons[5] = {
			Button(sf::Vector2f(256,136), sf::Vector2f(0,0), sf::Vector2f(128,128), sf::Vector2f(64,0), "sprites/bunny.png"),
			Button(sf::Vector2f(256,136), sf::Vector2f(0,204), sf::Vector2f(128,128), sf::Vector2f(64,204), "sprites/skunk.png"),
			Button(sf::Vector2f(256,136), sf::Vector2f(0,408), sf::Vector2f(128,128), sf::Vector2f(64,408), "sprites/chipmunk.png"),
			Button(sf::Vector2f(256,136), sf::Vector2f(0,612), sf::Vector2f(128,128), sf::Vector2f(64,612), "sprites/hedgehog.png"),
			Button(sf::Vector2f(256,136), sf::Vector2f(0,816), sf::Vector2f(128,128), sf::Vector2f(64,816), "sprites/raccoon.png") };

		bunny Bunny({ 0,0 });
		skunk Skunk({ 0,0 });
		chipmunk Chipmunk({ 0,0 });
		hedgehog Hedgehog({ 0,0 });
		raccoon Raccoon({ 0,0 });
		
		// Generate Play Button - Pause?
		Button startWaveButton(sf::Vector2f(128, 128), sf::Vector2f(window.getSize().x - 128, window.getSize().y - 128), 
			sf::Vector2f(128, 128), sf::Vector2f(window.getSize().x - 128, window.getSize().y - 128), "sprites/GUI/play_button.png");

		// NO PLACE TOWER BUTTON
		Button recycleTowerButton(sf::Vector2f(128, 128), sf::Vector2f(0, window.getSize().y - 128),
			sf::Vector2f(128, 128), sf::Vector2f(0, window.getSize().y - 128), "sprites/GUI/xmark.png");

		// Create Gold's and Health's texts and sprites
		sf::Font font;
		if (!font.loadFromFile("SugarpunchDEMO.otf"))
		{
			std::cout << "Failed to load font in LevelPlayer (71)" << std::endl;
		}

		sf::Text round_text(std::to_string(currentWave+2), font, 50);
		round_text.setPosition(sf::Vector2f(WINDOW_WIDTH-100, 0));

		sf::Text gold_text(std::to_string(gold), font, 50);
		sf::Text health_text(std::to_string(health), font, 50);
		gold_text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, 0));
		health_text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, 50));
		Sprite gold_sprite("sprites/currency.png", sf::Vector2f(WINDOW_WIDTH / 2 - 64, 0));
		Sprite health_sprite("sprites/hearts.png", sf::Vector2f(WINDOW_WIDTH / 2 - 64, 50));

		//Opacity Boxes
		sf::RectangleShape tower_background({ 256,WINDOW_HEIGHT });
		tower_background.setFillColor({ 0,0,0,64});

		// Stores towers
		std::vector<base_tower*> towers;

		//Stores Bullets
		std::vector<Bullet*> bullets;

		// Create wave
		std::vector<std::vector<int>> waves; // { easy, medium, hard }
		waves.push_back({ 5, 0, 0 }); // 0
		waves.push_back({ 20, 0, 0 }); // 1
		waves.push_back({ 10, 5, 0 }); // 2
		waves.push_back({ 10, 10, 0 }); // 3
		waves.push_back({ 20, 20, 0 }); // 4
		waves.push_back({ 0, 40, 0 }); // 5
		waves.push_back({ 20, 0, 5 }); // 6
		waves.push_back({ 0, 15, 10 }); // 7
		waves.push_back({ 20, 20, 20 }); // 8
		waves.push_back({ 0, 30, 30 }); // 9

		int enemiesDead = 0;
		int enemiesCurrentSize = 0;
		int enemiesTotalSize = 0;
		std::vector<Enemy> enemies;

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
				case sf::Event::MouseMoved:
				{
					if (TowerMenuActive)
					{
						for (int i = 0; i < towersSize; i++)
						{
							if (towerButtons[i].contains(event.mouseMove.x, event.mouseMove.y))
							{
								switch (i)
								{
								case BUNNY:
									BunnyHoverMenu = true;
									break;
								case SKUNK:
									SkunkHoverMenu = true;
									break;
								case CHIPMUNK:
									ChipmunkHoverMenu = true;
									break;
								case HEDGEHOG:
									HedgehogHoverMenu = true;
									break;
								case RACCOON:
									RaccoonHoverMenu = true;
									break;
								}
							}
						}
					}
				}
				break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						if (CanRecycle && recycleTowerButton.contains(event.mouseButton.x, event.mouseButton.y))
						{
							CanRecycle = BunnySelected =SkunkSelected = ChipmunkSelected = HedgehogSelected = RaccoonSelected = false;
							TowerMenuActive = true;
						}

						else if (towerMenuButton.contains(event.mouseButton.x, event.mouseButton.y)) TowerMenuActive = !TowerMenuActive;
						else if (startWaveButton.contains(event.mouseButton.x, event.mouseButton.y)) StartNextWave = !StartNextWave;
						else if (TowerMenuActive)
						{
							for (int i = 0; i < towersSize; i++)
							{
								if (towerButtons[i].contains(event.mouseButton.x, event.mouseButton.y))
								{
									switch (i)
									{
									case BUNNY:
										BunnySelected = !BunnySelected;
										TowerMenuActive = false;
										BunnyHoverMenu = false;
										SkunkSelected = ChipmunkSelected = HedgehogSelected = RaccoonSelected = false;
										break;
									case SKUNK:
										SkunkSelected = !SkunkSelected;
										TowerMenuActive = false;
										SkunkHoverMenu = false;
										BunnySelected = ChipmunkSelected = HedgehogSelected = RaccoonSelected = false;
										break;
									case CHIPMUNK:
										ChipmunkSelected = !ChipmunkSelected;
										TowerMenuActive = false;
										ChipmunkHoverMenu = false;
										SkunkSelected = BunnySelected = HedgehogSelected = RaccoonSelected = false;
										break;
									case HEDGEHOG:
										HedgehogSelected = !HedgehogSelected;
										TowerMenuActive = false;
										HedgehogHoverMenu = false;
										ChipmunkSelected = SkunkSelected = BunnySelected = RaccoonSelected = false;
										break;
									case RACCOON:
										RaccoonSelected = !RaccoonSelected;
										TowerMenuActive = false;
										RaccoonHoverMenu = false;
										HedgehogSelected = ChipmunkSelected = SkunkSelected = BunnySelected = false;
										break;
									}
								}
							}
						}
						else if (BunnySelected && Bunny.getPrice() <= gold)
						{
							int x = sf::Mouse::getPosition(window).x / 64, y = sf::Mouse::getPosition(window).y / 64;
							if (x >= 0 && x < MAX_LEVEL_WIDTH && y >= 0 && y < MAX_LEVEL_HEIGHT && available[y * MAX_LEVEL_WIDTH + x])
							{
								BunnySelected = false;
								available[y * MAX_LEVEL_WIDTH + x] = 0;
								gold -= Bunny.getPrice();
								towers.push_back(new bunny({ x, y }));
								TowerMenuActive = true;
							}
						}
						else if (SkunkSelected && Skunk.getPrice() <= gold)
						{
							int x = sf::Mouse::getPosition(window).x / 64, y = sf::Mouse::getPosition(window).y / 64;
							if (x >= 0 && x < MAX_LEVEL_WIDTH && y >= 0 && y < MAX_LEVEL_HEIGHT && available[y * MAX_LEVEL_WIDTH + x])
							{
								SkunkSelected = false;
								available[y * MAX_LEVEL_WIDTH + x] = 0;
								gold -= Skunk.getPrice();
								towers.push_back(new skunk({ x, y }));
								TowerMenuActive = true;
							}
						}
						else if (ChipmunkSelected && Chipmunk.getPrice() <= gold)
						{
							int x = sf::Mouse::getPosition(window).x / 64, y = sf::Mouse::getPosition(window).y / 64;
							if (x >= 0 && x < MAX_LEVEL_WIDTH && y >= 0 && y < MAX_LEVEL_HEIGHT && available[y * MAX_LEVEL_WIDTH + x])
							{
								ChipmunkSelected = false;
								available[y * MAX_LEVEL_WIDTH + x] = 0;
								gold -= Chipmunk.getPrice();
								towers.push_back(new chipmunk({ x, y }));
								TowerMenuActive = true;
							}
						}
						else if (HedgehogSelected && Hedgehog.getPrice() <= gold)
						{
							int x = sf::Mouse::getPosition(window).x / 64, y = sf::Mouse::getPosition(window).y / 64;
							if (x >= 0 && x < MAX_LEVEL_WIDTH && y >= 0 && y < MAX_LEVEL_HEIGHT && available[y * MAX_LEVEL_WIDTH + x])
							{
								HedgehogSelected = false;
								available[y * MAX_LEVEL_WIDTH + x] = 0;
								gold -= Hedgehog.getPrice();
								towers.push_back(new hedgehog({ x, y }));
								TowerMenuActive = true;
							}
						}
						else if (RaccoonSelected && Raccoon.getPrice() <= gold)
						{
							int x = sf::Mouse::getPosition(window).x / 64, y = sf::Mouse::getPosition(window).y / 64;
							if (x >= 0 && x < MAX_LEVEL_WIDTH && y >= 0 && y < MAX_LEVEL_HEIGHT && available[y * MAX_LEVEL_WIDTH + x])
							{
								RaccoonSelected = false;
								available[y * MAX_LEVEL_WIDTH + x] = 0;
								gold -= Raccoon.getPrice();
								towers.push_back(new raccoon({ x, y }));
								TowerMenuActive = true;
							}
						}
					}
				}
			}

			if (GenerateWave && enemiesDead == enemiesTotalSize)
			{
				WaveInProgress = false;
				StartNextWave = false;
				GenerateWave = false;
				enemiesDead = 0;
				enemiesCurrentSize = 0;
				currentWave++;
				round_text.setString(std::to_string(currentWave+1));
				if (!generateWave(currentWave, enemies, waves, easyEnemyTexture, mediumEnemyTexture, hardEnemyTexture, enemiesTotalSize))
				{
					std::cout << "WINNER!" << std::endl;
					return;
				}
			}

			if (StartNextWave && !WaveInProgress)
			{
				WaveInProgress = true;
				GenerateWave = true;
				gold += (currentWave) % 3 * 25;
			}

			if (enemiesCurrentSize + enemiesDead < enemiesTotalSize && clock.getElapsedTime().asSeconds() >= 1 && WaveInProgress)
			{
				enemiesCurrentSize++;
				clock.restart();
			}

			for (int i = 0; i < enemiesCurrentSize; i++)
			{
				int damage = enemies[i].move();
				if (damage > 0)
				{
					// enemy ran out of waypoints
					enemies.erase(enemies.begin() + i);
					enemiesDead++;
					enemiesCurrentSize--;
					i--;
				}
				health -= damage;
				health_text.setString(std::to_string(health));
			}

			if (health <= 0)
			{
				std::cout << "You lose!" << std::endl;
				return;
			}

			gold_text.setString(std::to_string(gold));

			window.clear();
			window.draw(map);

			for (int i = 0; i < towers.size(); i++)
			{
				if (WaveInProgress)
				{
					towers[i]->targetEnemy(enemies,bullets);
				}

				towers[i]->renderTower(window);
			}

			for (int i = 0; i < bullets.size(); i++)
			{
				bullets[i]->update(enemies, gold, enemiesDead, enemiesCurrentSize);
				bullets[i]->renderBullet(window);
				if (bullets[i]->needsRemoval())
					bullets.erase(bullets.begin() + i);
			}


			if (TowerMenuActive)
			{
				window.draw(tower_background);

				for (int i = 0; i < towersSize; i++)
				{
					window.draw(towerButtons[i]);
				}
			}


			if (BunnyHoverMenu)
			{
				hoverDisplay(window, Bunny, "Bunny");
				if (sf::Mouse::getPosition(window).x > 256 ||
					sf::Mouse::getPosition(window).y > 128) BunnyHoverMenu = false;
			}
			else if (SkunkHoverMenu)
			{
				hoverDisplay(window, Skunk, "Skunk");
				if (sf::Mouse::getPosition(window).x > 256 ||
					sf::Mouse::getPosition(window).y < 204 ||
					sf::Mouse::getPosition(window).y > 332) SkunkHoverMenu = false;
			}
			else if (ChipmunkHoverMenu)
			{
				hoverDisplay(window, Chipmunk, "Chipmunk");
				if (sf::Mouse::getPosition(window).x > 256 ||
					sf::Mouse::getPosition(window).y < 408 ||
					sf::Mouse::getPosition(window).y > 536) ChipmunkHoverMenu = false;
			}
			else if (HedgehogHoverMenu)
			{
				hoverDisplay(window, Hedgehog, "Hedgehog");
				if (sf::Mouse::getPosition(window).x > 256 ||
					sf::Mouse::getPosition(window).y < 612 ||
					sf::Mouse::getPosition(window).y > 740) HedgehogHoverMenu = false;
			}
			else if (RaccoonHoverMenu)
			{
				hoverDisplay(window, Raccoon, "Raccoon");
				if (sf::Mouse::getPosition(window).x > 256 ||
					sf::Mouse::getPosition(window).y < 816 ||
					sf::Mouse::getPosition(window).y > 944) RaccoonHoverMenu = false;
			}

			if (BunnySelected)
			{
				Bunny.setPosition(sf::Vector2i(sf::Mouse::getPosition(window).x / 64, sf::Mouse::getPosition(window).y / 64));
				Bunny.renderTower(window);
			}
			else if (SkunkSelected)
			{
				Skunk.setPosition(sf::Vector2i(sf::Mouse::getPosition(window).x / 64, sf::Mouse::getPosition(window).y / 64));
				Skunk.renderTower(window);
			}
			else if (ChipmunkSelected)
			{
				Chipmunk.setPosition(sf::Vector2i(sf::Mouse::getPosition(window).x / 64, sf::Mouse::getPosition(window).y / 64));
				Chipmunk.renderTower(window);
			}
			else if (HedgehogSelected)
			{
				Hedgehog.setPosition(sf::Vector2i(sf::Mouse::getPosition(window).x / 64, sf::Mouse::getPosition(window).y / 64));
				Hedgehog.renderTower(window);
			}
			else if (RaccoonSelected)
			{
				Raccoon.setPosition(sf::Vector2i(sf::Mouse::getPosition(window).x / 64, sf::Mouse::getPosition(window).y / 64));
				Raccoon.renderTower(window);
			}
			for (int i = 0; i < enemiesCurrentSize; i++)
			{
				window.draw(enemies[i]);
			}

			window.draw(towerMenuButton);
			if (enemiesCurrentSize == 0)
			{
				startWaveButton.setFillColor(sf::Color::Green);
			}
			else
			{
				startWaveButton.setFillColor({150,150,150});
			}

			window.draw(startWaveButton);
			window.draw(gold_text);
			window.draw(health_text);
			window.draw(gold_sprite);
			window.draw(round_text);
			window.draw(health_sprite);


			if (BunnySelected || RaccoonSelected || SkunkSelected || ChipmunkSelected || HedgehogSelected)
			{
				window.draw(recycleTowerButton);
				CanRecycle = true;
			}
			window.display();
		}
	}
private:
	int gold;
	int health;
	int* map_tiles;
	int* available; // if == 1 can place tower

	// Tower Menu
	bool TowerMenuActive;
	bool BunnySelected;
	bool BunnyHoverMenu;
	bool SkunkSelected;
	bool SkunkHoverMenu;
	bool ChipmunkSelected;
	bool ChipmunkHoverMenu;
	bool HedgehogSelected;
	bool HedgehogHoverMenu;
	bool RaccoonSelected;
	bool RaccoonHoverMenu;
	bool CanRecycle;

	// Wave
	bool StartNextWave;
	bool WaveInProgress;
	bool GenerateWave;
	int currentWave;

	// Misc
	bool Pause;

	// polymorphism :-)
	void hoverDisplay(sf::RenderWindow& window, base_tower& tower, const std::string &name)
	{
		int price = tower.getPrice();
		int damage = tower.getDamage();
		int range = tower.getRange();

		sf::Vector2i pos(sf::Mouse::getPosition(window));
		sf::Font font;
		if (!font.loadFromFile("SugarpunchDEMO.otf"))
		{
			std::cout << "Failed to load font in LevelPlayer (413)" << std::endl;
		}

		sf::Text name_text("Name: " + name, font, 30);
		sf::Text price_text("Price: " + std::to_string(price), font, 30);
		sf::Text damage_text("Damage: " + std::to_string(damage), font, 30);
		sf::Text range_text("Range: " + std::to_string(range), font, 30);
		int yOffset = 0;

		if (name == "Skunk")
			yOffset = 204;
		else if (name == "Chipmunk")
			yOffset = 2* 204;
		else if (name == "Hedgehog")
			yOffset = 3* 204;
		else if (name == "Raccoon")
			yOffset = 4* 204;

		sf::RectangleShape shadow({ 256,136 });
		shadow.setFillColor({ 0,0,0,128 });
		shadow.setPosition(0, yOffset);


		name_text.setPosition(10, 5+yOffset);
		price_text.setPosition(10, 35 + yOffset);
		damage_text.setPosition(10, 65 + yOffset);
		range_text.setPosition(10, 95 + yOffset);

		window.draw(shadow);
		window.draw(name_text); window.draw(price_text); window.draw(damage_text); window.draw(range_text);
	}

	// returns false if no more waves to generate
	bool generateWave(int currentWave, std::vector<Enemy>& enemies, std::vector<std::vector<int>>& waves,
		sf::Texture& easyEnemyTexture, sf::Texture& mediumEnemyTexture, sf::Texture& hardEnemyTexture,
		int &enemiesTotalSize)
	{
		bool success = false;
		enemies.clear();
		if (currentWave < waves.size())
		{
			success = true;
			enemiesTotalSize = waves[currentWave][0] + waves[currentWave][1] + waves[currentWave][2];
			for (int i = 0; i < waves[currentWave][0]; i++) // easy enemies
			{
				Enemy easy(easyEnemyTexture, 100, 1, 1.5, 10);
				easy.calcWaypoints(map_tiles);
				enemies.push_back(easy);
			}
			for (int i = 0; i < waves[currentWave][1]; i++) // medium enemies
			{
				Enemy medium(mediumEnemyTexture, 125, 3, 2.75, 15);
				medium.calcWaypoints(map_tiles);
				enemies.push_back(medium);
			}
			for (int i = 0; i < waves[currentWave][2]; i++) // hard enemies
			{
				Enemy hard(hardEnemyTexture, 250, 5, 2.25, 20);
				hard.calcWaypoints(map_tiles);
				enemies.push_back(hard);
			}
		}
		return success;
	}

	void setAvailable(int** available, int* tiles)
	{
		*available = new int[MAX_LEVEL_HEIGHT * MAX_LEVEL_WIDTH];
		for (int i = 0; i < MAX_LEVEL_HEIGHT * MAX_LEVEL_WIDTH; i++)
		{
			if (tiles[i] == TILE_GRASS) (*available)[i] = 1;
			else (*available)[i] = 0;
		}
	}

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
		if (!font.loadFromFile("SugarpunchDEMO.otf"))
		{
			std::cout << "Failed to load font in LevelPlayer (219)" << std::endl;
		}

		for (int i = 0; i < level_list.size(); i++)
		{
			sf::Text tmp(level_list[i], font, 50);
			tmp.move(WINDOW_WIDTH / 3 - 100, WINDOW_HEIGHT / 2 - 100);
			levelListText.push_back(tmp);
		}

		Button exit_button(sf::Vector2f(64, 64), sf::Vector2f(0, 0), 
			sf::Vector2f(64, 64), sf::Vector2f(0, 0), "sprites/GUI/xmark.png");
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