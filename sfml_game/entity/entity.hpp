#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "scene/scene_node.hpp"

class Entity: public SceneNode
{
public:
	virtual void UpdateCurrent(const sf::Time dt);

	void SetVelocity (const float dx, const float dy);

public:
	sf::Vector2f mVelocity;
};

#endif
