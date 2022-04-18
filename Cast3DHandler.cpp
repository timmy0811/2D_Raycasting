#include "Cast3DHandler.h"

int Cast3DHandler::clamp(int val, int min, int max)
{
	if (val > max) return max;
	else if (val < min) return min;
	else return val;
}

sf::Color Cast3DHandler::applyDistanceToRGB(sf::Color color, float deltaFactor)
{
	unsigned char R = static_cast<char>(color.r * deltaFactor);
	unsigned char G = static_cast<char>(color.g * deltaFactor);
	unsigned char B = static_cast<char>(color.b * deltaFactor);

	return sf::Color(R, G, B, 255);
}

Cast3DHandler::Cast3DHandler(SourceEntity* entity, sf::RenderTarget* window, float wallHeight)
	:entityRef(entity), colHeight(0), wallHeight(wallHeight)
{
	this->colWidth = window->getSize().x / static_cast<float>(entity->resolution);
}

Cast3DHandler::~Cast3DHandler()
{
	this->rects.clear();
	this->rects.shrink_to_fit();
}

void Cast3DHandler::render(sf::RenderTarget* target)
{
	std::cout << this->rects.size() << "   " << this->rects.capacity() << std::endl;
	for (auto renderRect : this->rects) {
		target->draw(*renderRect);
		delete renderRect;
	}
}

void Cast3DHandler::translate(sf::RenderTarget* target)
{
	sf::Vector2u windowSize = target->getSize();

	this->rects.clear();
	this->rects.shrink_to_fit();

	float maxDistance = static_cast<float>(sqrt(windowSize.x * windowSize.x + windowSize.y * windowSize.y));
	float grayscaleFactor = 1.f / maxDistance;
	float heightFactor = windowSize.y / maxDistance;
	std::vector<Ray*>* rays = this->entityRef->getRayCollection();

	for (int rayIndex = 0; rayIndex < rays->size(); rayIndex++) {
		float lenght = (*rays)[rayIndex]->getLenght();
		float lenghtProj = static_cast<float>(cos(abs((*rays)[rayIndex]->getAngleOffset()) / 180 / M_PI) * lenght);

		if ((*rays)[rayIndex]->getOutside()) continue;

		// Calculate rect height
		float height = windowSize.y / lenghtProj * wallHeight;
		float sizeOffsetY = windowSize.y / 2.f - height / 2.f;

		// Texture and color mapping
		Boundary* ptrToBound = (*rays)[rayIndex]->getIntersectBound();
		float deltaFactor = (1 - grayscaleFactor * lenghtProj);
		sf::RectangleShape* colRect;

		if (!ptrToBound->isTextured) {
			// Drawing rects with albedo color
			sf::Color boundColor = ptrToBound->getColor();

			colRect = new sf::RectangleShape();
			colRect->setSize(sf::Vector2f(this->colWidth, height));
			colRect->setPosition(sf::Vector2f(this->colWidth * rayIndex, sizeOffsetY));
			colRect->setFillColor(this->applyDistanceToRGB(boundColor, deltaFactor));

			this->rects.push_back(colRect);
		}
		else if (height >= TexHeight) {
			for (int i = 0; i < TexHeight; i++) {
				// Drawing rects with Texture map
				colRect = new sf::RectangleShape();
				float rectHeight = height / TexHeight;
				colRect->setSize(sf::Vector2f(this->colWidth, rectHeight));
				colRect->setPosition(sf::Vector2f(this->colWidth * rayIndex, sizeOffsetY + rectHeight * i));
				sf::Vector2f inters = (*rays)[rayIndex]->getIntersect();
				colRect->setFillColor(this->applyDistanceToRGB(ptrToBound->getRGB(inters, i), deltaFactor));

				this->rects.push_back(colRect);
			}
		}
	}
}
