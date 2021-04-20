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
	sf::RenderWindow window(sf::VideoMode(1440, 1080, 32), "SFML works!", sf::Style::Close);

	sf::RectangleShape shape({ 100,100 });
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && shape.getPosition().x > 0.5)
		{
			shape.move({ -0.5,0.0 });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && shape.getPosition().x + shape.getSize().x < 1440)
		{
			shape.move({ 0.5,0.0 });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && shape.getPosition().y + shape.getSize().y < 1080)
		{
			shape.move({ 0.0,0.5 });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && shape.getPosition().y > 0.5)
		{
			shape.move({ 0.0,-0.5 });
		}


		window.clear();
		window.draw(shape);
		window.display();
	}




	return 0;
}