#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <iostream>

#include "SourceEntity.h"

# define M_PI           3.14159265358979323846  /* pi */

class Cast3DHandler
{
private:
	// Variables
	float colWidth;
	float colHeight;
	float wallHeight;

	SourceEntity* entityRef;

	std::vector<sf::RectangleShape*> rects;
	std::vector<sf::VertexArray*> texels;

	// Methods
	int clamp(int val, int min, int max);
	sf::Color applyDistanceToRGB(sf::Color color, float deltaFactor);

public:
	// Constructor
	Cast3DHandler(SourceEntity* entity, sf::RenderTarget* window, float wallHeight);
	~Cast3DHandler();

	// Methods
	void translate(sf::RenderTarget* target);
	void render(sf::RenderTarget* target);
};

