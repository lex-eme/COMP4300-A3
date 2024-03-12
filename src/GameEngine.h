#pragma once

#include <unordered_map>
#include <string>

#include "Scene.h"
#include "Assets.h"

typedef std::unordered_map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
	sf::RenderWindow	m_Window;
	Assets				m_Assets;
	std::string			m_CurrentScene;
	SceneMap			m_SceneMap;
	size_t				m_SimulationSpeed = 1;
	bool				m_Running = true;

public:
	GameEngine(const std::string& path);

	void changeScene(std::string sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

	void quit();
	void run();

	sf::RenderWindow& window();
	const Assets& assets();
	bool isRunning() const;

private:
	void init(const std::string& path);
	void update();
	void sUserInput();
	std::shared_ptr<Scene> currentScene();

};

