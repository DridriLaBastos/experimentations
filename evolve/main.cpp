#include "box2d/box2d.h"

int main(int argc, char const *argv[])
{
	b2WorldDef worlDef = b2DefaultWorldDef();
	b2WorldId worldId = b2CreateWorld(&worlDef);

	b2DestroyWorld(worldId);
	return 0;
}
