#include <cassert>

#include "textures/texture_holder.hpp"

void TextureHolder::Load(TextureID id, const std::string& filename)
{
	std::unique_ptr<sf::Texture> t = std::make_unique<sf::Texture>();
	if (!t->loadFromFile(filename))
	{
		throw std::runtime_error("TextureHolder::Load Failed to load " + filename);
	}

	auto inserted = mTextureMap.emplace(id,std::move(t));
	assert(inserted.second);
}

sf::Texture& TextureHolder::Get(const TextureID id)
{
	auto found = mTextureMap.find(id);
	assert(found != mTextureMap.end());
	return *found->second;
}

const sf::Texture& TextureHolder::Get(const TextureID id) const
{
	auto found = mTextureMap.find(id);
	assert(found != mTextureMap.end());
	return *found->second;
}
