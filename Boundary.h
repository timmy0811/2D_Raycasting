#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <vector>

#include "Ressources.h"

class Boundary
{
private:
	sf::Vector2f pA;
	sf::Vector2f pB;

	sf::Texture* texture;
	sf::Vertex vertex[2];
	sf::Color color;

	sf::Image texImage;

public:
	// Constructor
	Boundary(sf::Vector2f pointA, sf::Vector2f pointB, sf::Color color);
	Boundary(sf::Vector2f pointA, sf::Vector2f pointB, sf::Texture* texture);

	// Variables
	bool isTextured;

	// Geter
	sf::Vector2f getPointsA();
	sf::Vector2f getPointsB();

	sf::Color getColor();
	sf::Color getRGB(sf::Vector2f pointOfIntersection, int indexPixelFromTop);

	// Methods
	void render(sf::RenderTarget* target);
};

