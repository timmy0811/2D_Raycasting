#include "Ressources.h"

// Initialize textures

void Res::initTextures(std::map<std::string, sf::Texture*>* textures)
{
    std::map<std::string, sf::Texture*> tex = *textures;

    tex["BRICKS"] = new sf::Texture();
    tex["BRICKS"]->loadFromFile("Assets/BRICKS.png");

    *textures = tex;
}
