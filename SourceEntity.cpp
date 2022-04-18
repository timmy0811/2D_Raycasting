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

void SourceEntity::updateRays(sf::RenderTarget* target)
{
    for (auto ray : this->rays) {
        ray->update(target, this->viewAngle);
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

void SourceEntity::move(Move_Dir dir, float angle, int MovementSpeed)
{
    this->viewAngle += angle;
    sf::Vector2i dirVector;
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

    dirVector.x = static_cast<int>(cos(radian) * MovementSpeed);
    dirVector.y = static_cast<int>(sin(radian) * MovementSpeed);

    for (auto ray : this->rays) {
        ray->addPosition(dirVector);
    }
}

std::vector<Ray*>* SourceEntity::getRayCollection()
{
    return &this->rays;
}

void SourceEntity::update(sf::RenderTarget* target)
{
    this->updateRays(target);
}

void SourceEntity::render(sf::RenderTarget* target)
{
    this->renderRays(target);
}
