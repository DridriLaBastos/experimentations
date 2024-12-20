#include "scene/sprite_node.hpp"

SpriteNode::SpriteNode(const sf::Texture& texture): mSprite(texture)
{

}

SpriteNode::SpriteNode(const sf::Texture& texture,const sf::IntRect& rect): mSprite(texture,rect)
{

}
	
void SpriteNode::DrawCurrent(sf::RenderTarget& target,sf::RenderStates states) const
{
	target.draw(mSprite,states);
}