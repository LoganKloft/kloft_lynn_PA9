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
	testClass(sf::RenderWindow& window);

private:
	sf::Texture m_texture;

	// horizontal
	void testCase1(sf::RenderWindow& window);

	// vertical
	void testCase2(sf::RenderWindow& window);

	// topl
	void testCase3(sf::RenderWindow& window);

	// topr
	void testCase4(sf::RenderWindow& window);

	// bottoml
	void testCase5(sf::RenderWindow& window);

	// bottomr
	void testCase6(sf::RenderWindow& window);
};