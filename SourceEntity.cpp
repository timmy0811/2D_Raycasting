#include "SourceEntity.h"

void SourceEntity::setupRays(sf::Vector2f position, int rays, std::vector<Boundary*>* bounds, sf::Color color)
{
    float angleOffset = this->FOV / rays;
    float currentAngle = this->viewAngle - this->FOV / 2;
    for (int ray = 0; ray < rays; ray++) {
        float radian = currentAngle * static_cast<float>(M_PI) / 180.f;

        sf::Vector2f direction;
        direction.x = cos(radian);
        direction.y = sin(radian);

        this->rays.push_back(new Ray(position, direction, bounds, currentAngle, color));

        currentAngle += angleOffset;
    }
}

void SourceEntity::updateRays(sf::Vector2f frameDim)
{
    for (auto ray : this->rays) {
        ray->update(frameDim, this->viewAngle);
    }
}

void SourceEntity::renderRays(sf::RenderTarget* target)
{
    for (auto ray : this->rays) {
        ray->render(target);
    }
}

SourceEntity::SourceEntity(float FOV, int resolution, sf::Vector2f position, std::vector<Boundary*>* bounds, sf::Color color)
    :FOV(FOV), position(position), resolution(resolution)
{
    this->viewAngle = 0.f;

    this->setupRays(position, resolution, bounds, color);
}

SourceEntity::~SourceEntity()
{
    for (auto ray : this->rays) {
        delete ray;
    }
}

void SourceEntity::move(Move_Dir dir, float angle, float MovementSpeed)
{
    this->viewAngle += angle;
    sf::Vector2f dirVector;
    float radian = 0;

    switch (dir) {
    case Move_Dir::LEFT:
        radian = (this->viewAngle - 90) * static_cast<float>(M_PI) / 180;
        break;
    case Move_Dir::RIGHT:
        radian = (this->viewAngle + 90) * static_cast<float>(M_PI) / 180;
        break;
    case Move_Dir::UP:
        radian = this->viewAngle * static_cast<float>(M_PI) / 180;
        break;
    case Move_Dir::DOWN:
        radian = (this->viewAngle - 180) * static_cast<float>(M_PI) / 180;
        break;
    }

    dirVector.x = static_cast<float>(cos(radian) * MovementSpeed);
    dirVector.y = static_cast<float>(sin(radian) * MovementSpeed);

    this->position.x += dirVector.x;
    this->position.y += dirVector.y;

    for (auto ray : this->rays) {
        if (static_cast<int>(angle) == 0) {
            ray->setPosition(sf::Vector2i(static_cast<int>(this->position.x), static_cast<int>(this->position.y)));
        }
    }
}

std::vector<Ray*>* SourceEntity::getRayCollection()
{
    return &this->rays;
}

void SourceEntity::update(sf::Vector2f frameDim)
{
    this->updateRays(frameDim);
}

void SourceEntity::render(sf::RenderTarget* target)
{
    this->renderRays(target);
}
