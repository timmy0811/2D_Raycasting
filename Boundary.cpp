
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

void Boundary::render2D(sf::RenderTarget* target)
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

float Boundary::getOrientationFace(sf::Vector2f sourcePos, float lightAngle)
{
	sf::Vector2f midPoint;
	midPoint.x = this->vertex[0].position.x + (this->vertex[1].position.x - this->vertex[0].position.x) / 2;
	midPoint.y = this->vertex[0].position.y + (this->vertex[1].position.y - this->vertex[0].position.y) / 2;

	sf::Vector2f angledVec = sourcePos - midPoint;

	sf::Vector2f normPos;
	normPos.x = -(this->vertex[1].position.y - this->vertex[0].position.y);
	normPos.y = (this->vertex[1].position.x - this->vertex[0].position.x);

	sf::Vector2f normNeg = -normPos;

	float angleBetween = acos((angledVec.x * normPos.x + angledVec.y * normPos.y) /
		(std::sqrt(angledVec.x * angledVec.x + angledVec.y * angledVec.y) * std::sqrt(normPos.x * normPos.x + normPos.y * normPos.y)));

	angleBetween *= (180.f / static_cast<float>(M_PI));

	sf::Vector2f* facingToSource;
	if (angleBetween > -90 && angleBetween < 90) {
		facingToSource = &normPos;
	}
	else {
		facingToSource = &normNeg;
	}

	sf::Vector2f lightDir;
	lightDir.x = cos(lightAngle * static_cast<float>(M_PI) / 180);
	lightDir.y = sin(lightAngle * static_cast<float>(M_PI) / 180);

	float angleToLight = acos((facingToSource->x * lightDir.x + facingToSource->y * lightDir.y) /
		(std::sqrt(facingToSource->x * facingToSource->x + facingToSource->y * facingToSource->y) * std::sqrt(lightDir.x * lightDir.x + lightDir.y * lightDir.y)));

	angleToLight *= (180 / static_cast<float>(M_PI));
	if (abs(angleToLight) >= 180) angleToLight = 360 - abs(angleToLight);

	return angleToLight * 1 / 180;
}

sf::Vector2f Boundary::getPointsB()
{
	return this->pB;
}
