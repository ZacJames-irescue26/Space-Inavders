#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "projectile.h"
#include <vector>
#include "GameLevel.h"
enum GameState
{
 GAME_ACTIVE,
 GAME_MENU,
 GAME_WIN
};


class Game {
public:
	GameState State;
	bool Keys[1024];
	unsigned int Width, Height;

	Game(unsigned int width, unsigned int height);
	~Game();
	std::vector<GameLevel> Levels;
	unsigned int Level;
	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	std::vector<Projectile> projectiles;
	void spawnProjectiles();
	void UpdateProjectiles(float dt);
	void Docollision();
};

