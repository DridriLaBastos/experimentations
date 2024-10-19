#ifndef TEXTURE_HOLDER_HPP
#define TEXTURE_HOLDER_HPP

#include <map>
#include <memory>

#include "textures/texture_id.hpp"

#include "SFML/Graphics/Texture.hpp"

class TextureHolder
{
public:
	void Load(TextureID id, const std::string& filename);
	sf::Texture& Get (const TextureID id);
	const sf::Texture& Get (const TextureID id) const;

private:
	std::map<TextureID,std::unique_ptr<sf::Texture> > mTextureMap;
};

#endif
