#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include "entity/entity.hpp"
#include "resource/types.hpp"

class Aircraft: public Entity
{
public:
	enum class Type
	{
		Eagle, Raptor
	};

public:
	explicit Aircraft(const Type type, const TextureHolder& textureHolder);

	virtual void DrawCurrent(sf::RenderTarget& target,sf::RenderStates states) const;

private:
	Type mType;
	sf::Sprite mSprite;
};

#endif
