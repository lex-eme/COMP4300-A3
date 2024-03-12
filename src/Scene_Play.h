#pragma once

#include <string>
#include <memory>

#include "Entity.h"
#include "Scene.h"


class Scene_Play : public Scene
{
	struct PlayerConfig
	{
		float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
		std::string WEAPON;
	};

	std::shared_ptr<Entity>		m_Player;
	std::string					m_LevelPath;
	PlayerConfig				m_PlayerConfig;
	bool						m_DrawTextures = true;
	bool						m_DrawCollision = false;
	bool						m_DrawGrid = false;
	const Vec2					m_GridSize = { 64.0f, 64.0f };
	sf::Text					m_GridText;

public:
	Scene_Play(GameEngine* gameEngine, const std::string& levelPath);
	void update() override;

private:
	void init(const std::string& levelPath);
	Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
	void loadLevel(const std::string& filename);
	void spawnPlayer();
	void spawnBullet(std::shared_ptr<Entity> entity);
	void onEnd();

	// Systems
	void sAnimation();
	void sMovement();
	void sLifespan();
	void sCollision();
	void sRender() override;
	void sDoAction(const Action& action) override;
};

