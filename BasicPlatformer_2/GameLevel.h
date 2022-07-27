#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
class GameLevel
{
public:

	std::vector<GameObject> Enemies;
	GameLevel() {};
	void Load(const char* file, unsigned int levelwidth, unsigned int levelHeight);
	void Draw(SpriteRenderer& renderer);
	void init(std::vector<std::vector<unsigned int>> enemydata, unsigned int levelwidth, unsigned int levelheight);
	bool IsComplete();
	bool GoingLeft = false;
	void move(float dt);
};