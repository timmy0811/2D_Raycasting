#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <vector>

#include "Ressources.h"

class Floor
{
private:
	sf::Vector2f position;
	sf::Vector2f scale;

	sf::Texture* texture;
	sf::Color color;

	sf::Image texImage;

public:
	// Constructor
	Floor(sf::Vector2f position, sf::Vector2f scale, sf::Color color);
	Floor(sf::Vector2f position, sf::Vector2f scale, sf::Texture* texture);

	// Variables
	bool isTextured;

	// Methods
	void render2D(sf::RenderTarget* target);
	sf::Color getTexelColor(sf::Vector2f worldPos);

	sf::Vector2f getPosition();
	sf::Vector2f getSize();
};

