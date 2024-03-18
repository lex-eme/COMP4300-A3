#include "Scene_Play.h"

#include <fstream>

#include "GameEngine.h"
#include "Physics.h"

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

    m_GridText.setCharacterSize(5);
    m_GridText.setFont(m_Game->assets().getFont("Mario"));

    sf::View view = m_Game->window().getView();
    view.setSize(width() / 4.0f, height() / 4.0f);
    m_Game->window().setView(view);

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
    m_Entities = EntityManager();

    std::ifstream fin(filename);
    std::string overlap;

    while (fin >> overlap)
    {
        if (overlap == "Tile")
        {
            std::string name;
            float x, y;
            fin >> name >> x >> y;
            auto e = m_Entities.addEntity(name);
            
            e->addComponent<CAnimation>(m_Game->assets().getAnimation(name), true);
            e->addComponent<CTransform>(gridToMidPixel(x, y, e));
            e->addComponent<CBoundingBox>(m_Game->assets().getAnimation(name).getSize());
        }
        else if (overlap == "Dec")
        {
            std::string name;
            float x, y;
            fin >> name >> x >> y;
            auto e = m_Entities.addEntity(name);

            e->addComponent<CAnimation>(m_Game->assets().getAnimation(name), false);
            e->addComponent<CTransform>(gridToMidPixel(x, y, e));
        }
        else if (overlap == "Player")
        {
            fin >> m_PlayerConfig.X >> m_PlayerConfig.Y >> m_PlayerConfig.CX
                >> m_PlayerConfig.CY >> m_PlayerConfig.SPEED >> m_PlayerConfig.MAXSPEED
                >> m_PlayerConfig.JUMP >> m_PlayerConfig.GRAVITY >> m_PlayerConfig.WEAPON;
        }
    }

    spawnPlayer();
}

void Scene_Play::spawnPlayer()
{
    m_Player = m_Entities.addEntity("player");
    m_Player->addComponent<CAnimation>(m_Game->assets().getAnimation("Stand"), true);
    m_Player->addComponent<CTransform>(gridToMidPixel(m_PlayerConfig.X, m_PlayerConfig.Y, m_Player), Vec2(), Vec2(-1.0f, 1.0f), 0.0f);
    m_Player->addComponent<CBoundingBox>(Vec2(m_PlayerConfig.CX, m_PlayerConfig.CY));
    m_Player->addComponent<CGravity>(m_PlayerConfig.GRAVITY);
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
}

void Scene_Play::update()
{
    m_Entities.update();

    m_StateChanged = false;
    if (!m_Paused)
    {
        sMovement();
        sLifespan();
        sCollision();
        sAnimation();
    }
}

void Scene_Play::sMovement()
{
    auto& input = m_Player->getComponent<CInput>();
    auto& transform = m_Player->getComponent<CTransform>();
    if (input.up && input.canJump)
    {
        transform.velocity.y -= m_PlayerConfig.JUMP;
        input.canJump = false;
    }

    float xSpeed = 0.0f;
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

    if (xSpeed == 0.0f)
    {
        if (m_Player->getComponent<CState>().state != "Stand")
        {
            m_Player->getComponent<CState>().state = "Stand";
            m_StateChanged = true;
        }
    }
    else
    {
        if (m_Player->getComponent<CState>().state != "Run")
        {
            m_Player->getComponent<CState>().state = "Run";
            m_StateChanged = true;
        }
    }

    auto& gravity = m_Player->getComponent<CGravity>();
    transform.velocity.y += gravity.gravity;

    if (transform.velocity.y >= m_PlayerConfig.MAXSPEED)
        transform.velocity.y = m_PlayerConfig.MAXSPEED;

    transform.prevPos = transform.pos;
    transform.pos += transform.velocity;
}

void Scene_Play::sLifespan()
{
    // TODO: Check Lifespan of entities that have them, and destroy them id they go over
}

void Scene_Play::sCollision()
{
    // TODO: Implement bullet / tile collisions
    //       Destroy the tile if it has a Brick animation
    auto& pPos = m_Player->getComponent<CTransform>().pos;
    bool hasCollision = false;

    for (auto e : m_Entities.getEntities())
    {
        if (!e->hasComponent<CBoundingBox>() || !e->hasComponent<CTransform>() || e == m_Player)
            continue;

        Vec2 overlap = Physics::GetOverlap(e, m_Player);
        Vec2 prevOverlap = Physics::GetPreviousOverlap(e, m_Player);

        if (!(overlap.x < 0.0f || overlap.y < 0.0f))
        {
            hasCollision = true;
            auto& qPos = e->getComponent<CTransform>().pos;
            auto& pPrevPos = m_Player->getComponent<CTransform>().prevPos;
            if (prevOverlap.y > 0.0f)
            {
                pPos.x += pPos.x < qPos.x ? -overlap.x : overlap.x;
            }

            if (prevOverlap.x > 0.0f)
            {
                if (pPrevPos.y > qPos.y)
                {
                    pPos.y += overlap.y;
                    const std::string& name = e->getComponent<CAnimation>().animation.getName();
                    if (name == "Question")
                    {
                        e->addComponent<CAnimation>(m_Game->assets().getAnimation("Question2"), true);
                    }
                    else if (name == "Brick")
                    {
                        e->destroy();
                    }
                }
                else
                {
                    pPos.y -= overlap.y;
                    m_Player->getComponent<CInput>().canJump = true;
                }
                m_Player->getComponent<CTransform>().velocity.y = 0.0f;
            }
        }
    }

    if (!hasCollision)
    {
        if (m_Player->getComponent<CState>().state != "Air")
        {
            m_Player->getComponent<CState>().state = "Air";
            m_StateChanged = true;
        }
    }
    
    if (pPos.y > height())
    {
        m_Player->destroy();
        spawnPlayer();
    }
    
    if (pPos.x < 0.0f)
    {
        pPos.x = 0.0f;
    }
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
        else if (action.name() == "DOWN")               { input.down = true; }
        else if (action.name() == "RIGHT")              { input.right = true; }
        else if (action.name() == "LEFT")               { input.left = true; }
    }
    else if (action.type() == "END")
    {
             if (action.name() == "UP")         { input.up = false; }
        else if (action.name() == "DOWN")       { input.down = false; }
        else if (action.name() == "RIGHT")      { input.right = false; }
        else if (action.name() == "LEFT")       { input.left = false; }
    }
}

void Scene_Play::sAnimation()
{
    if (m_StateChanged)
    {
        m_Player->addComponent<CAnimation>(m_Game->assets().getAnimation(m_Player->getComponent<CState>().state), true);
    }
    
    for (auto e : m_Entities.getEntities())
    {
        auto& anim = e->getComponent<CAnimation>();
        anim.animation.update();

        if (!anim.repeat && anim.animation.hasEnded())
        {
            e->destroy();
        }
    }
}

void Scene_Play::onEnd()
{
    m_Game->changeScene("MENU", nullptr, true);
}

void Scene_Play::sRender()
{
    if (!m_Paused) { m_Game->window().clear(sf::Color(148, 148, 255)); }
    else { m_Game->window().clear(sf::Color(58, 58, 100)); }

    auto& pPos = m_Player->getComponent<CTransform>().pos;
    sf::View view = m_Game->window().getView();
    float windowCenterX = std::max(view.getSize().x / 2.0f, pPos.x);
    view.setCenter(windowCenterX, m_Game->window().getSize().y - view.getSize().y / 2.0f);
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
        float vWidth = view.getSize().x;
        float vHeight = view.getSize().y;
        float leftX = view.getCenter().x - vWidth / 2.0f;
        float rightX = leftX + vWidth + m_GridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_GridSize.x);

        for (float x = nextGridX; x < rightX; x += m_GridSize.x)
        {
            drawLine(Vec2(x, height() - vHeight), Vec2(x, height()));
        }

        for (float y = 0; y < vHeight; y += m_GridSize.y)
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_GridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_GridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_GridSize.y);
                m_GridText.setString("(" + xCell + "," + yCell + ")");
                m_GridText.setPosition(x + 3.0f, height() - y - m_GridSize.y + 2.0f);
                //m_Game->window().draw(m_GridText);
            }
        }
    }
}