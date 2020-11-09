#include "altar.hpp"

#include "game/game.hpp"
#include "game/progression.hpp"
#include "player.hpp"

Altar::Altar()
    : Entity(500.f, 500.f, 160.f, 480.f, 0.f, 0.f)
    , m_sprite("res/altar.png", 160.f, 480.f)
    , m_activated(false)
{
    m_sprite.SetDimensions(160.f, 480.f);
    m_sprite.AddAnimation("default", 0, 6);
    m_sprite.AddAnimation("activated", 7, 8);
    m_sprite.PlayAnimation("default");
    m_sprite.SetFPS(12);
}

void Altar::Update(float delta)
{
    // The interact key is the same as the attack key
    if (m_activated && InputService::Attack() && m_activateTimer <= 0.f)
    {
        // Check player proximity with altar
        Ref<Player> player = GameService::GetPlayer();
        const float xDist = (GetX() + GetWidth() / 2) - player->GetX();
        const float yDist = (GetY() + GetWidth() / 2) - player->GetY();
        if (xDist * xDist + yDist * yDist <= m_activateRadius * m_activateRadius)
        {
            UIManager::ShowWindow(kAltarUiName);
        }
    }

    if (m_activated && m_activateTimer <= 0.f)
    {
        if (GameService::SacrificeButton())
        {
            UIManager::HideWindow(kAltarUiName);
            GameService::ChangeLevel(Level::LOBBY);
            // Save the player health here in progression
            Ref<Player> player = GameService::GetPlayer();
            Progression::SetPlayerHealth(player->GetHealth());
            // Close the altar since we already beat the level
            Progression::SetLevelFinished(Level::BOSS1);
        }
    }

    m_activateTimer -= delta;

    // Want to check whether to activate altar or not
    if (GameService::ActivateAltar() && !m_activated)
    {
        m_sprite.PlayAnimation("activated");
        m_activated = true;
        m_activateTimer = m_activateTime;
        m_sprite.SetFPS(2);
    }

    m_sprite.SetPos(CameraService::RawToScreenX(m_x), CameraService::RawToScreenY(m_y));
    Oasis::Renderer::DrawAnimatedSprite(&m_sprite);
}