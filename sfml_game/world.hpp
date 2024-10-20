#ifndef WORLD_HPP
#define WORLD_HPP

#include "resource/types.hpp"
#include "entity/aircraft.hpp"
#include "scene/scene_node.hpp"

class World: public sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);
	void Update(const sf::Time dt);
	void Draw(void);

private:
	void LoadTexture(void);
	void BuildScene(void);

private:
	enum class Layer
	{
		Background,Air,LayerCount
	};

private:
	sf::RenderWindow& mWindow;
	sf::View mWorldView;
	TextureHolder mTextures;
	SceneNode mSceneGraph;
	std::array<SceneNode*,static_cast<size_t>(Layer::LayerCount)> mSceneLayers;
	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	float mScrollSpeed;
	Aircraft* mPlayerAircraft;
};

#endif