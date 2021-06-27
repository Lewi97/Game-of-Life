#include "square.h"
#include "rng.h"
#include <iostream>

//square::square() {};
//
//square::square(float width, float height)
//{
//	shape.setSize({ width, height });
//	shape.setFillColor(sf::Color::Blue);
//}

square newMap[81][61];

sf::Vector2i square::move()
{
	int dir = rng::get().intInRange(0, 3);

	switch (dir)
	{
	case 0:
		return sf::Vector2i({ 0,-1});

	case 1:
		return sf::Vector2i({ 0,1});

	case 2:
		return sf::Vector2i({ -1,0});

	default:
		return sf::Vector2i({ 1,0});

	}
}

sf::Vector2i square::getCellIndex(int cellSize)
{
	int x = shape.getPosition().x / cellSize;
	int y = shape.getPosition().y / cellSize;
	return sf::Vector2i({x,y});
}

void square::updateType()
{
	switch(this->type)
	{
	case 0:
		shape.setFillColor(sf::Color::Black);
		break;

	case 1:
		shape.setFillColor(sf::Color::Blue);
		break;
	}
}

square* square::update(int tileSize, square (*map)[81][61])
{
	int x = getCellIndex(tileSize).x;
	int y = getCellIndex(tileSize).y;

	int neighbours = 0;

	neighbours += (*map)[x - 1][y + 1].getType();
	neighbours += (*map)[x - 1][y - 1].getType();
	neighbours += (*map)[x - 1][y].getType();
	neighbours += (*map)[x + 1][y + 1].getType();
	neighbours += (*map)[x + 1][y - 1].getType();
	neighbours += (*map)[x + 1][y].getType();
	neighbours += (*map)[x][y - 1].getType();
	neighbours += (*map)[x][y + 1].getType();

	if (type == 0 && neighbours == 3) { newMap[x][y].setType(1); }
	else if (type == 1 && (neighbours < 2 || neighbours > 3)) { newMap[x][y].setType(0); }
	else { newMap[x][y].setType(type); }

	std::cout << neighbours << std::endl;

	updated = true;

	return *newMap;
	//move();

	/*if (x < 50.f) { shape.move({ 50.f,0.f }); }
	else if (x > 700.f) { shape.move({ -50.f,0 }); }
	
	if (y < 50.f) { shape.move({ 0,50.f}); }
	else if (y > 500.f) { shape.move({ 0,-50.f }); }*/

}

void square::setType(int nType)
{
	switch (nType)
	{
	case 0:
		type = 0;
		break;
	case 1:
		type = 1;
		break;
	}
	updateType();
}

int square::getType()
{
	return type;
}