#include "entity/entity.hpp"

void Entity::UpdateCurrent(const sf::Time dt)
{
	move(mVelocity * dt.asSeconds());
}

void Entity::SetVelocity (const float dx, const float dy)
{
	mVelocity.x = dx;
	mVelocity.y = dy;
}
