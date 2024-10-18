#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Graphics.hpp"

class Game
{
public:
	Game(void);
	void Run(void);

private:
	void ProcessEvent(void);
	void Update(void);
	void Render(void);

private:
	sf::RenderWindow mWindow;
	sf::CircleShape mPlayer;
};

#endif
