#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include <vector>

#include "Ressources.h"

class Skybox
{
private: 
	float offset;
	float imgSize;
	int popIndex;

	sf::Texture* img;

	std::vector<sf::Sprite*> canvas;

public:
	Skybox(sf::Texture* texture);

	void render(sf::RenderTarget* target);
	void update();
	void move(float amount);
};

