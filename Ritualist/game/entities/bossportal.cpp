#include "bossportal.hpp"

#include "game/game.hpp"
#include "game/progression.hpp"
#include "game/camera/camera.hpp"
#include "game/input/input.hpp"

#include "game/entities/player.hpp"

BigPortal::BigPortal(float x, float y)
    : Entity(x, y, 180.f, 80.f, 0.f, 0.f)
    , m_sprite("res/portal_large.png", 270.f, 120.f)
{
    m_sprite.SetDimensions(270.f, 120.f);
    m_sprite.AddAnimation("default", 0, 1);
    m_sprite.AddAnimation("hover", 2, 3);
    m_sprite.AddAnimation("closed", 4, 5);
    m_sprite.PlayAnimation("default");
}

void BigPortal::Update(float delta)
{
    // Check that all levels are already completed
    bool available = true;
    for (int i = 0; i < static_cast<int>(Level::COUNT); ++i)
    {
        Level level = static_cast<Level>(i);
        if (level == Level::LOBBY) continue;
        if (level == Level::FINALBOSS) continue;
        if (!Progression::CheckLevelFinished(level))
        {
            available = false;
            break;
        }
    }
    if (available)
    {
        // Check player proximity with altar
        Ref<Player> player = GameService::GetPlayer();
        const float xDist = (GetX() + GetWidth() / 2) - player->GetX();
        const float yDist = (GetY() + GetHeight() / 2) - player->GetY();
        if (xDist * xDist + yDist * yDist <= m_activateRadius * m_activateRadius)
        {
            if (InputService::Attack())
            {
                GameService::ChangeLevel(Level::FINALBOSS);
            }
            m_sprite.PlayAnimation("hover");
        }
        else
        {
            m_sprite.PlayAnimation("default");
        }
    }
    else
    {
        m_sprite.PlayAnimation("closed");
    }

    m_sprite.SetPos(CameraService::RawToScreenX(m_x), CameraService::RawToScreenY(m_y));
    Oasis::Renderer::DrawAnimatedSprite(&m_sprite);
}