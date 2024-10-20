#include "world.hpp"

#include "scene/sprite_node.hpp"

#define RESOURCE_PATH(r) (RESOURCE_FOLDER "/" r)

World::World(sf::RenderWindow& window): mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
{
	LoadTexture();
	BuildScene();

	mWorldView.setCenter(mSpawnPosition);
}

void World::LoadTexture()
{
	mTextures.Load(TextureID::Eagle, RESOURCE_PATH("textures/Eagle.png"));
	mTextures.Load(TextureID::Raptor, RESOURCE_PATH("textures/Raptor.png"));
	mTextures.Load(TextureID::Desert, RESOURCE_PATH("textures/Desert.png"));
}

void World::BuildScene(void)
{
	for (size_t i = 0; i < static_cast<size_t>(Layer::LayerCount);i += 1)
	{
		std::unique_ptr<SceneNode> l = std::make_unique<SceneNode>();
		mSceneLayers[i] = l.get();
		mSceneGraph.Add(std::move(l));
	}

	sf::Texture texture = mTextures.Get(TextureID::Desert);
	sf::IntRect textureRect (mWorldBounds);
	texture.setRepeated(false);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left,mWorldBounds.top);
	mSceneLayers[(size_t)Layer::Background]->Add(std::move(backgroundSprite));

	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Type::Eagle, mTextures));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mPlayerAircraft->mVelocity = {40.f, mScrollSpeed};
	mSceneLayers[(size_t)Layer::Air]->Add(std::move(leader));

	std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Type::Raptor, mTextures));
	leftEscort->setPosition(-80.f, 50.f);
	mPlayerAircraft->Add(std::move(leftEscort));

	std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Type::Raptor, mTextures));
	rightEscort->setPosition(80.f, 50.f);
	mPlayerAircraft->Add(std::move(rightEscort));
}

void World::Draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void World::Update(sf::Time dt)
{
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	sf::Vector2f position = mPlayerAircraft->getPosition();
	sf::Vector2f velocity = mPlayerAircraft->mVelocity;
	if (position.x <= mWorldBounds.left + 150
	|| position.x >= mWorldBounds.left + mWorldBounds.width - 150)
	{
		velocity.x = -velocity.x;
		mPlayerAircraft->mVelocity = velocity;
	}

	mSceneGraph.Update(dt);
}
