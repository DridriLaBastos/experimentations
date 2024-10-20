#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "scene/scene_node.hpp"

class Entity: public SceneNode
{
public:
	void UpdateCurrent(const sf::Time dt);

public:
	sf::Vector2f mVelocity;
};

#endif
