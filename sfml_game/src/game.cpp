#include "game.hpp"

#define RESOURCE_PATH(r) (RESOURCE_FOLDER "/" r)

Game::Game(): mWindow(sf::VideoMode(640,480),"SFML Game"),mPlayer(),mTextures(),mFonts(),mPerformanceOverlay(),
				mShouldMoveUp(false),mShouldMoveDown(false),mShouldMoveLeft(false),mShouldMoveRight(false)
{
	mTextures.Load(TextureID::Airplane,RESOURCE_PATH("textures/Eagle.png"));

	mPlayer.setTexture(mTextures.Get(TextureID::Airplane));
	mPlayer.setPosition(100.0,100.0);

	mFonts.Load(FontID::Sansation,RESOURCE_PATH("fonts/Sansation.ttf"));

	mPerformanceOverlay.setFont(mFonts.Get(FontID::Sansation));
}

static constexpr float FPS = 60.0;
static const sf::Time TIME_PER_FRAME = sf::milliseconds(1000.0/FPS);

static unsigned int sRenderTick = 0;
static unsigned int sUpdateTicks = 0;
static char buffer[256];

void Game::Run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time elapsed = sf::Time::Zero;
	memset(buffer,0,sizeof(buffer));

	mPerformanceOverlay.setPosition(0,0);

	mPerformanceOverlay.setPosition(5,5);
	mPerformanceOverlay.setCharacterSize(13);

	mWindow.setFramerateLimit(FPS);

	while(mWindow.isOpen())
	{
		const sf::Time dt = clock.restart();

		timeSinceLastUpdate += dt;
		elapsed += dt;

		while (timeSinceLastUpdate >= TIME_PER_FRAME)
		{
			ProcessEvent();
			Update(TIME_PER_FRAME);
			timeSinceLastUpdate -= TIME_PER_FRAME;
			sUpdateTicks += 1;
		}
		
		sRenderTick += 1;
		Render();

		if (elapsed >= sf::seconds(1))
		{
			snprintf(buffer,sizeof(buffer), "FPS : %d\nTPS %d\n",sRenderTick,sUpdateTicks);
			mPerformanceOverlay.setString(buffer);
			sRenderTick = 0;
			sUpdateTicks = 0;
			elapsed -= sf::seconds(1);
		}
	}
}

void Game::ProcessEvent()
{
	sf::Event event;

	while(mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			mWindow.close();
			break;
		
		case sf::Event::KeyPressed:
			HandlePlayerInput(event.key.code,true);
			break;
		
		case sf::Event::KeyReleased:
			HandlePlayerInput(event.key.code,false);
			break;
		
		default:
			break;
		}
	}
}

static constexpr float PLAYER_SPEED = 200.0;

void Game::Update(const sf::Time dt)
{
	sf::Vector2f movement {0.f, .0f};

	if (mShouldMoveUp) { movement.y -= PLAYER_SPEED; }
	if (mShouldMoveDown) { movement.y += PLAYER_SPEED; }
	if (mShouldMoveLeft) { movement.x -= PLAYER_SPEED; }
	if (mShouldMoveRight) { movement.x += PLAYER_SPEED; }

	mPlayer.move(movement * dt.asSeconds());
}

void Game::Render()
{
	mWindow.clear();
	mWindow.draw(mPlayer);
	mWindow.draw(mPerformanceOverlay);
	mWindow.display();
}

void Game::HandlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if ((key == sf::Keyboard::Z) || (key == sf::Keyboard::Up))
		mShouldMoveUp = isPressed;
	if ((key == sf::Keyboard::S) || (key == sf::Keyboard::Down))
		mShouldMoveDown = isPressed;
	if ((key == sf::Keyboard::Q) || (key == sf::Keyboard::Left))
		mShouldMoveLeft = isPressed;
	if ((key == sf::Keyboard::D) || (key == sf::Keyboard::Right))
		mShouldMoveRight = isPressed;
}