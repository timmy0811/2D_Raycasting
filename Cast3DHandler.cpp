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

Floor* Cast3DHandler::getIntersectFloorTile(sf::Vector3f worldPos)
{
	// Can be expanded by Z dimension for floor height
	for (Floor* floorTile : *this->floorRef) {
		if (worldPos.x > floorTile->getPosition().x && worldPos.x < floorTile->getPosition().x + floorTile->getSize().x
			&& worldPos.y > floorTile->getPosition().y && worldPos.y < floorTile->getPosition().y + floorTile->getSize().y) {
			return floorTile;
		}
	}
	return nullptr;
}

Cast3DHandler::Cast3DHandler(SourceEntity* entity, sf::RenderTarget* window, float wallHeight, int* virtualHeight, int floorCastingRes, std::vector<Floor*>* floorRef)
	:entityRef(entity), colHeight(0), wallHeight(wallHeight), virtualHeight(virtualHeight), floorCastingRes(floorCastingRes), floorRef(floorRef)
{
	this->minProjAngle = 45;
	this->colWidth = window->getSize().x / static_cast<float>(entity->resolution);
}

Cast3DHandler::~Cast3DHandler()
{
	this->floorVertices.clear();
	this->floorVertices.shrink_to_fit();

	this->wallVertices.clear();
	this->wallVertices.shrink_to_fit();
}

void Cast3DHandler::render(sf::RenderTarget* target)
{
	for (sf::VertexArray* texel : this->wallVertices) {
		target->draw(*texel);
		delete texel;
	}
}

void Cast3DHandler::renderFloor(sf::RenderTarget* target)
{
	for (sf::VertexArray* texel : this->floorVertices) {
		target->draw(*texel);
		delete texel;
	}
}

void Cast3DHandler::translate(sf::RenderTarget* target)
{
	sf::Vector2u windowSize = target->getSize();
	sf::Vector2i sourcePosition = sf::Vector2i(this->entityRef->getPosition());		// Das funktioniert?

	sf::Vector3f positionVector = sf::Vector3f(
		(float)sourcePosition.x,
		(float)sourcePosition.y,
		(float)(*this->virtualHeight)
	);

	int floorTexelHeight = (windowSize.y / 2) / floorCastingRes;

	this->wallVertices.clear();
	this->wallVertices.shrink_to_fit();

	this->floorVertices.clear();
	this->floorVertices.shrink_to_fit();

	float maxDistance = static_cast<float>(sqrt(windowSize.x * windowSize.x + windowSize.y * windowSize.y));
	float grayscaleFactor = 1.f / maxDistance;
	float heightFactor = windowSize.y / maxDistance;
	std::vector<Ray*>* rays = this->entityRef->getRayCollection();

	// Amount of degree each ray is offset by
	float anglePerRayX = entityRef->getFOV() / rays->size();
	float anglePerRayY = (180.f - this->minProjAngle - 90.f) / floorCastingRes;

	for (int rayIndex = 0; rayIndex < rays->size(); rayIndex++) {
		float lenght = (*rays)[rayIndex]->getLenght();
		float lenghtProj = static_cast<float>(cos(abs((*rays)[rayIndex]->getAngleOffset()) / 180 / M_PI) * lenght);

		if ((*rays)[rayIndex]->getOutside()) continue;																							// <---- HERE FIX

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

			this->wallVertices.push_back(texel);
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
				this->wallVertices.push_back(texel);
			}
		}

		// floor casting
		int upperFloorEdge = sizeOffsetY + height;

		if ((unsigned)upperFloorEdge < windowSize.y) {
			// Calculate the amount of rays for col
			int colRays = (windowSize.y - upperFloorEdge) / floorTexelHeight;
			if (colRays <= 0) colRays = 1;

			// Ceil pixel placement to upper multiple of the rows height
			assert(floorTexelHeight);
			int isPositive = (int)(upperFloorEdge >= 0);
			int startAt = (((upperFloorEdge + isPositive * (floorTexelHeight - 1)) / floorTexelHeight) * floorTexelHeight) - floorTexelHeight;

			// Iterate every ray in col
			for (int floorIndexY = 0; floorIndexY < colRays + 2; floorIndexY++) {
				if ((unsigned)(startAt + floorTexelHeight * floorIndexY) > windowSize.y) break;

				// Vector angle from camera to floor texel
				float angleAlpha = this->entityRef->getViewAngle() - (rays->size() / 2.f) * anglePerRayX + rayIndex * anglePerRayX + anglePerRayX / 2.f;		// angle around z axis (rotation)
				float angleBeta = 90 - ((floorCastingRes - colRays) * anglePerRayY + floorIndexY * anglePerRayY + anglePerRayY / 2.f);							// angle pointing to floor

				sf::Vector3f rayVector = sf::Vector3f(
					cos(angleAlpha * M_PI / 180),
					sin(angleAlpha * M_PI / 180),
					-cos(angleBeta * M_PI / 180)
				);

				// r factor of vector
				float r = positionVector.z / rayVector.z;

				// intersection with xy plane
				sf::Vector3f intersVector = sf::Vector3f(
					positionVector.x + rayVector.x * r,
					positionVector.y + rayVector.y * r,
					positionVector.z + rayVector.z * r
				);

				// Optimization by breaking when intersPoint is out of screen
				Floor* intersectingFloorTile = this->getIntersectFloorTile(intersVector);
				sf::Color texelColor;

				if (intersectingFloorTile == nullptr) {
					continue;
				}
				else {
					texelColor = intersectingFloorTile->getTexelColor(sf::Vector2f(intersVector.x, intersVector.y));
				}

				sf::VertexArray* floorTexel = new sf::VertexArray(sf::Quads, 4);

				(*floorTexel)[0].position = sf::Vector2f(this->colWidth * rayIndex, startAt + floorTexelHeight * floorIndexY);
				(*floorTexel)[1].position = sf::Vector2f(this->colWidth * rayIndex + this->colWidth, startAt + floorTexelHeight * floorIndexY);
				(*floorTexel)[2].position = sf::Vector2f(this->colWidth * rayIndex + this->colWidth, startAt + floorTexelHeight * floorIndexY + floorTexelHeight);
				(*floorTexel)[3].position = sf::Vector2f(this->colWidth * rayIndex, startAt + floorTexelHeight * floorIndexY + floorTexelHeight);

				for (int j = 0; j < 4; j++) {
					(*floorTexel)[j].color = texelColor;
				}

				this->floorVertices.push_back(floorTexel);
			}
		}
		
	}
}
