#include "Skybox.h"

Skybox::Skybox(sf::Texture* texture)
{
	this->img = texture;
	this->offset = 0.f;
	this->popIndex = 0;
	this->imgSize = texture->getSize().x;

	for (int i = 0; i < 3; i++) {
		this->canvas.push_back(new sf::Sprite);
		this->canvas[i]->setTexture(*texture);
		this->canvas[i]->setPosition(sf::Vector2f(-this->imgSize + i * this->imgSize, 0.f));
	}
}

void Skybox::render(sf::RenderTarget* target)
{
	for (int i = 0; i < canvas.size(); i++) {
		target->draw(*canvas[i]);
	}
}

void Skybox::update()
{
	for (int i = 0; i < 3; i++) {
		this->canvas[i]->setPosition(sf::Vector2f(-this->imgSize + i * this->imgSize + this->offset, 0.f));
	}

	if (offset < -this->imgSize / 2) {
		this->offset = this->imgSize / 2;
	}
	else if (offset > this->imgSize / 2) {
		this->offset = -this->imgSize / 2;
	}
}

void Skybox::move(float amount)
{
	this->offset += amount;
}
