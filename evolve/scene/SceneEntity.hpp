//
// Created by Adrien COURNAND on 04/02/2025.
//

#ifndef SCENEENTITY_HPP
#define SCENEENTITY_HPP

#include "../../../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/System/Library/Frameworks/CoreServices.framework/Frameworks/CarbonCore.framework/Headers/Components.h"
#include "entt/entt.hpp"

class SceneEntity
{
public:
	explicit SceneEntity(entt::registry& registry);

	template<class ComponentType>
	void AddComponent(ComponentType&& component)
	{
		mRegistry.emplace<ComponentType>(mEntityId, std::forward<ComponentType&&>(component));
	}

	template<class ComponentType, class... Args>
	void AddComponent(Args&& ...args)
	{
		AddComponent<ComponentType>(ComponentType{std::forward<Args&&>(args)...});
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
