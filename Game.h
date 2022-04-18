#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "Boundary.h"
#include "Ray.h"
#include "SourceEntity.h"
#include "Cast3DHandler.h"
#include "Ressources.h"

#define MOVEMENTESPEED 6
#define ROTATIONSPEED 5

/*
	Game depending functions
*/

/* TODO
*	Screen Buffer, anstatt mit rects aud aktuellen screen
*		-> Image.create() und darauf pixel setzen, dann window.draw(image)
*/

class Game
{
private:
	// Variables
	std::string window2DTitle = "Window2D";
	std::string window3DTitle = "Window3D";

	sf::Clock clock;
	float lastTime; // testing

	// Window
	sf::RenderWindow* window2D;
	sf::RenderWindow* window3D;
	sf::Event ev;
	sf::VideoMode videoMode2D;
	sf::VideoMode videoMode3D;

	std::map<std::string, sf::Texture*> textures;

	// Private functions
	void initVariables();
	void initWindow();
	void initCastHandler();
	void initObjects();

	void setTitle();
	sf::Color randColor();
	float randfloat(int max, int min = 0);

	// Game Logic

	// Game objects
	Cast3DHandler* handler3D;
	SourceEntity* entity;
	std::vector<Boundary*> bounds;

	struct TileObjects {
		static void genRect(std::vector<Boundary*>* bounds, sf::Vector2f position, sf::Vector2f dimensions, sf::Color color);
		static void genRect(std::vector<Boundary*>* bounds, sf::Vector2f position, sf::Vector2f dimensions, sf::Texture* texture);

		static void genWall(std::vector<Boundary*>* bounds, sf::Vector2f position, sf::Vector2f pointA, sf::Vector2f pointB, sf::Color color);
		static void genWall(std::vector<Boundary*>* bounds, sf::Vector2f position, sf::Vector2f pointA, sf::Vector2f pointB, sf::Texture* texture);
	};

public:
	// Constructors
	Game();
	virtual ~Game();

	// Accessors
	const bool getWindowIsOpen() const;
	std::map<std::string, sf::Texture*>* getTextureMap();

	// Functions
	void pollEvents();
	void update();
	void render();

	void renderBounds(sf::RenderTarget* target);
};

