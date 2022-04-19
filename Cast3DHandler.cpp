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
	for (sf::VertexArray* texel : this->texels) {
		target->draw(*texel);
		delete texel;
	}
}

void Cast3DHandler::translate(sf::RenderTarget* target)
{
	sf::Vector2u windowSize = target->getSize();

	this->texels.clear();
	this->texels.shrink_to_fit();

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


		if (!ptrToBound->isTextured) {
			sf::VertexArray* texel = new sf::VertexArray(sf::Quads, 4);

			// Texel position and scale
			(*texel)[0].position = sf::Vector2f(this->colWidth * rayIndex, sizeOffsetY);
			(*texel)[1].position = sf::Vector2f(this->colWidth * rayIndex + this->colWidth, sizeOffsetY);
			(*texel)[2].position = sf::Vector2f(this->colWidth * rayIndex + this->colWidth, sizeOffsetY + height);
			(*texel)[3].position = sf::Vector2f(this->colWidth * rayIndex, sizeOffsetY + height);

			// Texel color
			sf::Color col = this->applyDistanceToRGB(ptrToBound->getColor(), deltaFactor);
			for (int i = 0; i < 4; i++) {
				(*texel)[i].color = col;
			}

			this->texels.push_back(texel);
		}
		else if (height >= TexHeight) {
			for (int i = 0; i < TexHeight; i++) {
				sf::VertexArray* texel = new sf::VertexArray(sf::Quads, 4);

				// Texel position and scale
				float rectHeight = height / TexHeight;

				(*texel)[0].position = sf::Vector2f(this->colWidth * rayIndex, sizeOffsetY + rectHeight * i);
				(*texel)[1].position = sf::Vector2f(this->colWidth * rayIndex + this->colWidth, sizeOffsetY + rectHeight * i);
				(*texel)[2].position = sf::Vector2f(this->colWidth * rayIndex + this->colWidth, sizeOffsetY + rectHeight * i + rectHeight);
				(*texel)[3].position = sf::Vector2f(this->colWidth * rayIndex, sizeOffsetY + rectHeight * i + rectHeight);

				// Texel color
				sf::Vector2f inters = (*rays)[rayIndex]->getIntersect();

				sf::Color col = this->applyDistanceToRGB(ptrToBound->getRGB(inters, i), deltaFactor);
				for (int j = 0; j < 4; j++) {
					(*texel)[j].color = col;
				}

				this->texels.push_back(texel);
			}
		}

	}
}
