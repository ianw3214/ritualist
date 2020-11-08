#include "game.hpp"

#include "oasis.h"
#include "graphics/opengl/texture.hpp"

#include "entities/entity.hpp"
#include "entities/player.hpp"
#include "entities/enemies/boss1.hpp"
#include "entities/altar.hpp"

#include "camera/camera.hpp"

////////////////////////////////////////////////////////////////
Ref<GameLayer> GameService::s_game = nullptr;
void GameService::SetPlayer(Ref<Player> player)
{
    s_game->m_player = player;
}
Ref<Player> GameService::GetPlayer()
{
    return s_game->m_player;
}

Ref<Entity> GameService::GetBoss()
{
    return s_game->m_boss;
}

bool GameService::ActivateAltar()
{
    return s_game->m_boss->GetHealth() <= 0.f;
}

void GameService::AddAttackCallback(attack_func f)
{
    s_game->m_attackManager.AddCallback(f);
}

void GameService::AddAttack(Target target, float damage, float x, float y, float w, float h)
{
    s_game->m_attackManager.AddAttack(target, damage, x, y, w, h);
}

////////////////////////////////////////////////////////////////
void GameLayer::Init()
{
    // Make sure we set the game service to ourselves
    GameService::s_game = this;

    Player * player = new Player();
    m_entities.push_back(player);
    Boss1 * boss = new Boss1();
    m_entities.push_back(boss);
    Altar * altar = new Altar();
    m_entities.push_back(altar);
    m_boss = boss;

    m_map = new Oasis::Sprite("res/maps/background1.png");
}

void GameLayer::Close() 
{
    for (Entity * entity : m_entities)
    {
        delete entity;
    }
}

bool GameLayer::HandleEvent(const Oasis::Event& event)
{
    return false;
}

void GameLayer::Update()
{
    // Microseconds to seconds
    float delta = Oasis::WindowService::GetDeltaF() / 1000000.f;

    // Render the map first
    m_map->SetPos(CameraService::RawToScreenX(0.f), CameraService::RawToScreenY(0.f));
    Oasis::Renderer::DrawSprite(m_map);
    
    // TODO: Sort entity by y to render in correct order
    // Update all entity components
    for (Ref<Entity> entity : m_entities)
    {
        entity->Update(delta);
    }

    m_attackManager.Update(delta);
}