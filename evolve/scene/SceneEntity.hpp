//
// Created by Adrien COURNAND on 04/02/2025.
//

#ifndef SCENEENTITY_HPP
#define SCENEENTITY_HPP

#include "entt/entt.hpp"

class SceneEntity
{
public:
	explicit SceneEntity(entt::registry& registry);

	template<class ComponentType>
	ComponentType& AddComponent(ComponentType&& component)
	{
		mRegistry.emplace<ComponentType>(mEntityId, std::forward<ComponentType&&>(component));
		return mRegistry.get<ComponentType>(mEntityId);
	}

	template<class ComponentType, class... Args>
	ComponentType& AddComponent(Args&& ...args)
	{
		return AddComponent<ComponentType>(ComponentType{std::forward<Args&&>(args)...});
	}

	template <class ComponentType>
	ComponentType& GetComponent()
	{
		return mRegistry.get<ComponentType>(mEntityId);
	}

private:
	entt::entity mEntityId;
	entt::registry& mRegistry;
};

#endif //SCENEENTITY_HPP
