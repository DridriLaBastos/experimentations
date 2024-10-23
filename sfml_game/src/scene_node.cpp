#include <cassert>
#include <algorithm>

#include "scene/scene_node.hpp"

SceneNode::SceneNode(void): mChilds(), mParent(nullptr)
{

}

void SceneNode::Add (std::unique_ptr<SceneNode>&& child)
{
	child->mParent = this;
	mChilds.push_back(std::move(child));
}

std::unique_ptr<SceneNode> SceneNode::Detach (const SceneNode& node)
{
	auto found = std::find_if(mChilds.begin(),mChilds.end(),[&](const std::unique_ptr<SceneNode>& ptr)
	{
		return ptr.get() == &node;
	});

	assert(found != mChilds.end());

	std::unique_ptr<SceneNode> removed = std::move(*found);
	removed->mParent = nullptr;

	mChilds.erase(found);
	return removed;
}

void SceneNode::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
	states.transform *= getTransform();
	DrawCurrent(target,states);
	std::for_each(mChilds.begin(),mChilds.end(),[&](const std::unique_ptr<SceneNode>& child)
	{
		child->draw(target,states);
	});
}

void SceneNode::DrawCurrent(sf::RenderTarget& target,sf::RenderStates states) const
{}

void SceneNode::Update(const sf::Time dt)
{
	UpdateCurrent(dt);

	for (auto& child: mChilds)
	{
		child->Update(dt);
	}
}

void SceneNode::UpdateCurrent(const sf::Time dt)
{}

sf::Transform SceneNode::GetWorldTransform(void) const
{
	sf::Transform t = sf::Transform::Identity;

	for(const SceneNode* node = this; node != nullptr; node = node->mParent)
	{
		// Matrix multiply is not associative -> not using *=
		t = node->getTransform() * t;
	}

	return t;
}

sf::Vector2f SceneNode::GetWorldPosition() const
{
	return GetWorldTransform() * sf::Vector2f();
}
