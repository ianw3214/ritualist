#include "game.hpp"

#include "oasis.h"
#include "graphics/opengl/texture.hpp"

#include "entities/entity.hpp"
#include "entities/player.hpp"
#include "entities/enemies/boss1.hpp"
#include "entities/altar.hpp"

#include "levels.hpp"
#include "progression.hpp"
#include "game/camera/camera.hpp"

#include "death/death.hpp"
#include "credits/credits.hpp"

////////////////////////////////////////////////////////////////
Ref<GameLayer> GameService::s_game = nullptr;
Level GameService::s_level = Level::LOBBY;
bool GameService::s_gameInit = false;
bool GameService::s_sacrificeButton = false;
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

bool GameService::CheckMapConstriants(float x, float y)
{
    return s_game->CheckMapConstraint(x, y);
}

void GameService::AddAttackCallback(attack_func f)
{
    s_game->m_attackManager.AddCallback(f);
}

void GameService::AddAttack(Target target, float damage, float x, float y, float w, float h)
{
    s_game->m_attackManager.AddAttack(target, damage, x, y, w, h);
}

void GameService::ChangeLevel(Level level)
{
    s_game->m_changeLevel = true;
    s_level = level;
}

Level GameService::GetLevel()
{
    return s_level;
}

bool GameService::PlayerDead()
{
    return s_game->m_player->IsDead();
}

void GameService::ResetButtons()
{
    s_sacrificeButton = false;
}

bool GameService::SacrificeButton()
{
    return s_sacrificeButton;
}

void GameService::GlobalInit()
{
    if (s_gameInit) return;
    UIManager::AddUIEventCallback(kLobbyButton, [&](){
        s_sacrificeButton = true;
        return;
    });
    s_gameInit = true;

    Oasis::TextRenderer::LoadFont("default", "res/fonts/Munro.ttf");
    Oasis::TextRenderer::LoadFont("large", "res/fonts/Munro.ttf", 48);

    Progression::ResetProgression();

    // Play out one and only background track
    Ref<Oasis::AudioResource> audio = Oasis::ResourceManager::LoadResource<Oasis::AudioResource>("res/audio/track.wav");
    Oasis::AudioSource * source = new Oasis::AudioSource();
    source->Play(audio, true);
}

////////////////////////////////////////////////////////////////
void GameLayer::Init()
{
    // The game service will ensure this is only called once
    GameService::GlobalInit();

    // Make sure we set the game service to ourselves
    GameService::s_game = this;
    m_playerShadow = new Oasis::Sprite("res/player_shadow.png");
    
    UIManager::HideWindow(kAltarUiName);
    LevelLoader::LoadLevel(GameService::GetLevel(), this);

    m_changeLevel = false;
    m_transitionChangeLevel = false;
    m_transitionTimer = 0.f;
    m_fadeInTimer = m_fadeInTime;
}

void GameLayer::Close() 
{
    delete m_map;
    delete m_playerShadow;

    for (Entity * entity : m_entities)
    {
        delete entity;
    }
}

bool GameLayer::HandleEvent(const Oasis::Event& event)
{
    return false;
}

Oasis::IState * GameLayer::Update()
{
    // Microseconds to seconds
    float delta = Oasis::WindowService::GetDeltaF() / 1000000.f;

    // Render the map first
    Oasis::Renderer::Clear(Oasis::Colour{15.f / 255.f, 13.f / 255.f, 48.f / 255.f});
    m_map->RenderMap();
    // Update shadow positions and render those first
    if (m_player && !m_player->IsTelporting())
    {
        m_playerShadow->SetPos(CameraService::RawToScreenX(m_player->GetX() - m_player->GetWidth() / 2.f), CameraService::RawToScreenY(m_player->GetY() - 8.f));
        Oasis::Renderer::DrawSprite(m_playerShadow);   
    }

    std::sort(m_entities.begin(), m_entities.end(), [](Ref<Entity> a, Ref<Entity> b) {
        // Return true = a goes first = render first = y is higher
        // Return true = a goes first = render first = layer is lower
        return a->GetLayer() == b->GetLayer() ? a->GetY() > b->GetY() : a->GetLayer() < b->GetLayer();
    });
    // Update all entity components
    for (Ref<Entity> entity : m_entities)
    {
        entity->Update(delta);
    }
    m_attackManager.Update(delta);

    if (m_fadeInTimer >= 0.f)
    {
        m_fadeInTimer -= delta;
        float alpha = 1.f - (m_fadeInTime - m_fadeInTimer) / m_fadeInTime;
        Oasis::Renderer::DrawQuad(0.f, 0.f, (float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight(), Oasis::Colours::BLACK, alpha);
    }

    // Handle transitions
    if (m_changeLevel && !m_transitionChangeLevel)
    {
        m_transitionChangeLevel = true;
        m_transitionTimer = m_transitionTime;
    }
    else if (m_changeLevel)
    {
        m_transitionTimer -= delta;
        float alpha = (m_transitionTime - m_transitionTimer) / m_transitionTime;
        Oasis::Renderer::DrawQuad(0.f, 0.f, (float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight(), Oasis::Colours::BLACK, alpha);
    }

    if (GameService::PlayerDead() && !m_transitionChangeLevel)
    {
        m_transitionChangeLevel = true;
        m_transitionTimer = m_transitionTime;
    }
    else if (GameService::PlayerDead())
    {
        m_transitionTimer -= delta;
        float alpha = (m_transitionTime - m_transitionTimer) / m_transitionTime;
        Oasis::Renderer::DrawQuad(0.f, 0.f, (float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight(), Oasis::Colours::BLACK, alpha);
    }

    if (Progression::GameWon() && !m_transitionChangeLevel)
    {
        m_transitionChangeLevel = true;
        m_transitionTimer = m_winTransitionTime;
    }
    else if (Progression::GameWon())
    {
        m_transitionTimer -= delta;
        float alpha = (m_winTransitionTime - m_transitionTimer) / m_winTransitionTime;
        alpha *= alpha;
        Oasis::Renderer::DrawQuad(0.f, 0.f, (float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight(), Oasis::Colours::WHITE, alpha);
    }

    GameService::ResetButtons();

    // State changes at the very end
    if (m_transitionChangeLevel && m_transitionTimer <= 0.f)
    {
        if (m_changeLevel)
        {
            return new Game();
        }
        if (GameService::PlayerDead())
        {
            return new Death();
        }
        if (Progression::GameWon())
        {
            return new Credits();
        }
    }

    return nullptr;
}

bool GameLayer::CheckMapConstraint(float x, float y)
{
    return !m_map->CheckCollision(x, y);
}