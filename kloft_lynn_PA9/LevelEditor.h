#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Level.h"
#include "Button.h"

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
	bool edit(sf::RenderWindow& window)
	{
		// default font for sf::Text objects
		sf::Font font;
		if (!font.loadFromFile("Arialic Hollow.ttf"))
		{
			std::cout << "Failed to load font \"Arrialic Hollow.ttf\": LevelEditor(102)" << std::endl;
			return -1;
		}
		Level map;

		// the tile number corresponding to texture being applied in level
		int tileNumber = 0;

		// select to create a new level or edit an existing one
		std::string levelFile = selectLevel(window);
		if (levelFile == "exit") return true;
		if (!map.load("tileset.png", sf::Vector2u(64, 64), levelFile, 22, 16))
		{
			std::cout << "Failed to load level in LevelEditor" << std::endl;
			return false;
		}

		// generate images of tiles to use
		std::vector<struct TileContainer> tileOptions;
		generateTileOptions("tileset.png", sf::Vector2u(64, 64), 8, 3, tileOptions);

		// save and exit buttons
		Button saveLevel(sf::Vector2f(64, 32), sf::Vector2f(window.getSize().x / 2 - 64, 0),
			sf::Vector2f(64, 32),sf::Vector2f(window.getSize().x / 2 - 64, 0), "check_mark.png");
		Button deleteLevel(sf::Vector2f(64, 32), sf::Vector2f(window.getSize().x / 2, 0),
			sf::Vector2f(64, 32),sf::Vector2f(window.getSize().x / 2, 0), "x_mark.png");
		Button viewTileMenuButton(sf::Vector2f(32, 64), sf::Vector2f(window.getSize().x - 32, window.getSize().y / 2 - 32),
			sf::Vector2f(64, 32),sf::Vector2f(window.getSize().x - 32, window.getSize().y / 2 - 32), "vertical_3lines.png");

		// Stores file name to store in level_list.txt
		std::string saveName;
		sf::Text saveFileText("Enter File Name:", font, 80);
		saveFileText.setOutlineColor(sf::Color::White);
		saveFileText.setPosition(sf::Vector2f(400, window.getSize().y / 2 - 15));

		// controls selection of tileNumber
		while (true)
		{
			sf::Event event2;
			while (window.pollEvent(event2))
			{
				switch (event2.type)
				{
				case sf::Event::MouseButtonPressed:
					if (event2.mouseButton.button == sf::Mouse::Left)
					{
						bool flag = false;
						for (int i = 0; i < tileOptions.size(); i++) // select tileNumber
						{
							if (viewTileMenu && tileOptions[i].hitbox.contains(event2.mouseButton.x, event2.mouseButton.y))
							{
								tileNumber = tileOptions[i].tileNumber;
								flag = true;
								break;
							}
						}
						if (flag) break;
						if (!saveLevel.contains(event2.mouseButton.x, event2.mouseButton.y) // only allow tile placement if mouse not selecting a button
							&& !deleteLevel.contains(event2.mouseButton.x, event2.mouseButton.y)
							&& !viewTileMenuButton.contains(event2.mouseButton.x, event2.mouseButton.y))
						{
							mousePressed = true;
						}
					}
					break;
				case sf::Event::MouseButtonReleased:
					if (event2.mouseButton.button == sf::Mouse::Left)
					{
						mousePressed = false;
						if (saveLevel.contains(event2.mouseButton.x, event2.mouseButton.y))
						{
							// record keyboard input until enter key pressed
							detectText = true;
							saveFile = true;
						}
						else if (deleteLevel.contains(event2.mouseButton.x, event2.mouseButton.y))
						{
							// exit back to main menu
							detectText = false;
							saveFile = false;
							return true;
						}
						else if (viewTileMenuButton.contains(event2.mouseButton.x, event2.mouseButton.y))
						{
							viewTileMenu = !viewTileMenu;
						}
					}
					break;
				case sf::Event::TextEntered:
					if (detectText && saveFile)
					{
						if (event2.text.unicode == 13)
						{
							detectText = saveFile = false;
							saveName += ".txt";
							map.saveTile(saveName, 22, 16);
							std::ofstream outfile("level_list.txt");
							outfile << saveName << ',';
							for (int i = 1; i < level_list.size(); i++) // start at 1 to skip new
							{
								if (level_list[i] != saveName) // prevent adding duplicates
								{
									outfile << level_list[i] << ',';
								}
							}
							outfile.close();
							return true;
						}
						else if(event2.text.unicode <= 128)
						{
							saveName += event2.text.unicode;
							saveFileText.setString(saveName);
						}
					}
				}
			}

			if (mousePressed)
			{
				// apply texture to tile
				int x = sf::Mouse::getPosition(window).x / 64;
				int y = sf::Mouse::getPosition(window).y / 64;
				map.setTile(sf::Vector2u(64, 64), x, y, MAX_LEVEL_WIDTH, tileNumber);
			}

			window.clear();
			window.draw(map);
			if (viewTileMenu)
			{
				for (int i = 0; i < tileOptions.size(); i++)
				{
					window.draw(tileOptions[i].shape);
				}
			}
			if (saveFile)
			{
				window.draw(saveFileText);
			}
			window.draw(viewTileMenuButton);
			window.draw(saveLevel);
			window.draw(deleteLevel);
			window.display();
		}
	}
private:

	sf::Texture m_tileset;
	std::vector<std::string> level_list;
	bool detectText;
	bool saveFile;
	bool mousePressed;
	bool viewTileMenu;

	// width = image width / tileSize width in pixels
	// height = image height / tileSize height in pixels
	// tileSize - 64 x 64
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

	std::string selectLevel(sf::RenderWindow& window)
	{
		level_list.clear();
		level_list.push_back("new");
		std::ifstream infile("level_list.txt");
		std::string lvl = "", str = "";
		while (std::getline(infile, lvl, ','))
		{
			level_list.push_back(lvl);
			std::cout << lvl << std::endl;
		}

		infile.close();

		std::vector<sf::Text> levelListText;
		sf::Font font;
		font.loadFromFile("Arialic Hollow.ttf");
		for (int i = 0; i < level_list.size(); i++)
		{
			sf::Text tmp(level_list[i], font, 50);
			tmp.move(WINDOW_WIDTH / 3 - 100, WINDOW_HEIGHT / 2 - 100);
			levelListText.push_back(tmp);
		}

		Button exit_button(sf::Vector2f(64, 64), sf::Vector2f(0, 0), 
			sf::Vector2f(64, 64), sf::Vector2f(0, 0), "x_mark.png");
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
					if (exit_button.contains(event.mouseButton.x, event.mouseButton.y))
					{
						// exit
						return "exit";
					}
					else if (event.mouseButton.x < WINDOW_WIDTH / 2)
					{
						// select level
						str = level_list[i];
						if (str == "new") str = "";
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