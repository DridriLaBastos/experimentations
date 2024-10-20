#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Graphics.hpp"

#include "resource/resource_holder.hpp"
#include "resource/resource_id.hpp"

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
	sf::Sprite mPlayer;

	ResourceHolder<TextureID,sf::Texture> mTextures;
	ResourceHolder<FontID,sf::Font> mFonts;

	sf::Text mPerformanceOverlay;

	bool mShouldMoveUp;
	bool mShouldMoveDown;
	bool mShouldMoveLeft;
	bool mShouldMoveRight;
};

#endif
