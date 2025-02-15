//
// Created by Adrien COURNAND on 03/02/2025.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "SFML/Graphics.hpp"

struct SpriteComponent
{
	SpriteComponent(sf::Texture& texture);
	sf::Sprite sprite;

	explicit operator sf::Sprite () const{ return sprite; }
};

#endif Component//COMPONENT_HPP
