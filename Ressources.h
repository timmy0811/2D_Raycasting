#pragma once

#include <SFML/Graphics.hpp>

struct Res {
	// Dimensions
	#define TexWidth 64
	#define TexHeight 64

	// Texture enumerator
	enum class Texture { BRICKS = 0, EMPTY };
	//const char* textureString[] = {"BRICKS", "EMPTY"};

	// Variables
	static const int unitPerPixel = 2;

	// Functions
	static void initTextures(std::map<std::string, sf::Texture*>* textures);
};
