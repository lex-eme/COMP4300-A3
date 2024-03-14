#pragma once

#include "Entity.h"

class Physics
{
public:
	static Vec2 GetOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);
	static Vec2 GetPreviousOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);
};

