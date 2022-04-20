#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <vector>
#include "Ray.h"

enum class Move_Dir {LEFT, RIGHT, UP, DOWN, DEFAULT};

class SourceEntity
{
private:
	// Variables
	float FOV;
	float viewAngle;
	sf::Vector2f position;

	std::vector<Ray*> rays;

	// Methods
	void setupRays(sf::Vector2f position, int rays, std::vector<Boundary*>* bounds, sf::Color color);
	void updateRays(sf::RenderTarget* target);
	void renderRays(sf::RenderTarget* target);

public:
	// Constructor
	SourceEntity(float FOV, int resolution, sf::Vector2f position, std::vector<Boundary*>* bounds, sf::Color color = sf::Color::White);
	~SourceEntity();

	// Variables
	int resolution;

	// Methods
	void move(Move_Dir dir, float angle, float MovementSpeed);
	std::vector<Ray*>* getRayCollection();

	void update(sf::RenderTarget* target);
	void render(sf::RenderTarget* target);
};

