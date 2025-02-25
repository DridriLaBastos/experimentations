//
// Created by Adrien COURNAND on 03/02/2025.
//

#ifndef PHYSICCOMPONENT_HPP
#define PHYSICCOMPONENT_HPP

#include "box2d/box2d.h"

struct PhysicComponent
{
	PhysicComponent(b2WorldId worldId, const float radius, const b2Vec2 initalPosition = {.0f, .0f});
	b2BodyId bodyId;
};

#endif //PHYSICCOMPONENT_HPP
