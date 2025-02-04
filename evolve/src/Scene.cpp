//
// Created by Adrien COURNAND on 03/02/2025.
//
#include "scene/Scene.hpp"

SceneEntity Scene::SpawnEntity()
{
	return SceneEntity(mRegistry);
}

