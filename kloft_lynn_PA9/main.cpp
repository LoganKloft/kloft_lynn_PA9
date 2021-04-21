/************************************************
*	Creators: Jeremiah Lynn and Logan Kloft		*
*	Project: #TBD# Defense Game					*
*	Class: CPT_S 122							*
*	Description: just a game made for class		*
*				 that will one day sell for		*
*				 millions of dollars.. prolly	*
*												*
************************************************/

/*
*	Last Commit	: 4/15/21 9:47AM
*	Commit by:	Jeremiah
*	Description: Added comment block and possible
*				 format for making and commenting
*				 commits.
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>

int main(void)
{
	//GLOBAL VARIABLES
	int gamestate = 0;
	int difficulty = 0;
	const unsigned int resX = 1408;
	const unsigned int resY = 1024;
	const unsigned int BPP = 64;

	//Window Initializer
	sf::RenderWindow window(sf::VideoMode(resX, resY, BPP), "TD Kloft/Lynn Development", sf::Style::Close);

	//Disable Cursor
	window.setMouseCursorVisible(false);


	//Shapes for Testing
	sf::RectangleShape arrowShape({ 100,100 });
	sf::RectangleShape mouseShape({ 100,100 });
	arrowShape.setFillColor(sf::Color::Green);
	mouseShape.setFillColor(sf::Color::Blue);

	//for mouse testing
	sf::Mouse mouse;
	sf::Vector2i mousePos;

	while (window.isOpen())
	{
		//Handle Events
		sf::Event event;
		while (window.pollEvent(event))
		{
			//CLOSING GAME
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}

			//MOUSE TESTING
			mousePos.x = mouse.getPosition().x - window.getPosition().x;
			mousePos.y = mouse.getPosition().y - window.getPosition().y;

			if (mousePos.x >= 0 && mousePos.x <= resX
				&& mousePos.y >= 0 && mousePos.y <= resY)
				mouseShape.setPosition({ (float)(mousePos.x)-50,(float)(mousePos.y)-50 });

			//Key Handling (Slow? Look into timing, FPS limits, loop speeds, etc)
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					std::cout << "PAUSE";
					break;

				case sf::Keyboard::Left:
					if (arrowShape.getPosition().x > 5.f)
						arrowShape.move({ -5.f,0.0 });
					break;

				case sf::Keyboard::Right:
					if (arrowShape.getPosition().x < 1303.f)
						arrowShape.move({ 5.f,0.0 });
					break;

				case sf::Keyboard::Down:
					if (arrowShape.getPosition().y < 1024.f)
						arrowShape.move({ 0.0,5.f });
					break;

				case sf::Keyboard::Up:
					if (arrowShape.getPosition().y > 5.f)
						arrowShape.move({ 0.0,-5.f });
					break;

				default:
					std::cout << "No Button Found!";
					break;
				}
			}			
		}




		window.clear();
		window.draw(arrowShape);
		window.draw(mouseShape);
		window.display();
	}




	return 0;
}