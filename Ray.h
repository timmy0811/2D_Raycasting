#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <random>

#include "Boundary.h"

# define M_PI           3.14159265358979323846  /* pi */

class Ray
{
private:
	sf::Vector2f direction;
	sf::Vector2f position;
	sf::Color color;

	float angle;
	float lenght;

	bool pointsOutside;
	sf::Vertex vertex[2];

	std::vector<Boundary*>* boundaries;
	Boundary* intersectBound;

public:
	Ray(sf::Vector2f position, sf::Vector2f direction, std::vector<Boundary*>* boundaries, float angle, sf::Color color = sf::Color::White);

	sf::Vector2f intersect();
	void addPosition(sf::Vector2i position);
	void setPosition(sf::Vector2i position);

	float getLenght();
	float getAngleOffset();
	sf::Vector2f getIntersect();
	bool getOutside();
	Boundary* getIntersectBound();

	void update(sf::RenderTarget* target, float angle);
	void render(sf::RenderTarget* target);
};

