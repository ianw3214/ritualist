#include "portal.hpp"

#include "game/game.hpp"
#include "game/camera/camera.hpp"
#include "game/input/input.hpp"

#include "game/entities/player.hpp"

#include "game/progression.hpp"

Portal::Portal(float x, float y)
    : Entity(x, y, 180.f, 80.f, 0.f, 0.f)
    , m_sprite("res/portal.png", 180.f, 80.f)
{
    m_sprite.SetDimensions(180.f, 80.f);
    m_sprite.AddAnimation("default", 0, 1);
    m_sprite.AddAnimation("hover", 2, 3);
    m_sprite.AddAnimation("closed", 4, 5);
    m_sprite.PlayAnimation("default");
}

void Portal::Update(float delta)
{
    if (Progression::CheckLevelFinished(Level::BOSS1))
    {
        m_sprite.PlayAnimation("closed");
    }
    else
    {
        // Check player proximity with altar
        Ref<Player> player = GameService::GetPlayer();
        const float xDist = (GetX() + GetWidth() / 2) - player->GetX();
        const float yDist = (GetY() + GetHeight() / 2) - player->GetY();
        if (xDist * xDist + yDist * yDist <= m_activateRadius * m_activateRadius)
        {
            if (InputService::Attack())
            {
                GameService::ChangeLevel(Level::BOSS1);   
            }
            m_sprite.PlayAnimation("hover");
        }
        else
        {
            m_sprite.PlayAnimation("default");
        }
    }

    m_sprite.SetPos(CameraService::RawToScreenX(m_x), CameraService::RawToScreenY(m_y));
    Oasis::Renderer::DrawAnimatedSprite(&m_sprite);
}