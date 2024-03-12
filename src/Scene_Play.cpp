#include "Scene_Play.h"
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
    // TODO: Register all gameplay Actions

    m_GridText.setCharacterSize(12);
    m_GridText.setFont(m_Game->assets().getFont("Mario"));

    loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    // TODO:This function takes in a grid (x, y) position and an Entity
    //      Return a Vec2 indicating where the center position of the Entity should be
    //      You must use the Entity's Animation size to position it correctly
    //      The size of the grid width and height is stored in m_GridSize.x and m_GridSize.y
    //      The bottom-left corner of the Animation should align with the bottom left of the grid cell

    return Vec2();
}

void Scene_Play::loadLevel(const std::string& filename)
{
    // reset the entity manager every time we load a level
    m_Entities = EntityManager();

    // TODO: read in the level file and add the appropriate entities
    //       use the playerConfig struct m_PlayerConfig to store player properties
    //       this struct is defined at the top of Scene_Play.h

    // NOTE: all of the code below is sample code which shows you how to
    //       set up and use entities with the new syntax, it should be removed

    spawnPlayer();

    // some sample entities
    auto brick = m_Entities.addEntity("tile");
    // IMPORTANT: always add the CAnimation component first so that gridToMapPixel can compute correctly
    brick->addComponent<CAnimation>(m_Game->assets().getAnimation("Brick"), true);
    brick->addComponent<CTransform>(Vec2(96, 480));
    brick->addComponent<CBoundingBox>(Vec2(64, 64));

    // NOTE: Your final code should position the entity with the grid x,y position read from the file:
    // brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick);

    /*if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
        std::cout << "This could be a good way od identifying if tile is a tile is a brick" << std::endl;
    }*/

    auto block = m_Entities.addEntity("tile");
    //block->addComponent<CAnimation>(m_Game->assets().getAnimation("Block"), true);
    block->addComponent<CTransform>(Vec2(224, 480));
    // add a bounding box, this will now show up if we press 'C' key
    //block->addComponent<CBoundingBox>(m_Game->assets().getAnimation("Block").getSize());

    auto question = m_Entities.addEntity("tile");
    //question->addComponent<CAnimation>(m_Game->assets().getAnimation("Question"), true);
    question->addComponent<CTransform>(Vec2(352, 480));

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
    //m_Player->addComponent<CAnimation>(m_Game->assets().getAnimation("Stand"), true);
    m_Player->addComponent<CTransform>(Vec2(224, 352));
    m_Player->addComponent<CBoundingBox>(Vec2(48, 48));
    m_Player->addComponent<CGravity>(0.1f);

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
    // TODO: Implement gravity's effect on the player
    // TODO: Implement the maximum player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right
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
    if (action.type() == "START")
    {
             if (action.name() == "TOGGLE_TEXTURE")     { m_DrawTextures = !m_DrawTextures; }
        else if (action.name() == "TOGGLE_COLLISION")   { m_DrawCollision = !m_DrawCollision; }
        else if (action.name() == "TOGGLE_GRID")        { m_DrawGrid = !m_DrawGrid; }
        else if (action.name() == "PAUSE")              { m_Paused = !m_Paused; }
        else if (action.name() == "QUIT")               { onEnd(); }
    }
    else if (action.type() == "END")
    {

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
    if (!m_Paused) { m_Game->window().clear(sf::Color(100, 100, 255)); }
    else { m_Game->window().clear(sf::Color(50, 50, 150)); }

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