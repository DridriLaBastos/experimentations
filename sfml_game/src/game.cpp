#include "game.hpp"

Game::Game(): mWindow(sf::VideoMode(640,480),"SFML Game"),mPlayer(),
				mShouldMoveUp(false),mShouldMoveDown(false),mShouldMoveLeft(false),mShouldMoveRight(false)
{
	mPlayer.setRadius(40.f);
	mPlayer.setPosition(100.f,100.f);
	mPlayer.setFillColor(sf::Color::Cyan);
}

void Game::Run()
{
	while(mWindow.isOpen())
	{
		ProcessEvent();
		Update();
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

void Game::Update()
{
	sf::Vector2f movement {0.f, .0f};

	if (mShouldMoveUp) { movement.y -= 1.0; }
	if (mShouldMoveDown) { movement.y += 1.0; }
	if (mShouldMoveLeft) { movement.x -= 1.0; }
	if (mShouldMoveRight) { movement.x += 1.0; }

	mPlayer.move(movement);
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