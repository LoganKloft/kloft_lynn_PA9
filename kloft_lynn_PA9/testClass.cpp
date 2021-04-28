#include "testClass.h"

testClass::testClass(sf::RenderWindow& window)
{
	if (!m_texture.loadFromFile("sprites/Professor_O_Fallon.png"))
	{
		std::cout << "Professor O'Fallon failed to load O.O" << std::endl;
	}
	testCase1(window);
	testCase2(window);
	testCase3(window);
	testCase4(window);
	testCase5(window);
	testCase6(window);
}

void testClass::testCase1(sf::RenderWindow& window)
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
	std::vector<sf::Vector2f> expected_waypoints({ { 352, 416 }, { 416, 416 }, { 480, 416 }, { 544, 416 }, { 608, 416 },
		{ 672, 416 }, { 736, 416 }, { 800, 416 }, { 864, 416 }, { 928, 416 } });

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
			success = false;
			break;
		}
	}

	// Create success message
	sf::Font font;
	if (!font.loadFromFile("SugarpunchDEMO.otf"))
	{
		std::cout << "Failed to load font in testCase1" << std::endl;
	}
	sf::Text result("Test Passed!", font, 50);
	result.move(0, 60);
	if (success == false)
	{
		result.setString("Test Failed!");
	}

	// Generate Level
	Level map;
	if (!map.load("tileset.png", sf::Vector2u(64, 64), "horizontal.txt", MAX_LEVEL_WIDTH, MAX_LEVEL_HEIGHT))
	{
		std::cout << "Failed to load level in testCase1" << std::endl;
	}

	sf::Text testNumber("Test 1: horizontal", font, 50);

	sf::Event event;
	while (true)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				return;
				break;
			}
		}

		if (ProfessorOFallon.move() > 0) return;

		window.clear();
		window.draw(map);
		window.draw(ProfessorOFallon);
		window.draw(testNumber);
		window.draw(result);
		window.display();
	}
}

// vertical
void testClass::testCase2(sf::RenderWindow& window)
{
	bool success = true;

	int tiles[] = {
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,19,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,20,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18
	};

	// expected waypoints
	std::vector<sf::Vector2f> expected_waypoints({ { 672, 224 }, { 672, 288 }, { 672, 352 }, { 672, 416 }, { 672, 480 },
		{ 672, 544 }, { 672, 608 }, { 672, 672 }, { 672, 736 } });

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
			success = false;
			break;
		}
	}

	// Create success message
	sf::Font font;
	if (!font.loadFromFile("SugarpunchDEMO.otf"))
	{
		std::cout << "Failed to load font in testCase2" << std::endl;
	}
	sf::Text result("Test Passed!", font, 50);
	result.move(0, 60);
	if (success == false)
	{
		result.setString("Test Failed!");
	}

	// Generate Level
	Level map;
	if (!map.load("tileset.png", sf::Vector2u(64, 64), "vertical.txt", MAX_LEVEL_WIDTH, MAX_LEVEL_HEIGHT))
	{
		std::cout << "Failed to load level in testCase2" << std::endl;
	}

	sf::Text testNumber("Test 2: vertical", font, 50);

	sf::Event event;
	while (true)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				return;
				break;
			}
		}

		if (ProfessorOFallon.move() > 0) return;

		window.clear();
		window.draw(map);
		window.draw(ProfessorOFallon);
		window.draw(testNumber);
		window.draw(result);
		window.display();
	}
}

// topl
void testClass::testCase3(sf::RenderWindow& window)
{
	bool success = true;

	int tiles[] = {
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,8,2,2,20,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,19,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18
	};

	// expected waypoints
	std::vector<sf::Vector2f> expected_waypoints({ { 480, 544 }, { 480, 480 }, { 480, 416 }, { 480, 352 }, { 544, 352 },
		{ 608, 352 }, { 672, 352 } });

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
			success = false;
			break;
		}
	}

	// Create success message
	sf::Font font;
	if (!font.loadFromFile("SugarpunchDEMO.otf"))
	{
		std::cout << "Failed to load font in testCase3" << std::endl;
	}
	sf::Text result("Test Passed!", font, 50);
	result.move(0, 60);
	if (success == false)
	{
		result.setString("Test Failed!");
	}

	// Generate Level
	Level map;
	if (!map.load("tileset.png", sf::Vector2u(64, 64), "topl.txt", MAX_LEVEL_WIDTH, MAX_LEVEL_HEIGHT))
	{
		std::cout << "Failed to load level in testCase3" << std::endl;
	}

	sf::Text testNumber("Test 3: topl", font, 50);

	sf::Event event;
	while (true)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				return;
				break;
			}
		}

		if (ProfessorOFallon.move() > 0) return;

		window.clear();
		window.draw(map);
		window.draw(ProfessorOFallon);
		window.draw(testNumber);
		window.draw(result);
		window.display();
	}
}

// topr
void testClass::testCase4(sf::RenderWindow& window)
{
	bool success = true;

	int tiles[] = {
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,20,2,2,9,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18
	};

	// expected waypoints
	std::vector<sf::Vector2f> expected_waypoints({ { 928, 544 }, { 928, 480 }, { 928, 416 }, { 928, 352 }, { 864, 352 },
		{ 800, 352 }, { 736, 352 } });

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
			success = false;
			break;
		}
	}

	// Create success message
	sf::Font font;
	if (!font.loadFromFile("SugarpunchDEMO.otf"))
	{
		std::cout << "Failed to load font in testCase5" << std::endl;
	}
	sf::Text result("Test Passed!", font, 50);
	result.move(0, 60);
	if (success == false)
	{
		result.setString("Test Failed!");
	}

	// Generate Level
	Level map;
	if (!map.load("tileset.png", sf::Vector2u(64, 64), "topr.txt", MAX_LEVEL_WIDTH, MAX_LEVEL_HEIGHT))
	{
		std::cout << "Failed to load level in testCase4" << std::endl;
	}

	sf::Text testNumber("Test 4: topr", font, 50);

	sf::Event event;
	while (true)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				return;
				break;
			}
		}

		if (ProfessorOFallon.move() > 0) return;

		window.clear();
		window.draw(map);
		window.draw(ProfessorOFallon);
		window.draw(testNumber);
		window.draw(result);
		window.display();
	}
}

// bottoml
void testClass::testCase5(sf::RenderWindow& window)
{
	bool success = true;

	int tiles[] = {
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,19,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,11,2,2,20,18,18,18,18,18,18,18,18,18,18,
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
	std::vector<sf::Vector2f> expected_waypoints({ { 544, 288 }, { 544, 352 }, { 544, 416 }, { 544, 480 }, { 608, 480 },
		{ 672, 480 }, { 736, 480 } });

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
			success = false;
			break;
		}
	}

	// Create success message
	sf::Font font;
	if (!font.loadFromFile("SugarpunchDEMO.otf"))
	{
		std::cout << "Failed to load font in testCase5" << std::endl;
	}
	sf::Text result("Test Passed!", font, 50);
	result.move(0, 60);
	if (success == false)
	{
		result.setString("Test Failed!");
	}

	// Generate Level
	Level map;
	if (!map.load("tileset.png", sf::Vector2u(64, 64), "bottoml.txt", MAX_LEVEL_WIDTH, MAX_LEVEL_HEIGHT))
	{
		std::cout << "Failed to load level in testCase5" << std::endl;
	}

	sf::Text testNumber("Test 5: bottoml", font, 50);

	sf::Event event;
	while (true)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				return;
				break;
			}
		}

		if (ProfessorOFallon.move() > 0) return;

		window.clear();
		window.draw(map);
		window.draw(ProfessorOFallon);
		window.draw(testNumber);
		window.draw(result);
		window.display();
	}
}

// bottomr
void testClass::testCase6(sf::RenderWindow& window)
{
	bool success = true;

	int tiles[] = {
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,19,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,3,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,20,2,2,10,18,18,18,18,18,18,18,18,
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
	std::vector<sf::Vector2f> expected_waypoints({ { 864, 288 }, { 864, 352 }, { 864, 416 }, { 864, 480 }, { 800, 480 },
		{ 736, 480 }, { 672, 480 } });

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
			success = false;
			break;
		}
	}

	// Create success message
	sf::Font font;
	if (!font.loadFromFile("SugarpunchDEMO.otf"))
	{
		std::cout << "Failed to load font in testCase6" << std::endl;
	}
	sf::Text result("Test Passed!", font, 50);
	result.move(0, 60);
	if (success == false)
	{
		result.setString("Test Failed!");
	}

	sf::Text testNumber("Test 6: bottomr", font, 50);

	// Generate Level
	Level map;
	if (!map.load("tileset.png", sf::Vector2u(64, 64), "bottomr.txt", MAX_LEVEL_WIDTH, MAX_LEVEL_HEIGHT))
	{
		std::cout << "Failed to load level in testCase6" << std::endl;
	}

	sf::Event event;
	while (true)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				return;
				break;
			}
		}

		if (ProfessorOFallon.move() > 0) return;

		window.clear();
		window.draw(map);
		window.draw(ProfessorOFallon);
		window.draw(testNumber);
		window.draw(result);
		window.display();
	}
}