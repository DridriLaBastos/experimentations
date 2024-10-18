#include "game.hpp"

Game::Game(): mWindow(sf::VideoMode(640,480),"SFML Game"),mPlayer()
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
		
		default:
			break;
		}
	}
}

void Game::Update()
{
}

void Game::Render()
{
	mWindow.clear();
	mWindow.draw(mPlayer);
	mWindow.display();
}