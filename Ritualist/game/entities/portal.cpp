#include "portal.hpp"

#include "game/game.hpp"
#include "game/camera/camera.hpp"
#include "game/input/input.hpp"

#include "game/entities/player.hpp"

Portal::Portal(float x, float y)
    : Entity(x, y, 180.f, 80.f, 0.f, 0.f)
    , m_sprite("res/portal.png", 180.f, 80.f)
{
    m_sprite.SetDimensions(180.f, 80.f);
    m_sprite.AddAnimation("default", 0, 0);
    m_sprite.PlayAnimation("default");
}

void Portal::Update(float delta)
{
    if (InputService::Attack())
    {
        // Check player proximity with altar
        Ref<Player> player = GameService::GetPlayer();
        const float xDist = (GetX() + GetWidth() / 2) - player->GetX();
        const float yDist = (GetY() + GetWidth() / 2) - player->GetY();
        if (xDist * xDist + yDist * yDist <= m_activateRadius * m_activateRadius)
        {
            GameService::ChangeLevel(Level::BOSS1);
        }
    }

    m_sprite.SetPos(CameraService::RawToScreenX(m_x), CameraService::RawToScreenY(m_y));
    Oasis::Renderer::DrawAnimatedSprite(&m_sprite);
}