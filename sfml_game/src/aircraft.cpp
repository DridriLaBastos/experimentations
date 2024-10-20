#include "entity/aircraft.hpp"

static TextureID AircraftType2TextureID(Aircraft::Type type)
{
	switch (type)
	{
	case Aircraft::Type::Eagle:
		return TextureID::Eagle;
	case Aircraft::Type::Raptor:
		return TextureID::Raptor;
	
	default:
		assert(false);
	}
}

Aircraft::Aircraft(const Type type, const TextureHolder& textureHolder): Entity(), mType(type),mSprite(textureHolder.Get(AircraftType2TextureID(type)))
{
	sf::FloatRect localBound = mSprite.getLocalBounds();
	mSprite.setOrigin(localBound.width / 2.f, localBound.height / 2.f);
}

void Aircraft::DrawCurrent(sf::RenderTarget& target,sf::RenderStates states) const
{
	target.draw(mSprite,states);
}
