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
    float fps = 1.f / this->dt;

    this->window3D->setTitle(window3DTitle + " " + std::to_string(static_cast<int>(fps)));
    if (SHOW2D) this->window2D->setTitle(window2DTitle);
}

void Game::initCastHandler()
{
    this->handler3D = new Cast3DHandler(this->entity, this->window3D, 100.f, &this->sourceHeight, FLOORRES, &this->floorTiles);
}

void Game::initObjects()
{
    this->entity = new SourceEntity(60.f, 128, sf::Vector2f(300, 500), &this->bounds, sf::Color::Blue);

    // Map construction
    // Bounds
    TileObjects::genRect(&this->bounds, sf::Vector2f(600.f, 350.f), sf::Vector2f(80, 80), this->textures["BRICKS"]);
    TileObjects::genRect(&this->bounds, sf::Vector2f(700.f, 500.f), sf::Vector2f(80, 80), this->textures["COBBLE"]);

    //TileObjects::genWall(&this->bounds, sf::Vector2f(750, 250), sf::Vector2f(300, 700), this->textures["DARK_PLANK"]);

    TileObjects::genRect(&this->bounds, sf::Vector2f(300.f, 100.f), sf::Vector2f(100.f, 100.f), sf::Color::Red);
    TileObjects::genRect(&this->bounds, sf::Vector2f(500.f, 100.f), sf::Vector2f(100.f, 100.f), sf::Color::Blue);

    //TileObjects::genRect(&this->bounds, sf::Vector2f(0.f, 0.f), sf::Vector2f(static_cast<float>(WIDTH2D), static_cast<float>(HEIGHT2D)), sf::Color::Magenta);

    // Floor
    this->floorTiles.push_back(new Floor(sf::Vector2f(400.f, 400.f), sf::Vector2f(WIDTH2D / 4, HEIGHT2D / 4), this->textures["DARK_PLANK"]));
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
    if (SHOW2D) delete this->window2D;
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
    this->videoMode2D.height = HEIGHT2D;
    this->videoMode2D.width = WIDTH2D;

    this->videoMode3D.height = 1000;
    this->videoMode3D.width = 1280;

    // Window for 2D Raycasting
    if (SHOW2D) {
        this->window2D = new sf::RenderWindow(this->videoMode2D, this->window2DTitle, sf::Style::Titlebar | sf::Style::Close);
        this->window2D->setFramerateLimit(30);
    }
    
    // Window for 3D Rendering
    this->window3D = new sf::RenderWindow(this->videoMode3D, this->window3DTitle, sf::Style::Titlebar | sf::Style::Close);
    this->window3D->setFramerateLimit(30);

    //this->sourceHeight = static_cast<int>(this->window3D->getSize().y / 2);
    this->sourceHeight = -75;
}

// Accessors

// Check if game is still running
const bool Game::getWindowIsOpen() const {
    return this->window3D->isOpen();
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
            if (SHOW2D) this->window2D->close();
            this->window3D->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape) {
                if(SHOW2D) this->window2D->close();
                this->window3D->close();
            }
            break;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        this->entity->move(Move_Dir::LEFT, 0.f, MOVEMENTESPEED * this->dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        this->entity->move(Move_Dir::RIGHT, 0.f, MOVEMENTESPEED * this->dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        this->entity->move(Move_Dir::UP, 0.f, MOVEMENTESPEED * this->dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        this->entity->move(Move_Dir::DOWN, 0.f, MOVEMENTESPEED * this->dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        this->entity->move(Move_Dir::DEFAULT, -ROTATIONSPEED * this->dt, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->entity->move(Move_Dir::DEFAULT, ROTATIONSPEED * this->dt, 0);
    }

    // Increase virtual height
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
        this->sourceHeight += 1;
        std::cout << this->sourceHeight << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        this->sourceHeight -= 1;
        std::cout << this->sourceHeight << std::endl;
    }
}

// main update method
void Game::update() {
    this->dt = clock.restart().asSeconds();
    this->setTitle();
    this->pollEvents();

    this->entity->update(sf::Vector2f(WIDTH2D, HEIGHT2D));
    this->handler3D->translate(this->window3D);
}

// main render method
void  Game::render() {
    /*
        Renders game objects to screen
    */

    this->window3D->clear(sf::Color::White);

    if (SHOW2D) {
        this->window2D->clear(sf::Color(0, 0, 0, 255));

        this->renderFloor(this->window2D);
        this->entity->render(this->window2D);
        this->renderBounds(this->window2D);

        this->window2D->display();
    }
    
    this->handler3D->renderFloor(this->window3D);
    this->handler3D->render(this->window3D);

    this->window3D->display();
}

void Game::renderBounds(sf::RenderTarget* target)
{
    for (auto boundary : this->bounds) {
        boundary->render2D(target);
    }
}

void Game::renderFloor(sf::RenderTarget* target)
{
    for (Floor* tile : this->floorTiles) {
        tile->render2D(target);
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

void Game::TileObjects::genWall(std::vector<Boundary*>* bounds, sf::Vector2f pointA, sf::Vector2f pointB, sf::Color color)
{
    bounds->push_back(new Boundary(pointA, pointB, color));
}

void Game::TileObjects::genWall(std::vector<Boundary*>* bounds, sf::Vector2f pointA, sf::Vector2f pointB, sf::Texture* texture)
{
    bounds->push_back(new Boundary(pointA, pointB, texture));
}
