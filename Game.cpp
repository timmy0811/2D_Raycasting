#include "Game.h"

// Constructors
Game::Game() {
    this->initVariables();
    this->initWindow();

    this->initObjects();
    this->initCastHandler();
}

void Game::setTitle()
{
    float currentTime = clock.restart().asSeconds();
    float fps = 1.f / currentTime;
    lastTime = currentTime;

    this->window2D->setTitle(window2DTitle + " " + std::to_string(static_cast<int>(fps)));
    this->window3D->setTitle(window3DTitle);
}

void Game::initCastHandler()
{
    this->handler3D = new Cast3DHandler(this->entity, this->window3D, 100.f);
}

void Game::initObjects()
{
    this->entity = new SourceEntity(60.f, 64, sf::Vector2f(300, 500), &this->bounds, sf::Color::Blue);

    // Map construction
    TileObjects::genRect(&this->bounds, sf::Vector2f(600.f, 350.f), sf::Vector2f(64, 129), this->textures["BRICKS"]);
    TileObjects::genRect(&this->bounds, sf::Vector2f(300.f, 100.f), sf::Vector2f(100.f, 100.f), sf::Color::Red);
    TileObjects::genRect(&this->bounds, sf::Vector2f(500.f, 100.f), sf::Vector2f(100.f, 100.f), sf::Color::Blue);

    TileObjects::genRect(&this->bounds, sf::Vector2f(0.f, 0.f), sf::Vector2f(static_cast<float>(this->window2D->getSize().x), static_cast<float>(this->window2D->getSize().y)), sf::Color::Magenta);


    }

sf::Color Game::randColor()
{
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;

    return sf::Color(r, g, b, 255);
}

float Game::randfloat(int max, int min)
{
    return static_cast<float>(rand() % static_cast<int>(max) + min);
}

// Destructors
Game::~Game() {
    delete this->window2D;
    delete this->entity;
    delete this->handler3D;

    for (auto bound : this->bounds) {
        delete bound;
    }
}

std::map<std::string, sf::Texture*>* Game::getTextureMap()
{
    return &this->textures;
}

// Private functions

// Init game important variables
void Game::initVariables() {
    srand(static_cast<unsigned int>(time(NULL)));

    this->window2D = nullptr;
    this->window3D = nullptr;

    this->lastTime = 0;

    Res::initTextures(&this->textures);
    sf::Texture tex = *this->textures["BRICKS"];
}

// Init game window
void Game::initWindow() {
    this->videoMode2D.height = 1000;
    this->videoMode2D.width = 1200;

    this->videoMode3D.height = 1000;
    this->videoMode3D.width = 1280;

    // Window for 2D Raycasting
    this->window2D = new sf::RenderWindow(this->videoMode2D, this->window2DTitle, sf::Style::Titlebar | sf::Style::Close);
    //this->window2D->setFramerateLimit(144);

    // Window for 3D Rendering
    this->window3D = new sf::RenderWindow(this->videoMode3D, this->window3DTitle, sf::Style::Titlebar | sf::Style::Close);
    //this->window3D->setFramerateLimit(144);
}

// Accessors

// Check if game is still running
const bool Game::getWindowIsOpen() const {
    return this->window2D->isOpen() && this->window3D->isOpen();
}

// Functions

// Check for events
void Game::pollEvents() {
    // Event polling
    while (this->window3D->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window2D->close();
            this->window3D->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape) {
                this->window2D->close();
                this->window3D->close();
            }
            if (this->ev.key.code == sf::Keyboard::Left) {
                this->entity->move(Move_Dir::LEFT, 0.f, MOVEMENTESPEED);
            }
            if (this->ev.key.code == sf::Keyboard::Right) {
                this->entity->move(Move_Dir::RIGHT, 0.f, MOVEMENTESPEED);
            }
            if (this->ev.key.code == sf::Keyboard::Up) {
                this->entity->move(Move_Dir::UP, 0.f, MOVEMENTESPEED);
            }
            if (this->ev.key.code == sf::Keyboard::Down) {
                this->entity->move(Move_Dir::DOWN, 0.f, MOVEMENTESPEED);
            }
            if (this->ev.key.code == sf::Keyboard::A) {
                this->entity->move(Move_Dir::DEFAULT, -ROTATIONSPEED, 0);
            }
            if (this->ev.key.code == sf::Keyboard::D) {
                this->entity->move(Move_Dir::DEFAULT, ROTATIONSPEED, 0);
            }
            break;
        }
    }
}

// main update method
void Game::update() {
    this->setTitle();
    this->pollEvents();

    this->entity->update(this->window2D);
    this->handler3D->translate(this->window3D);
}

// main render method
void  Game::render() {
    /*
        Renders game objects to screen
    */

    this->window2D->clear(sf::Color(0, 0, 0, 255));
    this->window3D->clear(sf::Color(0, 0, 0, 255));

    this->entity->render(this->window2D);
    this->renderBounds(this->window2D);

    this->handler3D->render(this->window3D);

    this->window2D->display();
    this->window3D->display();
}

void Game::renderBounds(sf::RenderTarget* target)
{
    for (auto boundary : this->bounds) {
        boundary->render(target);
    }
}

void Game::TileObjects::genRect(std::vector<Boundary*>* bounds, sf::Vector2f position, sf::Vector2f dimensions, sf::Color color)
{
    bounds->push_back(new Boundary(position, sf::Vector2f(position.x + dimensions.x, position.y), color));
    bounds->push_back(new Boundary(position, sf::Vector2f(position.x, position.y + dimensions.y), color));
    bounds->push_back(new Boundary(sf::Vector2f(position.x + dimensions.x, position.y), sf::Vector2f(position.x + dimensions.x, position.y + dimensions.y), color));
    bounds->push_back(new Boundary(sf::Vector2f(position.x, position.y + dimensions.y), sf::Vector2f(position.x + dimensions.x, position.y + dimensions.y), color));
}

void Game::TileObjects::genRect(std::vector<Boundary*>* bounds, sf::Vector2f position, sf::Vector2f dimensions, sf::Texture* texture)
{
    bounds->push_back(new Boundary(position, sf::Vector2f(position.x + dimensions.x, position.y), texture));
    bounds->push_back(new Boundary(position, sf::Vector2f(position.x, position.y + dimensions.y), texture));
    bounds->push_back(new Boundary(sf::Vector2f(position.x + dimensions.x, position.y), sf::Vector2f(position.x + dimensions.x, position.y + dimensions.y), texture));
    bounds->push_back(new Boundary(sf::Vector2f(position.x, position.y + dimensions.y), sf::Vector2f(position.x + dimensions.x, position.y + dimensions.y), texture));
}

void Game::TileObjects::genWall(std::vector<Boundary*>* bounds, sf::Vector2f position, sf::Vector2f pointA, sf::Vector2f pointB, sf::Color color)
{
    bounds->push_back(new Boundary(pointA, pointB, color));
}
