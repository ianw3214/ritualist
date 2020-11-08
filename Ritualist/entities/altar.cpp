#include "altar.hpp"

#include "game.hpp"

Altar::Altar()
    : Entity(300.f, 300.f, 200.f, 200.f, 0.f, 0.f)
    , m_sprite("res/altar.png", 160.f, 480.f)
    , m_activated(false)
{
    m_sprite.SetDimensions(160.f, 480.f);
    m_sprite.AddAnimation("default", 0, 0);
    m_sprite.AddAnimation("activated", 1, 1);
    m_sprite.PlayAnimation("default");   
}

void Altar::Update(float delta)
{
    // The interact key is the same as the attack key
    if (InputService::Attack())
    {
        Oasis::Console::Log("INTERACT WITH ALTAR!");
    }

    // Want to check whether to activate altar or not
    if (GameService::ActivateAltar())
    {
        m_sprite.PlayAnimation("activated");
    }

    m_sprite.SetPos(CameraService::RawToScreenX(m_x), CameraService::RawToScreenY(m_y));
    Oasis::Renderer::DrawAnimatedSprite(&m_sprite);
}