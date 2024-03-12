#include "GameEngine.h"

#include <iostream>
#include "Scene_Menu.h"

GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::changeScene(std::string sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (scene)
	{
		m_SceneMap[sceneName] = scene;
	}
	else
	{
		if (m_SceneMap.find(sceneName) == m_SceneMap.end())
		{
			std::cerr << "Warning: Scene does not exist: " << sceneName << std::endl;
			return;
		}
	}

	if (endCurrentScene)
	{
		m_SceneMap.erase(m_SceneMap.find(m_CurrentScene));
	}

	m_CurrentScene = sceneName;
}

void GameEngine::quit()
{
	m_Running = false;
}

void GameEngine::run()
{
	while (isRunning())
	{
		update();
	}
}

sf::RenderWindow& GameEngine::window()
{
	return m_Window;
}

const Assets& GameEngine::assets()
{
	return m_Assets;
}

bool GameEngine::isRunning() const
{
	return m_Running && m_Window.isOpen();
}

void GameEngine::init(const std::string& path)
{
	m_Assets.loadFromFile(path);

	m_Window.create(sf::VideoMode(1280, 800), "Not Mario");
	m_Window.setVerticalSyncEnabled(true);

	changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

void GameEngine::update()
{
	if (!isRunning() || m_SceneMap.empty())
		return;

	sUserInput();
	currentScene()->simulate(m_SimulationSpeed);
	currentScene()->sRender();
	m_Window.display();
}

void GameEngine::sUserInput()
{
	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			quit();
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				std::cout << "trying to save screenshot to " << "test.png" << std::endl;
				sf::Texture texture;
				texture.create(m_Window.getSize().x, m_Window.getSize().y);
				texture.update(m_Window);
				if (texture.copyToImage().saveToFile("test.png"))
				{
					std::cout << "screenshot saved to " << "test.png" << std::endl;
				}
			}
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
			{
				continue;
			}

			const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
			currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
		}
	}
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_SceneMap[m_CurrentScene];
}
