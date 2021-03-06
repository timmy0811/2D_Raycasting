#include "Ressources.h"

// Initialize textures

void Res::initTextures(std::map<std::string, sf::Texture*>* textures)
{
    std::map<std::string, sf::Texture*> tex = *textures;

    tex["BRICKS"] = new sf::Texture();
    tex["BRICKS"]->loadFromFile("Assets/block/grass_block_side.png");

    tex["AMETHYST"] = new sf::Texture();
    tex["AMETHYST"]->loadFromFile("Assets/block/large_amethyst_bud.png");

    tex["COBBLE"] = new sf::Texture();
    tex["COBBLE"]->loadFromFile("Assets/block/cobblestone.png");

    tex["DARK_PLANK"] = new sf::Texture();
    tex["DARK_PLANK"]->loadFromFile("Assets/block/dark_oak_planks.png");

    tex["SAND"] = new sf::Texture();
    tex["SAND"]->loadFromFile("Assets/block/sand.png");

    tex["RED_SAND"] = new sf::Texture();
    tex["RED_SAND"]->loadFromFile("Assets/block/red_sand.png");

    *textures = tex;
}

sf::Texture* Res::getSkybox()
{
    sf::Texture* tempTex = new sf::Texture();
    tempTex->loadFromFile("Assets/sky2.png");
    return tempTex;
}
