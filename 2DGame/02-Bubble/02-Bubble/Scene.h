#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Paddle.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Text.h"
#include "Winnie.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	Winnie *winnie;
	TileMap *map;
	Player *player;
	Ball *ball;
	Paddle *paddle;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;

};


#endif // _SCENE_INCLUDE

