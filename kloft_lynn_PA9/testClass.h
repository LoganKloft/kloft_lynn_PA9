#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>

#include "Enemy.h"
#include "Level.h"

class testClass
{
public:
	testClass(sf::RenderWindow& window)
	{
		if (!m_texture.loadFromFile("sprites/Professor_O_Fallon.png"))
		{
			std::cout << "Professor O'Fallon failed to load O.O" << std::endl;
		}
		testCase1(window);
	}

	// horizontal
	void testCase1(sf::RenderWindow& window)
	{
		bool success = true;

		int tiles[] = {
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,19,2,2,2,2,2,2,2,2,20,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
			18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18
		};

		// expected waypoints
		std::vector<sf::Vector2f> expected_waypoints({ { 1,1 }, { 1,1 } });

		// generated waypoints
		Enemy ProfessorOFallon(m_texture, 1, 1, 2, 1000000);
		ProfessorOFallon.calcWaypoints(tiles);
		std::deque<sf::Vector2f> generated_waypoints = ProfessorOFallon.getWaypoints();

		// compare waypoints
		for (int i = 0; i < generated_waypoints.size(); i++)
		{
			if (generated_waypoints[i].x != expected_waypoints[i].x
				|| generated_waypoints[i].y != expected_waypoints[i].y)
			{
				bool success = false;
				break;
			}
		}

		ProfessorOFallon.printWaypoints();
	}


private:
	sf::Texture m_texture;
};