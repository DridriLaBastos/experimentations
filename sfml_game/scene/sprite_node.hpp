#ifndef SPRITE_NODE_HPP
#define SPRITE_NODE_HPP

#include "scene/scene_node.hpp"

class SpriteNode: public SceneNode
{
public:
	explicit SpriteNode(const sf::Texture& texture);
			 SpriteNode(const sf::Texture& texture,const sf::IntRect& rect);
	
	virtual void DrawCurrent(sf::RenderTarget& target,sf::RenderStates states) const;

private:
	sf::Sprite mSprite;
};

#endif
