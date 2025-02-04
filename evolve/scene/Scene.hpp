//
// Created by Adrien COURNAND on 03/02/2025.
//

#ifndef WORLD_HPP
#define WORLD_HPP

#include "entt/entt.hpp"
#include "SceneEntity.hpp"

class Scene
{
public:
	SceneEntity SpawnEntity ();
	entt::registry mRegistry;
};

#endif //WORLD_HPP
