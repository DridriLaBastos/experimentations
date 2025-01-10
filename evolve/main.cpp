#include "box2d/box2d.h"

#include "SFML/Graphics.hpp"

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr unsigned int WORLD_CENTER_X = WINDOW_WIDTH / 2;
constexpr unsigned int WORLD_CENTER_Y = WINDOW_HEIGHT / 4;
constexpr float PIXEL_PRE_METER = 5.f;

int main(int argc, char const *argv[])
{
	b2Version version = b2GetVersion();
	printf("Box2D version %d.%d.%d\n", version.major, version.minor, version.revision);
	
	b2WorldDef worlDef = b2DefaultWorldDef();
	worlDef.gravity = (b2Vec2){0.0f, -9.8f};

	b2WorldId worldId = b2CreateWorld(&worlDef);

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.position = (b2Vec2){0.0, 10};
	bodyDef.type = b2BodyType::b2_dynamicBody;
	b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

	b2Polygon polygon = b2MakeRoundedBox(0.3,0.3,0.3);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = .8f;
	b2CreatePolygonShape(bodyId, &shapeDef, &polygon);

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Evolve");
	window.setFramerateLimit(30);

	sf::CircleShape circle (3.0*PIXEL_PRE_METER,16);
	circle.setPosition(40, 40);
	circle.setFillColor(sf::Color::Red);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		window.clear();

		b2Vec2 position = b2Body_GetPosition(bodyId);
		b2World_Step(worldId, 1.0f/30.0f, 4);

		circle.setPosition(WORLD_CENTER_X + position.x * PIXEL_PRE_METER, WORLD_CENTER_Y - position.y * PIXEL_PRE_METER);

		window.draw(circle);

		window.display();
	}

	b2DestroyWorld(worldId);

	return 0;
}
