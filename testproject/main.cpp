#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

#define LOG(x) std::cout << x << std::endl;

constexpr int windowWidth{ 1240 }, windowHeight{ 720 };
constexpr int tile_size = 5;
constexpr int simWidth = windowWidth / tile_size;
constexpr int simHeight = windowHeight / tile_size;

bool fps_log = false;

int current[simHeight * simWidth] = { 0 };
int next[simHeight * simWidth] = { 0 };

std::vector<sf::Vertex> vertexPoints;

void batch(float x, float y)
{
	float pixelX = x * tile_size;
	float pixelY = y * tile_size;

	sf::Vertex topLeft({pixelX, pixelY});
	sf::Vertex topRight({ pixelX + tile_size, pixelY });
	sf::Vertex bottomLeft({ pixelX, pixelY + tile_size });
	sf::Vertex bottomRight({ pixelX + tile_size, pixelY + tile_size });

	vertexPoints.push_back(topLeft);
	vertexPoints.push_back(bottomLeft);
	vertexPoints.push_back(bottomRight);
	vertexPoints.push_back(topRight);
}

int get_neighbours(int x, int y)
{
	int n = 0;

	n += current[(x - 1) + simWidth * (y + 1)];	
	n += current[(x - 1) + simWidth * (y - 1)];		
	n += current[(x - 1) + simWidth * (y)];			

	n += current[(x)+simWidth * (y + 1)];			

	n += current[(x)+simWidth * (y - 1)];			
 
	n += current[(x + 1) + simWidth * (y + 1)];		
	n += current[(x + 1) + simWidth * (y - 1)];		
	n += current[(x + 1) + simWidth * (y)];			
	
	return n;
}

void setCellColor(int index, bool state)
{
	sf::Color col = state ? sf::Color::Black : sf::Color::White;
	for (int x = 0; x < 4; x++)
	{
		vertexPoints[index + x].color = col;
	}
}

void updateSim()
{
	for (int x = 1; x < simWidth; x++) {
		for (int y = 1; y < simHeight; y++) {
			int n = get_neighbours(x, y);
			int index = x + simWidth * y;
			if (current[index] == 0 && n == 3) { next[index] = 1;}
			else if (current[index] == 1 && (n < 2 || n > 3)) { next[index] = 0; }
			//else {current[index] = next[index]; setCellColor(index * 4, current[index]);}
		}
	}
	//memcpy(current, next, sizeof(current));
	for (int i = 0; i < simWidth * simHeight; i++) { current[i] = next[i]; setCellColor(i*4, current[i]); }
}

void placeCell(int x, int y)
{
	x /= tile_size;
	y /= tile_size;
	if ((x > 0 && y > 0) && (x < simWidth && y < simHeight)) {
		int index = x + simWidth * y;
		current[index] = 1;
		next[index] = 1;
		setCellColor(index*4, true);
	}
}

int main()
{
	// window stuff
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Game of Life");
	sf::Event event;

	sf::Vertex line[2] =
	{
		sf::Vertex({0,0}, sf::Color::Red, sf::Vector2f(100, 100)),
		sf::Vertex({0,0}, sf::Color::Red, sf::Vector2f(100, 100))
	};

	sf::Clock tick; // TODO || add tickrate, no idea how though
	
	bool horiLine = false;
	bool vertiLine = false;
	bool autoRun = false;

	sf::Vector2i start = { 0,0 };
	sf::Vector2i stop = { 0,0 };

	sf::Clock fps;

	vertexPoints.reserve(simWidth*simHeight*4);
	for (int y = 0; y < simHeight; y++) {
		for (int x = 0; x < simWidth; x++) {
			batch((float)x, (float)y);
			}
		}

	float dt;

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { window.close(); }
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				if (!horiLine && !vertiLine) {placeCell(mPos.x, mPos.y);}
				else { start = mPos; line[0].position = window.mapPixelToCoords(start); }
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				if (!horiLine && !vertiLine) {
					int x = mPos.x / tile_size;
					int y = mPos.y / tile_size;
					int index = x + simWidth * y;
					current[index] = 0;
					next[index] = 0;
					setCellColor(index*4, false);
				}
				else if (horiLine)
				{
					stop = mPos;

					int startX = std::min(start.x, stop.x);
					int stopX = std::max(start.x, stop.x);

					for (int i = startX; i < stopX; i += tile_size) { placeCell(i, stop.y); }
				}
				else if (vertiLine)
				{
					stop = mPos;
					
					int startY = std::min(start.y, stop.y);
					int stopY = std::max(start.y, stop.y);

					for (int i = startY; i < stopY; i += tile_size) { placeCell(stop.x, i); }
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Enter) { updateSim(); }
				if (event.key.code == sf::Keyboard::LShift)
				{
					for (int y = 1; y < simHeight; y++) {
						for (int x = 1; x < simWidth; x++) {
							int index = x + simWidth * y;
							current[index] = 0;
							next[index] = 0;
							setCellColor(index * 4, false);
						}
					}
				}
				if (event.key.code == sf::Keyboard::Space) { autoRun = !autoRun; }
				if (event.key.code == sf::Keyboard::F2) { horiLine = !horiLine; vertiLine = false; 
				line[0].position.x = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x;}
				if (event.key.code == sf::Keyboard::F3) { vertiLine = !vertiLine; horiLine = false;
				line[0].position.x = window.mapPixelToCoords(sf::Mouse::getPosition(window)).y;}
				if (event.key.code == sf::Keyboard::F1){fps_log = !fps_log;}
				if (event.key.code == sf::Keyboard::F6){get_neighbours(100,100);}
				
			}
		}

		if (autoRun) { updateSim(); }

		if (fps_log) 
		{
			dt = fps.restart().asSeconds();
			LOG(1 / dt);
		}

		window.clear(sf::Color::Color(0,0,0, 255));

		window.draw(vertexPoints.data(), vertexPoints.size(), sf::Quads);

		if (vertiLine || horiLine) { 
			line[1].position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

			if(vertiLine){ line[0].position.x = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x; }
			if(horiLine){ line[0].position.y = window.mapPixelToCoords(sf::Mouse::getPosition(window)).y; }
			
			window.draw(line, 2, sf::Lines); 
		}

		window.display();

	}

	return 0;
}