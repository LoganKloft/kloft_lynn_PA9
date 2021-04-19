#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Level.h"

#define WINDOW_WIDTH 1408
#define WINDOW_HEIGHT 1024

struct TileContainer
{
	sf::RectangleShape shape;
	sf::IntRect hitbox;
	int tileNumber;
};

class LevelEditor
{
public:
	std::string selectLevel(sf::RenderWindow& window)
	{
		std::ifstream infile("level_list.txt");
		std::vector<std::string> level_list(1, "new");
		std::string lvl = "", str = "";
		while (std::getline(infile, lvl, ','))
		{
			level_list.push_back(lvl);
			std::cout << lvl << std::endl;
		}

		std::vector<sf::Text> levelListText;
		sf::Font font;
		font.loadFromFile("Arialic Hollow.ttf");
		for (int i = 0; i < level_list.size(); i++)
		{
			sf::Text tmp(level_list[i], font, 50);
			tmp.move(WINDOW_WIDTH / 3 - 100, WINDOW_HEIGHT / 2 - 100);
			levelListText.push_back(tmp);
		}

		sf::Event event;
		bool flag = true;
		int i = 0;
		sf::Text next("next", font, 50);
		next.move(2*(WINDOW_WIDTH / 3), WINDOW_HEIGHT / 2 - 100);
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
					if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x < WINDOW_WIDTH / 2)
					{
						// select level
						str = level_list[i];
						if (str == "new") str = "";
						flag = false;
					}
					else if(i == levelListText.size() - 1)
					{
						// wrap around to beginning
						i = 0;
					}
					else
					{
						// cycle to next level
						i++;
					}
					break;

				default:
					break;
				}
			}
			window.clear();
			window.draw(levelListText[i]);
			window.draw(next);
			window.display();
		}

		return str;
	}

	bool edit(sf::RenderWindow& window)
	{
		sf::RenderWindow tileSelector(sf::VideoMode(180, 1024), "Tile Selector");
		Level map;
		int tileNumber = 0;

		std::string levelFile = selectLevel(window);
		if (!map.load("tileSet_1.png", sf::Vector2u(64, 64), levelFile, 22, 16))
		{
			std::cout << "Failed to load level in LevelEditor" << std::endl;
			return false;
		}

		std::vector<struct TileContainer> tileOptions;
		generateTileOptions("tileSet_1.png", sf::Vector2u(64, 64), 8, 3, tileOptions);

		while (tileSelector.isOpen())
		{
			sf::Event event1;
			while (tileSelector.pollEvent(event1))
			{
				switch (event1.type)
				{
				case sf::Event::Closed:
					tileSelector.close();
					return true;
				case sf::Event::MouseButtonPressed:
					if (event1.mouseButton.button == sf::Mouse::Left)
					{
						for (int i = 0; i < tileOptions.size(); i++)
						{
							if (tileOptions[i].hitbox.contains(event1.mouseButton.x, event1.mouseButton.y))
							{
								tileNumber = tileOptions[i].tileNumber;
								break;
							}
						}
					}
					break;
				default:
					break;
				}
			}

			sf::Event event2;
			while (window.pollEvent(event2))
			{
				switch (event2.type)
				{
				case sf::Event::MouseButtonPressed:
					if (event2.mouseButton.button == sf::Mouse::Left)
					{
						int x = sf::Mouse::getPosition(window).x / 64;
						int y = sf::Mouse::getPosition(window).y / 64;
						map.setTile(sf::Vector2u(64, 64), x, y, MAX_LEVEL_WIDTH, tileNumber);
					}
					break;
				}
			}

			tileSelector.clear();
			for (int i = 0; i < tileOptions.size(); i++)
			{
				tileSelector.draw(tileOptions[i].shape);
			}
			tileSelector.display();
			window.clear();
			window.draw(map);
			window.display();
		}
	}
private:

	sf::Texture m_tileset;

	void generateTileOptions(std::string tileSetFile, sf::Vector2u tileSize, int width, int height, std::vector<struct TileContainer>& tileOptions)
	{
		if (!m_tileset.loadFromFile(tileSetFile))
		{
			return;
		}

		int offsetScale = 0;
		int tileNumber = 0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				sf::RectangleShape shape(sf::Vector2f(90, 90));
				shape.setTexture(&m_tileset);
				shape.setTextureRect(sf::IntRect(j * tileSize.x, i * tileSize.y , tileSize.x, tileSize.y));
				shape.move(sf::Vector2f((offsetScale % 2) * 90, (offsetScale / 2) * 90));

				sf::IntRect hitbox((offsetScale % 2) * 90, (offsetScale / 2) * 90, 90, 90);

				struct TileContainer newTileOption;
				newTileOption.shape = shape;
				newTileOption.hitbox = hitbox;
				newTileOption.tileNumber = tileNumber;

				tileOptions.push_back(newTileOption);

				offsetScale++;
				tileNumber++;
			}
		}
	}
};