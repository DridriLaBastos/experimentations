//
// Created by Adrien COURNAND on 03/02/2025.
//

#ifndef WORLD_HPP
#define WORLD_HPP

#include "entt/entt.hpp"

class Scene
{
public:
	template<class ComponentType>
	entt::entity Add(ComponentType&& component)
	{
		entt::entity entity = mRegistry.create();
		mRegistry.emplace<ComponentType>(entity, std::forward<ComponentType&&>(component));
		return entity;
	}

	entt::registry mRegistry;
};

#endif //WORLD_HPP
