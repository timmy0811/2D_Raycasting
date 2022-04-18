
#include "Boundary.h"

Boundary::Boundary(sf::Vector2f pointA, sf::Vector2f pointB, sf::Color color)
	:pA(pointA), pB(pointB), color(color)
{
	this->vertex[0] = sf::Vertex(pointA, color);
	this->vertex[1] = sf::Vertex(pointB, color);
	this->texture = nullptr;

	this->isTextured = false;
}

Boundary::Boundary(sf::Vector2f pointA, sf::Vector2f pointB, sf::Texture* texture)
	:pA(pointA), pB(pointB), color(sf::Color::White), texture(texture)
{
	this->vertex[0] = sf::Vertex(pointA, this->color);
	this->vertex[1] = sf::Vertex(pointB, this->color);

	this->texImage = texture->copyToImage();
	this->isTextured = true;
}

void Boundary::render(sf::RenderTarget* target)
{
	target->draw(vertex, 2, sf::Lines);
}

sf::Vector2f Boundary::getPointsA()
{
	return this->pA;
}

sf::Color Boundary::getColor()
{
	return this->color;
}

sf::Color Boundary::getRGB(sf::Vector2f pointOfIntersection, int indexPixelFromTop)
{
	sf::Vector2f vecToIntersection = sf::Vector2f(pointOfIntersection.x - this->pA.x, pointOfIntersection.y - this->pA.y);
	double distToIntersection = std::sqrt(vecToIntersection.x * vecToIntersection.x + vecToIntersection.y * vecToIntersection.y);

	int tileIndexFromLeft = static_cast<int>(distToIntersection / Res::unitPerPixel);
	if (tileIndexFromLeft >= TexWidth) tileIndexFromLeft = tileIndexFromLeft - (TexWidth * (tileIndexFromLeft / TexWidth));
	sf::Color pixelRGB = this->texImage.getPixel(tileIndexFromLeft, indexPixelFromTop);

	return pixelRGB;
}

sf::Vector2f Boundary::getPointsB()
{
	return this->pB;
}
