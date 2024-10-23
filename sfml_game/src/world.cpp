#include "world.hpp"

#include "scene/sprite_node.hpp"

#define RESOURCE_PATH(r) (RESOURCE_FOLDER "/" r)

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
{
	LoadTextures();
	BuildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::Update(sf::Time dt)
{
	// Scroll the world
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());	

	// Move the player sidewards (plane scouts follow the main aircraft)
	sf::Vector2f position = mPlayerAircraft->getPosition();
	sf::Vector2f velocity = mPlayerAircraft->mVelocity;

	// If player touches borders, flip its X velocity
	if (position.x <= mWorldBounds.left + 150.f
	 || position.x >= mWorldBounds.left + mWorldBounds.width - 150.f)
	{
		velocity.x = -velocity.x;
		mPlayerAircraft->mVelocity = velocity;
	}

	// Apply movements
	mSceneGraph.Update(dt);
}

void World::Draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void World::LoadTextures()
{
	mTextures.Load(TextureID::Eagle, RESOURCE_PATH("textures/Eagle.png"));
	mTextures.Load(TextureID::Raptor, RESOURCE_PATH("textures/Raptor.png"));
	mTextures.Load(TextureID::Desert, RESOURCE_PATH("textures/Desert.png"));
}

void World::BuildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		std::unique_ptr<SceneNode> layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.Add(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& texture = mTextures.Get(TextureID::Desert);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->Add(std::move(backgroundSprite));

	// Add player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Type::Eagle, mTextures));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mPlayerAircraft->SetVelocity(40.f, mScrollSpeed);
	mSceneLayers[Air]->Add(std::move(leader));

	// Add two escorting aircrafts, placed relatively to the main plane
	std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Type::Raptor, mTextures));
	leftEscort->setPosition(-80.f, 50.f);
	mPlayerAircraft->Add(std::move(leftEscort));

	std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Type::Raptor, mTextures));
	rightEscort->setPosition(80.f, 50.f); 
	mPlayerAircraft->Add(std::move(rightEscort));
}
