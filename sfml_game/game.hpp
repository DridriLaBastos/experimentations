#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Graphics.hpp"

#include "world.hpp"

class Game
{
public:
	Game(void);
	void Run(void);

private:
	void ProcessEvent(void);
	void Update(const sf::Time dt);
	void Render(void);

	void HandlePlayerInput(sf::Keyboard::Key key,bool isPressed);

private:
	sf::RenderWindow mWindow;

	World mWorld;

	sf::Text mPerformanceOverlay;

	bool mShouldMoveUp;
	bool mShouldMoveDown;
	bool mShouldMoveLeft;
	bool mShouldMoveRight;
};

#endif
