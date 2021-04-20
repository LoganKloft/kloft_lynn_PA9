#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "Level.h"
#include "Button.h"

class LevelPlayer
{
public:
	void run(sf::RenderWindow* window);
private:
	int gold;
	int health;
	bool startNextWave;

	std::string selectLevel();
};