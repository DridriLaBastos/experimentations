#include <utility>

#include "scene/PhysicComponent.hpp"

static constexpr float MUSCLE_DENSITY = 1060.f;

PhysicComponent::PhysicComponent(const b2WorldId worldId, const float radius, const b2Vec2 initalPosition)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = std::move(initalPosition);
	bodyId = b2CreateBody(worldId,&bodyDef);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = MUSCLE_DENSITY;
	shapeDef.friction = 0.8f;

	b2Circle circleShapeDefinition;
	circleShapeDefinition.radius = radius;
	circleShapeDefinition.center = std::move(initalPosition);
	b2CreateCircleShape(bodyId,&shapeDef,&circleShapeDefinition);
}