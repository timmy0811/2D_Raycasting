#include "Floor.h"

Floor::Floor(sf::Vector2f position, sf::Vector2f scale, sf::Color color)
	:position(position), scale(scale), color(color)
{
	this->texture = nullptr;
	this->isTextured = false;
}

Floor::Floor(sf::Vector2f position, sf::Vector2f scale, sf::Texture* texture)
	:position(position), scale(scale), texture(texture)
{
	this->color = sf::Color::Cyan;

	this->texImage = texture->copyToImage();
	this->isTextured = true;
}

void Floor::render2D(sf::RenderTarget* target)
{
	sf::RectangleShape rect2D = sf::RectangleShape();

	rect2D.setOutlineColor(this->color);
	rect2D.setOutlineThickness(1.f);
	rect2D.setFillColor(sf::Color::Transparent);
	rect2D.setPosition(this->position);
	rect2D.setSize(this->scale);

	target->draw(rect2D);
}

sf::Color Floor::getTexelColor(sf::Vector2f worldPos)
{
	if (!this->isTextured) return this->color;

	sf::Vector2f relativeCoords;

	relativeCoords.x = worldPos.x - this->position.x;
	relativeCoords.y = worldPos.y - this->position.y;

	int texelIndexX = static_cast<int>(relativeCoords.x / Res::unitPerPixelFloor);
	if (texelIndexX >= TexWidth) texelIndexX = texelIndexX - (TexWidth * (texelIndexX / TexWidth));

	int texelIndexY = static_cast<int>(relativeCoords.y / Res::unitPerPixelFloor);
	if (texelIndexY >= TexHeight) texelIndexY = texelIndexY - (TexHeight * (texelIndexY / TexHeight));

	return this->texImage.getPixel(texelIndexX, texelIndexY);
}


sf::Vector2f Floor::getPosition()
{
	return this->position;
}

sf::Vector2f Floor::getSize()
{
	return this->scale;
}
