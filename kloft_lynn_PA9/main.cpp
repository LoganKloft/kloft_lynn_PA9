#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Level.h"
#include "LevelEditor.h"

#define WINDOW_WIDTH 1408
#define WINDOW_HEIGHT 1024

/*
*	Credits:
*	tileset: KUBI Games https://kubigames.itch.io/road-tiles
*	font: Typodermic Fonts 	https://www.1001freefonts.com/bullpen-3d.font
*/
int main(void)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense", sf::Style::Close);
	LevelEditor lEditor;

	sf::Font font;
	if (!font.loadFromFile("Arialic Hollow.ttf"))
	{
		std::cout << "Failed to load font: Main(26)" << std::endl;
		return -1;
	}

	sf::Text editLevelText("EDIT", font, 100);
	editLevelText.move(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100);

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
					lEditor.edit(window);
				}
				break;

			default:
				break;
			}
		}

		window.clear();
		window.draw(editLevelText);
		window.display();
	}

	return 0;
}