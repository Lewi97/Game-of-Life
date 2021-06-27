#pragma once
#include <SFML/Graphics.hpp>

class square
{
private:
	int type = 0;
	void updateType();
	
public:

	// constructor
	square() { }
	square(float width, float height)
	{
		shape.setSize({ width, height });
		shape.setFillColor(sf::Color::Blue);
	}

	// Methods
	sf::Vector2i move();
	sf::Vector2i getCellIndex(int cellSize);
	void setType(int type);
	int getType();
	square* update(int tileSize, square(*array)[81][61]);

	// Vars
	sf::RectangleShape shape;
	bool updated = false;
};

