#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Level.h"
#include "LevelEditor.h"
#include "LevelPlayer.h"

#define WINDOW_WIDTH 1408
#define WINDOW_HEIGHT 1024

/*
*	Credits:
*	tileset: KUBI Games https://kubigames.itch.io/road-tiles
*	font: Typodermic Fonts 	https://www.1001freefonts.com/bullpen-3d.font, Gem Fonts https://www.1001freefonts.com/arialic-hollow.font
*/
int main(void)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense", sf::Style::Close);
	LevelEditor lEditor;
	LevelPlayer lPlayer;

	sf::Font font;
	if (!font.loadFromFile("Arialic Hollow.ttf"))
	{
		std::cout << "Failed to load font: Main(26)" << std::endl;
		return -1;
	}

	sf::Text editLevelText("EDIT", font, 100);
	editLevelText.move(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 300);

	sf::Text playLevelText("PLAY", font, 100);
	playLevelText.move(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 300);

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
					if (sf::Mouse::getPosition(window).y > WINDOW_HEIGHT / 2)
					{
						lEditor.edit(window);
					}
					else
					{
						lPlayer.run(window);
					}
				}
				break;

			default:
				break;
			}
		}

		window.clear();
		window.draw(editLevelText);
		window.draw(playLevelText);
		window.display();
	}

	return 0;
}