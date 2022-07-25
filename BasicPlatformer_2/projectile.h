#pragma once

#include "GameObject.h"

const glm::vec2 PROJECTIlE_SIZE(20.0f,40.0f);

const glm::vec2 VELOCITY(0.0f, 150.0f);

class Projectile : public GameObject
{
public:
	Projectile(glm::vec3 color, glm::vec2 position, Texture2D texture)
		: GameObject(position, PROJECTIlE_SIZE, texture, color, VELOCITY){}
};