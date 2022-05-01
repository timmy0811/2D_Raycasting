#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <iostream>
#include <math.h>
#include <assert.h>

#include "SourceEntity.h"
#include "Floor.h"

# define M_PI           3.14159265358979323846  /* pi */

#define CASTFLOOR true

class Cast3DHandler
{
private:
	// Variables
	float colWidth;
	float colHeight;
	float wallHeight;

	float lightSourceAngle;

	int minProjAngle;

	int* virtualHeight;
	int floorCastingRes;

	SourceEntity* entityRef;

	std::vector<sf::VertexArray*> wallVertices;
	std::vector<sf::VertexArray*> floorVertices;

	std::vector<Floor*>* floorRef;

	// Methods
	int clamp(int val, int min, int max);
	sf::Color applyDistanceToRGB(sf::Color color, float deltaFactor);

	Floor* getIntersectFloorTile(sf::Vector3f worldPos);

public:
	// Constructor
	Cast3DHandler(SourceEntity* entity, sf::RenderTarget* window, float wallHeight, int* virtualHeight, int floorCastingRes, std::vector<Floor*>* floorRef);
	~Cast3DHandler();

	// Methods
	void translate(sf::RenderTarget* target);
	void render(sf::RenderTarget* target);
	void renderFloor(sf::RenderTarget* target);
};

