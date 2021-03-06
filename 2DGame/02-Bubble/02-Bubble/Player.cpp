#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"





enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, STAND_UP, STAND_DOWN, DIE
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;

	spritesheet.loadFromFile("images/wasp2.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.20), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(STAND_LEFT, 8);

	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.6f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.2f));

	sprite->setAnimationSpeed(STAND_UP, 8);
	sprite->addKeyframe(STAND_UP, glm::vec2(0.25f, 0.4f));

	sprite->setAnimationSpeed(STAND_DOWN, 8);
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);

	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.2f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.2f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.2f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.4f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.4f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50, 0.4f));

	sprite->setAnimationSpeed(MOVE_UP, 8);
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.f, 0.6f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.25, 0.6f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.50, 0.6f));


	sprite->setAnimationSpeed(MOVE_DOWN, 8);
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.50, 0.f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.f, 0.8f));
	sprite->addKeyframe(DIE, glm::vec2(0.25, 0.8f));
	sprite->addKeyframe(DIE, glm::vec2(0.50, 0.8f));
	sprite->addKeyframe(DIE, glm::vec2(0.75, 0.8f));

	sprite->changeAnimation(MOVE_DOWN);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime, bool restart, bool collision, int offset, int offsetR)
{
	if (restart) {
		sprite->changeAnimation(DIE);
		posPlayer.x = 212;
		posPlayer.y = 400;
		backing = true;
		Game::instance().playSound("sounds/die.mp3");
	}
	if (offsetR == 93) {
		if(backing)
			++count1;
		if (backing && count1 == 100) {
			sprite->changeAnimation(MOVE_DOWN);
			backing = false;
			count1 = 0;
			count = 0;
			didStart = false;
		}
	}
	if (Game::instance().getKey(27)) { //ESC
		count = 0;
		didStart = false;
	}
	sprite->update(deltaTime);
	if (!didStart) {
		++count;
		if (count == 150 && !backing) {
			didStart = true;
			count = 0;
		}
	}
	if (!backing) {
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && Game::instance().getSpecialKey(GLUT_KEY_LEFT) && (posPlayer.y + 32) <= 27 * 16 - 2)
		{
			if (sprite->animation() != MOVE_DOWN)
				sprite->changeAnimation(MOVE_DOWN);
			posPlayer.y += speed;
			posPlayer.x -= speed;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), 0, false) || map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), 0, false))
			{
				posPlayer.y -= speed;
				posPlayer.x += speed;
				sprite->changeAnimation(STAND_DOWN);
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && (posPlayer.y + 32) <= 27 * 16 - 2)
		{
			if (sprite->animation() != MOVE_DOWN)
				sprite->changeAnimation(MOVE_DOWN);
			posPlayer.y += speed;
			posPlayer.x += speed;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), 0, false) || map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), 0, false))
			{
				posPlayer.y -= speed;
				posPlayer.x -= speed;
				sprite->changeAnimation(STAND_DOWN);
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) && Game::instance().getSpecialKey(GLUT_KEY_LEFT) && posPlayer.y >= 17)
		{
			if (sprite->animation() != MOVE_DOWN)
				sprite->changeAnimation(MOVE_DOWN);
			if ((offset == 3 && offsetR == 93) || (offset == 2 && offsetR == 62) || (offset == 1 && offsetR == 31) || (offset == 0 && offsetR == 0))
				didStart = true;
			posPlayer.y -= speed;
			posPlayer.x -= speed;
			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), 0, false) || map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), 0, false))
			{
				posPlayer.y += speed;
				posPlayer.x += speed;
				sprite->changeAnimation(STAND_DOWN);
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) && Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && posPlayer.y >= 17)
		{
			if (sprite->animation() != MOVE_DOWN)
				sprite->changeAnimation(MOVE_DOWN);
			if ((offset == 3 && offsetR == 93) || (offset == 2 && offsetR == 62) || (offset == 1 && offsetR == 31) || (offset == 0 && offsetR == 0))
				didStart = true;
			posPlayer.y -= speed;
			posPlayer.x += speed;
			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), 0, false) || map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), 0, false))
			{
				posPlayer.y += speed;
				posPlayer.x -= speed;
				sprite->changeAnimation(STAND_DOWN);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN) && Game::instance().getSpecialKey(GLUT_KEY_LEFT) &&
				!Game::instance().getSpecialKey(GLUT_KEY_UP) && Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				posPlayer.x -= speed;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), 0, false))
				{
					posPlayer.x += speed;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN) && Game::instance().getSpecialKey(GLUT_KEY_RIGHT) &&
				!Game::instance().getSpecialKey(GLUT_KEY_UP) && Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				posPlayer.x += speed;
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), 0, false))
				{
					posPlayer.x -= speed;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && posPlayer.y >= 17)
		{
			if (sprite->animation() != MOVE_UP)
				sprite->changeAnimation(MOVE_UP);
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT) &&
				Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				posPlayer.y -= speed;
				if ((offset == 3 && offsetR == 93) || (offset == 2 && offsetR == 62) || (offset == 1 && offsetR == 31) || (offset == 0 && offsetR == 0))
					didStart = true;
				if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), 0, false))
				{
					posPlayer.y += speed;
					sprite->changeAnimation(STAND_UP);
				}
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && (posPlayer.y + 32) <= 27 * 16 - 2)
		{
			if (sprite->animation() != MOVE_DOWN)
				sprite->changeAnimation(MOVE_DOWN);
			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT) &&
				Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				posPlayer.y += speed;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), 0, false))
				{
					posPlayer.y -= speed;
					sprite->changeAnimation(STAND_DOWN);
				}
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


void Player::render()
{
	sprite->render();
}

bool Player::getDidStart()
{
	return didStart;
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

int Player::getX()
{
	return posPlayer.x;
}


void Player::restartCount()
{
	didStart = false;
	count = 0;
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}