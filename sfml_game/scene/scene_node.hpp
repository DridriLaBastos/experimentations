#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <memory>
#include <vector>

#include "SFML/Graphics.hpp"

class SceneNode: public sf::Drawable, public sf::Transformable, public sf::NonCopyable
{
public:
	SceneNode(void);

	void Add (std::unique_ptr<SceneNode>&& child);
	std::unique_ptr<SceneNode> Detach (const SceneNode& node);

	void Update(const sf::Time dt);

	sf::Transform GetWorldTransform(void) const;
	sf::Vector2f GetWorldPosition(void) const;

private:
	virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const final;
	virtual void DrawCurrent(sf::RenderTarget& target,sf::RenderStates states) const;
	virtual void UpdateCurrent(const sf::Time dt);

private:
	std::vector<std::unique_ptr<SceneNode>> mChilds;
	SceneNode* mParent;
};

#endif
