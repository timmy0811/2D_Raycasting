#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "Boundary.h"
#include "Floor.h"
#include "Ray.h"
#include "SourceEntity.h"
#include "Cast3DHandler.h"
#include "Ressources.h"

#define MOVEMENTESPEED 100
#define ROTATIONSPEED 45

#define SHOW2D false

// Defines also the worlds global dimensions
#define WIDTH2D 1200	
#define HEIGHT2D 1000

#define FLOORRES 32

/* TODO
*	-
*/

class Game
{
private:
	// Variables
	std::string window2DTitle = "Window2D";
	std::string window3DTitle = "Window3D";

	sf::Clock clock;
	float lastTime;

	// Window
	sf::RenderWindow* window2D;
	sf::RenderWindow* window3D;
	sf::Event ev;
	sf::VideoMode videoMode2D;
	sf::VideoMode videoMode3D;

	float dt;

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
	int sourceHeight;

	// Game objects
	Cast3DHandler* handler3D;
	SourceEntity* entity;

	std::vector<Floor*> floorTiles;
	std::vector<Boundary*> bounds;

	struct TileObjects {
		static void genRect(std::vector<Boundary*>* bounds, sf::Vector2f position, sf::Vector2f dimensions, sf::Color color);
		static void genRect(std::vector<Boundary*>* bounds, sf::Vector2f position, sf::Vector2f dimensions, sf::Texture* texture);

		static void genWall(std::vector<Boundary*>* bounds, sf::Vector2f pointA, sf::Vector2f pointB, sf::Color color);
		static void genWall(std::vector<Boundary*>* bounds, sf::Vector2f pointA, sf::Vector2f pointB, sf::Texture* texture);
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
	void renderFloor(sf::RenderTarget* target);
};

