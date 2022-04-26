#pragma once

#include <SFML/Graphics.hpp>

struct Res {
	// Dimensions
	#define TexWidth 16
	#define TexHeight 16

	// Texture enumerator
	enum class Texture { BRICKS = 0, EMPTY };
	//const char* textureString[] = {"BRICKS", "EMPTY"};

	// Variables
	static const int unitPerPixel = 5;		// defines the pixel in world units of one texel
	static const int unitPerPixelFloor = 5;

	// Functions
	static void initTextures(std::map<std::string, sf::Texture*>* textures);
};
