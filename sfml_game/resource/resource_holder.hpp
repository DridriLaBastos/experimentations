#ifndef TEXTURE_HOLDER_HPP
#define TEXTURE_HOLDER_HPP

#include <map>
#include <memory>

#include <cassert>

template <typename Identifier, typename Resource>
class ResourceHolder
{
public:
	void Load(Identifier id, const std::string& filename)
	{
		std::unique_ptr<Resource> t = std::make_unique<Resource>();
		if (!t->loadFromFile(filename))
		{
			throw std::runtime_error("TextureHolder::Load Failed to load " + filename);
		}

		auto inserted = mResourceMap.emplace(id,std::move(t));
		assert(inserted.second);
	}

	template <typename SecondParameter>
	void Load(Identifier id, const std::string& filename, const SecondParameter& secondParameter)
	{
		std::unique_ptr<Resource> t = std::make_unique<Resource>();
		if (!t->loadFromFile(filename,secondParameter))
		{
			throw std::runtime_error("TextureHolder::Load Failed to load " + filename);
		}

		auto inserted = mResourceMap.emplace(id,std::move(t));
		assert(inserted.second);
	}

	Resource& Get (const Identifier id)
	{
		auto found = mResourceMap.find(id);
		assert(found != mResourceMap.end());
		return *found->second;
	}

	const Resource& Get (const Identifier id) const
	{
		auto found = mResourceMap.find(id);
		assert(found != mResourceMap.end());
		return *found->second;
	}

private:
	std::map<Identifier,std::unique_ptr<Resource> > mResourceMap;
};

#endif
