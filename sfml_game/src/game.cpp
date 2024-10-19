#include "game.hpp"

Game::Game(): mWindow(sf::VideoMode(640,480),"SFML Game"),mPlayer(),
				mShouldMoveUp(false),mShouldMoveDown(false),mShouldMoveLeft(false),mShouldMoveRight(false)
{
	mPlayer.setRadius(40.f);
	mPlayer.setPosition(100.f,100.f);
	mPlayer.setFillColor(sf::Color::Cyan);
}

static constexpr float FPS = 60.0;
static const sf::Time TIME_PER_FRAME = sf::milliseconds(1000.0/FPS);

void Game::Run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while(mWindow.isOpen())
	{
		timeSinceLastUpdate += clock.restart();

		while (timeSinceLastUpdate >= TIME_PER_FRAME)
		{
			ProcessEvent();
			Update(TIME_PER_FRAME);
			timeSinceLastUpdate -= TIME_PER_FRAME;
		}
		
		Render();
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

static constexpr float PLAYER_SPEED = 50.0;

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