#include "game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include <vector>
#include <iostream>
// Game-related State data
SpriteRenderer* Renderer;
GameObject* Player;
GameLevel* one;
//std::vector<GameObject*> EnemiesRow;
//std::vector<GameObject*> EnemiesRow1;
//std::vector<GameObject*> EnemiesRow2;
//std::vector<std::vector<GameObject*>> Enemies;
//GameObject* Enemy;
//GameObject* Enemy1;

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 100.0f);

const glm::vec2 ENEMY_SIZE(50.0f, 50.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

bool GoingLeft = false;
float Canfire = 1.0f;
float firecounter = 0.0f;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
}

void Game::Init()
{
	// load shaders
	ResourceManager::LoadShader("shader.vert", "shader.frag", nullptr, "sprite");
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
		static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// set render-specific controls
	Shader myShader;
	myShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(myShader);
	// load textures
	ResourceManager::LoadTexture("Textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("Textures/block_solid.png", false, "block_solid");
	one = new GameLevel;
	one->Load("Levels/one.lvl", this->Width, this->Height/2);
	//this->Levels.push_back(one);
	this->Level = 0;
	glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
	
	glm::vec2 EnemyPos = glm::vec2(this->Width / 2.0f - ENEMY_SIZE.x / 2.0f, 50.f);
	//Enemy = new GameObject(EnemyPos, ENEMY_SIZE, ResourceManager::GetTexture("face"));
	
	//for (int i = 0; i < 10; i++)
	//{
	//	Enemy = new GameObject(EnemyPos, ENEMY_SIZE, ResourceManager::GetTexture("face"));
	//	EnemiesRow.push_back(Enemy);
	//	Enemy = new GameObject(EnemyPos, ENEMY_SIZE, ResourceManager::GetTexture("face"));
	//	EnemiesRow1.push_back(Enemy);
	//	Enemy = new GameObject(EnemyPos, ENEMY_SIZE, ResourceManager::GetTexture("face"));
	//	EnemiesRow2.push_back(Enemy);
	//}
	//for (int i = 0; i < 3; i++)
	//{
	//	Enemies.push_back({EnemiesRow});
	//	Enemies.push_back({EnemiesRow1 });
	//	Enemies.push_back({ EnemiesRow2 });
	//}
	//for (int i = 0; i < Enemies.size(); i++)
	//{
	//	for (int j = 0; j < Enemies[i].size(); j++)
	//	{
	//		Enemies[i][j]->Position.x = Width - (100 + (50 * j));
	//		Enemies[i][j]->Position.y = Height - (200 + (50 * i));
	//	}
	//}
	//Enemies[0][0]->Position.x = 50.0f;
	//Enemies[1][1]->Position.x = 100.0f;
	//Enemies[2][2]->Position.x = 120.0f;
	//std::cout << Enemies.size();
	//for (int i = 0; i < Enemies.size(); i++)
	//{
	//
	//	Enemies[i]->Position += (i * 20.0f);
	//}

}

void Game::Update(float dt)
{
	this->UpdateProjectiles(dt);
	one->move(dt);
	Docollision();
	
	
	
}

void Game::ProcessInput(float dt)
{
	if (this->State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0.0f)
			{
				Player->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - PLAYER_SIZE.x)
			{
				Player->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE] && firecounter <= 0.0f)
		{
			spawnProjectiles();
			firecounter = Canfire;
		}
		
		firecounter -= dt;
	}
}

void Game::Render()
{

	//Texture2D myTexture;
	//myTexture = ResourceManager::GetTexture("face");
	//Renderer->DrawSprite(myTexture, glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
	one->Draw(*Renderer);
	Player->Draw(*Renderer);
	//Enemy->Draw(*Renderer);
	//Enemies[0]->Draw(*Renderer);
	//Enemies[1]->Draw(*Renderer);
	//for (auto it : Enemies)
	//{
	//	it[0]->Draw(*Renderer);
	//}
	//for (int i = 0; i < Enemies.size(); i++)
	//{
	//	for (int j = 0; j < Enemies[0].size(); j++)
	//	{
	//		Enemies[i][j]->Draw(*Renderer);
	//	}
	//}
	for (auto it : projectiles)
	{
		if(!it.Destroyed)
			it.Draw(*Renderer);
	}

	

}

void Game::spawnProjectiles()
{
	this->projectiles.push_back(Projectile(glm::vec3(0.5f, 0.5f, 1.0f), Player->Position, ResourceManager::GetTexture("face")));
}                                 

void Game::UpdateProjectiles(float dt)
{
	for (Projectile& projectile : this->projectiles)
	{
		projectile.Position -= projectile.Velocity * dt;
	}
}

bool CheckCollision(Projectile& one, GameObject& two);

void Game::Docollision()
{
	
	//this->projectiles.push_back(Projectile(glm::vec3(0.5f, 0.5f, 1.0f), Player->Position, ResourceManager::GetTexture("face")));
	for (GameObject& Enemy : one->Enemies)
	{
		for (Projectile& projectile : projectiles)
		{
			if (!projectile.Destroyed && !Enemy.Destroyed)
			{
				if (CheckCollision(projectile, Enemy))
				{
					std::cout << "colliding" << std::endl;
					projectile.Destroyed = true;
					Enemy.Destroyed = true;

				}
				else
				{
					//std::cout << "not colliding" << std::endl;
				}
			}
			
		}
		
	}
}

bool CheckCollision(Projectile& one, GameObject& two)
{

	if (one.Position.x > (two.Position.x + two.Size.x))
	{
	
		return false;
	}
	if ((one.Position.x + one.Size.x) < two.Position.x)
	{
	
		return false;
	}
	if (one.Position.y > (two.Position.y + two.Size.y))
	{
		return false;
	}
	if ((one.Position.y +	one.Size.y) < two.Position.y)
	{
		return false;
	}
	return true;
}
