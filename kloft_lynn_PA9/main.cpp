#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

int main(void)
{
	sf::RenderWindow window(sf::VideoMode(1480, 1080), "Tower Defense", sf::Style::Close);
	sf::CircleShape range(40);
	range.setFillColor(sf::Color::Red);
	while (window.isOpen())
	{
		window.clear();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			range.setPosition(sf::Mouse::getPosition(window).x - range.getRadius(), sf::Mouse::getPosition(window).y - range.getRadius());
			window.draw(range);
		}

		window.display();
	}

	return 0;
}