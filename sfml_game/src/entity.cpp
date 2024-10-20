#include "entity/entity.hpp"

void Entity::UpdateCurrent(const sf::Time dt)
{
	move(mVelocity * dt.asSeconds());
}
