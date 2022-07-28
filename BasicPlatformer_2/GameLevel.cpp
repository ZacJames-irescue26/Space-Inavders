#include "GameLevel.h"
#include <iostream>
#include <fstream>
#include <sstream>


const glm::vec2 ENEMY_SIZE(50.0f, 50.0f);

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
	// clear old data
	this->Enemies.clear();
	// load from file
	unsigned int tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line)) // read each line from level file
		{
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode) // read each word separated by spaces
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : this->Enemies)
		if (!tile.Destroyed)
			tile.Draw(renderer);
}

bool GameLevel::IsComplete()
{
	for (GameObject& tile : this->Enemies)
		if (!tile.IsSolid && !tile.Destroyed)
			return false;
	return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	// calculate dimensions
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 

	// initialize level tiles based on tileData		
	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			// check block type from level data (2D level array)
			if (tileData[y][x] == 1) // solid
			{
				glm::vec2 pos(ENEMY_SIZE.x * x, ENEMY_SIZE.y * y);
				glm::vec2 size(ENEMY_SIZE.x, ENEMY_SIZE.y);
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = true;
				this->Enemies.push_back(obj);
			}
			else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(ENEMY_SIZE.x * x, ENEMY_SIZE.y * y);
				glm::vec2 size(ENEMY_SIZE.x, ENEMY_SIZE.y);
				this->Enemies.push_back(GameObject(pos, size, ResourceManager::GetTexture("face"), color));
			}
			
		}
	}
}

void GameLevel::move(float dt)
{
	for (int i = 0; i < Enemies.size(); i++)
	{
			if (Enemies[i].Position.x > (800 - ENEMY_SIZE.x))
			{
				for (int i = 0; i < Enemies.size(); i++)
				{
						Enemies[i].Position.y += 200.0f * dt;
				}
				
				GoingLeft = true;
			}
			if (Enemies[i].Position.x < 0)
			{
				for (int i = 0; i < Enemies.size(); i++)
				{
						Enemies[i].Position.y += 200.0f * dt;
				}
				GoingLeft = false;
			}
			if (GoingLeft == false)
			{
				Enemies[i].Position.x += 10.0f * dt;
			}
			else if(GoingLeft == true)
			{
				Enemies[i].Position.x -= 10.0f * dt;
			}
	}
}

