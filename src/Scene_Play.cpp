#include "Scene_Play.h"

#include <fstream>

#include "GameEngine.h"

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine), m_LevelPath(levelPath)
{
    init(m_LevelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    registerAction(sf::Keyboard::Z, "UP");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Q, "LEFT");
    // TODO: Register all gameplay Actions

    m_GridText.setCharacterSize(12);
    m_GridText.setFont(m_Game->assets().getFont("Mario"));

    loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity) const
{
    const Vec2& size = entity->getComponent<CAnimation>().animation.getSize();
    float x = (gridX * m_GridSize.x) + (size.x / 2.0f);
    float y = height() - (gridY * m_GridSize.y) - (size.y / 2.0f);
    return Vec2(x, y);
}

void Scene_Play::loadLevel(const std::string& filename)
{
    // reset the entity manager every time we load a level
    m_Entities = EntityManager();

    std::ifstream fin(filename);
    std::string temp;

    while (fin >> temp)
    {
        if (temp == "Tile")
        {
            std::string name;
            float x, y;
            fin >> name >> x >> y;
            auto e = m_Entities.addEntity(name);
            
            e->addComponent<CAnimation>(m_Game->assets().getAnimation(name), false);
            e->addComponent<CTransform>(gridToMidPixel(x, y, e));
            e->addComponent<CBoundingBox>(m_Game->assets().getAnimation(name).getSize());
        }
        else if (temp == "Dec")
        {
            std::string name;
            float x, y;
            fin >> name >> x >> y;
            auto e = m_Entities.addEntity(name);

            e->addComponent<CAnimation>(m_Game->assets().getAnimation(name), false);
            e->addComponent<CTransform>(gridToMidPixel(x, y, e));
        }
        else if (temp == "Player")
        {
            fin >> m_PlayerConfig.X >> m_PlayerConfig.Y >> m_PlayerConfig.CX
                >> m_PlayerConfig.CY >> m_PlayerConfig.SPEED >> m_PlayerConfig.MAXSPEED
                >> m_PlayerConfig.JUMP >> m_PlayerConfig.GRAVITY >> m_PlayerConfig.WEAPON;
        }
    }

    spawnPlayer();

    /*if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
        std::cout << "This could be a good way od identifying if tile is a tile is a brick" << std::endl;
    }*/

    // NOTE: this is incredibly important please read this example
    //      Components are now returned as references rather than pointers
    //      If you do not specify a reference variable type, it will COPY the component
    //      Here is an example:
    //
    //      This will copy the transform into the variable 't1' - it is incorrect
    //      auto t1 = entity->get<CTransform>();
    //
    //      This will reference the transform with the variable 't2' - it is correct
    //      Now any changes you make to t2 will be changed inside the enetity
    //      auto& t2 = entity->get<CTransform>();
}

void Scene_Play::spawnPlayer()
{
    m_Player = m_Entities.addEntity("player");
    m_Player->addComponent<CAnimation>(m_Game->assets().getAnimation("Stand"), true);
    m_Player->addComponent<CTransform>(gridToMidPixel(m_PlayerConfig.X, m_PlayerConfig.Y, m_Player), Vec2(), Vec2(-1.0f, 1.0f), 0.0f);
    m_Player->addComponent<CBoundingBox>(Vec2(m_PlayerConfig.CX, m_PlayerConfig.CY));
    m_Player->addComponent<CGravity>(m_PlayerConfig.GRAVITY);

    // TODO: be suze to add the remaining components to the player
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
}

void Scene_Play::update()
{
    m_Entities.update();

    // TODO: implement pause functionality

    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();
}

void Scene_Play::sMovement()
{
    // TODO: Implement player movement / jumping based on its CInput component
    auto& input = m_Player->getComponent<CInput>();
    auto& transform = m_Player->getComponent<CTransform>();
    if (input.up && input.canJump)
    {
        transform.velocity.y -= m_PlayerConfig.JUMP;
        input.canJump = false;
    }

    float xSpeed = 0.0;
    if (input.right)
    {
        xSpeed += m_PlayerConfig.SPEED;
        transform.scale.x = -1.0f;
    }

    if (input.left)
    {
        xSpeed -= m_PlayerConfig.SPEED;
        transform.scale.x = 1.0f;
    }

    transform.velocity.x = xSpeed;
    // TODO: Implement gravity's effect on the player
    auto& gravity = m_Player->getComponent<CGravity>();
    transform.velocity.y += gravity.gravity;

    // TODO: Implement the maximum player speed in both X and Y directions
    if (transform.velocity.y <= m_PlayerConfig.MAXSPEED)
        transform.velocity.y = m_PlayerConfig.MAXSPEED;

    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right

    transform.prevPos = transform.pos;
    transform.pos += transform.velocity;
}

void Scene_Play::sLifespan()
{
    // TODO: Check Lifespan of entities that have them, and destroy them id they go over
}

void Scene_Play::sCollision()
{
    // REMEMBER: SFML's (0,0) position is on the TOP-LEFT corner
    //           This means jumping will have a negative y-component
    //           and gravity will have a positive y-component
    //           Also, something BELOW something else will have a y value GREATER than it
    //           Also, something ABOVE something else will have a y value LESS than it

    // TODO: Implement Physics::GetOverlap() function, use it inside this function

    // TODO: Implement bullet / tile collisions
    //       Destroy the tile if it has a Brick animation
    // TODO: Implement player / tile collisions and resolutions
    //       Update the CState component of the player to store whether
    //       it is currently on the ground or in the air. This will be
    //       used by the Animation system
    // TODO: Check to see if the player has fallen down a hole (y > height())
    // TODO: Don't let the player walk off the left side of the map
}

void Scene_Play::sDoAction(const Action& action)
{
    auto& input = m_Player->getComponent<CInput>();

    if (action.type() == "START")
    {
             if (action.name() == "TOGGLE_TEXTURE")     { m_DrawTextures = !m_DrawTextures; }
        else if (action.name() == "TOGGLE_COLLISION")   { m_DrawCollision = !m_DrawCollision; }
        else if (action.name() == "TOGGLE_GRID")        { m_DrawGrid = !m_DrawGrid; }
        else if (action.name() == "PAUSE")              { m_Paused = !m_Paused; }
        else if (action.name() == "QUIT")               { onEnd(); }
        else if (action.name() == "UP")                 { input.up = true; }
        else if (action.name() == "RIGHT")              { input.right = true; }
        else if (action.name() == "LEFT")               { input.left = true; }
    }
    else if (action.type() == "END")
    {
             if (action.name() == "UP")         { input.up = false; }
        else if (action.name() == "RIGHT")      { input.right = false; }
        else if (action.name() == "LEFT")       { input.left = false; }
    }
}

void Scene_Play::sAnimation()
{
    // TODO: Complete the Animation class code first

    // TODO: set the animation of the player based on its CState component
    // TODO: for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
    //       if the animation is not repeated, and it has ended, destroy the entity
}

void Scene_Play::onEnd()
{
    // TODO: when the scene ends, change back to the menu scene
    //       use m_Game->changeScene(correct params);
    m_Game->changeScene("MENU", nullptr, true);
}

void Scene_Play::sRender()
{
    // color the background darker so you know that the game is paused
    if (!m_Paused) { m_Game->window().clear(sf::Color(148, 148, 255)); }
    else { m_Game->window().clear(sf::Color(58, 58, 100)); }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto& pPos = m_Player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(m_Game->window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_Game->window().getView();
    view.setCenter(windowCenterX, m_Game->window().getSize().y - view.getCenter().y);
    m_Game->window().setView(view);

    if (m_DrawTextures)
    {
        for (auto e : m_Entities.getEntities())
        {
            auto& transform = e->getComponent<CTransform>();

            if (e->hasComponent<CAnimation>())
            {
                auto& animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_Game->window().draw(animation.getSprite());
            }
        }
    }

    if (m_DrawCollision)
    {
        for (auto e : m_Entities.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color::Transparent);
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1.0f);
                m_Game->window().draw(rect);
            }
        }
    }

    if (m_DrawGrid)
    {
        float leftX = m_Game->window().getView().getCenter().x - width() / 2;
        float rightX = leftX + width() + m_GridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_GridSize.x);

        for (float x = nextGridX; x < rightX; x += m_GridSize.x)
        {
            drawLine(Vec2(x, 0.0f), Vec2(x, height()));
        }

        for (float y = 0; y < height(); y += m_GridSize.y)
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_GridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_GridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_GridSize.y);
                m_GridText.setString("(" + xCell + "," + yCell + ")");
                m_GridText.setPosition(x + 3.0f, height() - y - m_GridSize.y + 2.0f);
                m_Game->window().draw(m_GridText);
            }
        }
    }
}