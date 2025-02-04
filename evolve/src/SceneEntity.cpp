//
// Created by Adrien COURNAND on 04/02/2025.
//
#include "scene/SceneEntity.hpp"

SceneEntity::SceneEntity(entt::registry &registry): mRegistry(registry)
{
	mEntityId = mRegistry.create();
}
