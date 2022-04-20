
#include "Ray.h"

// Constructor
Ray::Ray(sf::Vector2f position, sf::Vector2f direction, std::vector<Boundary*>* boundaries, float angle, sf::Color color)
	:position(position), direction(direction), boundaries(boundaries), color(color), angle(angle)
{
	this->vertex[0] = sf::Vertex(position, color);
}

sf::Vector2f Ray::intersect()
{
	std::vector<Boundary*> intersectBounds;
	std::vector<sf::Vector2f> intersects;
	for (Boundary* bound : *boundaries) {
		const float x1 = bound->getPointsA().x;
		const float y1 = bound->getPointsA().y;

		const float x2 = bound->getPointsB().x;
		const float y2 = bound->getPointsB().y;

		const float x3 = this->position.x;
		const float y3 = this->position.y;

		const float x4 = x3 + this->direction.x;
		const float y4 = y3 + this->direction.y;

		float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

		if (denominator == 0) {
			continue;
		}

		float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
		float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

		if (t > 0 && t < 1 && u > 0) {
			sf::Vector2f pt;
			pt.x = x1 + t * (x2 - x1);
			pt.y = y1 + t * (y2 - y1);

			intersects.push_back(pt);
			intersectBounds.push_back(bound);
			continue;
		}
		else {
			continue;
		}
	}

	if (intersects.size() > 0) {
		float closestDistance = 0;
		int index = 0;
		for (int i = 0; i < intersects.size(); i++) {
			sf::Vector2f distanceVec = sf::Vector2f(this->position.x - intersects[i].x, this->position.y - intersects[i].y);
			float distance = static_cast<float>(std::sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y));
			if (distance <= closestDistance || i == 0) {
				closestDistance = distance;
				this->lenght = distance;
				index = i;
			}
		}
		this->intersectBound = intersectBounds[index];
		return intersects[index];
	}
	else {
		return sf::Vector2f(-1.f, -1.f);
	}
}

void Ray::addPosition(sf::Vector2i position)
{
	this->position.x += static_cast<float>(position.x);
	this->position.y += static_cast<float>(position.y);

	this->vertex[0] = sf::Vertex(this->position, this->color);
}

void Ray::setPosition(sf::Vector2i position)
{
	this->position.x = static_cast<float>(position.x);
	this->position.y = static_cast<float>(position.y);

	this->vertex[0] = sf::Vertex(this->position, this->color);
}

float Ray::getLenght()
{
	return this->lenght;
}

float Ray::getAngleOffset()
{
	return this->angle;
}

sf::Vector2f Ray::getIntersect()
{
	return this->vertex[1].position;
}

bool Ray::getOutside()
{
	return this->pointsOutside;
}

Boundary* Ray::getIntersectBound()
{
	return this->intersectBound;
}

void Ray::update(sf::RenderTarget* target, float angleOffset)
{
	float radian = (this->angle + angleOffset) * static_cast<float>(M_PI) / 180.f;

	sf::Vector2f direction;
	direction.x = cos(radian);
	direction.y = sin(radian);

	this->direction = direction;

	sf::Vector2f endPoint = this->intersect();
	if (endPoint.x == -1.f && endPoint.y == -1.f) {
		endPoint = this->position;
		this->lenght = static_cast<float>(sqrt(target->getSize().x * target->getSize().x + target->getSize().y * target->getSize().y));
		this->pointsOutside = true;
		do {
			endPoint.x += this->direction.x;
			endPoint.y += this->direction.y;
		} while (endPoint.x <= target->getSize().x && endPoint.x >= 0
			&& endPoint.y <= target->getSize().y && endPoint.y >= 0);
	}
	else this->pointsOutside = false;
	this->vertex[1] = sf::Vertex(endPoint, this->color);
}

void Ray::render(sf::RenderTarget* target)
{
	target->draw(vertex, 2, sf::Lines);
}
