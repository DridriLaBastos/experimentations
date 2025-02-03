//
// Created by Adrien COURNAND on 03/02/2025.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "SFML/Graphics.hpp"

struct SpriteComponent
{
	SpriteComponent(sf::Texture& texture);
	sf::Sprite mSprite;
};

#endif Component//COMPONENT_HPP
