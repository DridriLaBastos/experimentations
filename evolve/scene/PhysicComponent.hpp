//
// Created by Adrien COURNAND on 03/02/2025.
//

#ifndef PHYSICCOMPONENT_HPP
#define PHYSICCOMPONENT_HPP

#include "box2d/box2d.h"

struct PhysicComponent
{
	PhysicComponent(b2BodyId id): bodyId(id) {}
	b2BodyId bodyId;
};

#endif //PHYSICCOMPONENT_HPP
