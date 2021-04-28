#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "LevelEditor.h"
#include "LevelPlayer.h"
#include "testClass.h"

#define WINDOW_WIDTH 1408
#define WINDOW_HEIGHT 1024

/*
*	Credits:
*	tileset: KUBI Games https://kubigames.itch.io/road-tiles
*	font: Typodermic Fonts 	https://www.1001freefonts.com/bullpen-3d.font, Gem Fonts https://www.1001freefonts.com/arialic-hollow.font,
*	https://www.1001fonts.com/sugarpunch-demo-font.html Jakob Fischer / pizzadude.dk, https://www.1001fonts.com/planet-benson-2-font.html Raymond Larabie
*	Professor Andrew O'Fallon Image: https://s3.wp.wsu.edu/uploads/sites/908/2019/11/Andy-O%E2%80%99Fallon-e1589485912918-198x198.jpg
*/
int main(void)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Habitation Defense - Lynn/Kloft", sf::Style::Close);
	window.setFramerateLimit(60);
	LevelEditor lEditor;
	LevelPlayer lPlayer;

	sf::Font font;
	if (!font.loadFromFile("SugarpunchDEMO.otf"))
	{
		std::cout << "Failed to load font: Main(26)" << std::endl;
		return -1;
	}

	sf::Text playLevelText("PLAY", font, 100);
	playLevelText.move(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 4 - 100);

	sf::Text editLevelText("EDIT", font, 100);
	editLevelText.move(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100);

	sf::Text testCasesText("TEST", font, 100);
	testCasesText.move(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 4 * 3 - 100);

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
					if (sf::Mouse::getPosition(window).y < WINDOW_HEIGHT / 3)
					{
						lPlayer.run(window);
						lPlayer.reset();
					}
					else if(sf::Mouse::getPosition(window).y < WINDOW_HEIGHT / 3 * 2)
					{
						lEditor.edit(window);
					}
					else
					{
						testClass tClass(window);
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
		window.draw(testCasesText);
		window.display();
	}

	return 0;
}