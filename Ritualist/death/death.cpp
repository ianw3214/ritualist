#include "death.hpp"

#include "game/game.hpp"
#include "game/progression.hpp"

Death::Death()
    : m_background("res/ui_assets/dead.png")
    , m_deathScreenTimer(kDeathScreenTimer)
    , m_buttonPressed(false)
{

}

void Death::Init()
{
    m_fadeInTimer = m_fadeInTime;
    m_fadeOutTimer = 0.f;
}

void Death::Close()
{

}

void Death::OnEvent(const Oasis::Event& event)
{
    if (event.GetType() == Oasis::EventType::KEY_PRESSED)
    {
        if (m_deathScreenTimer < 0.f && !m_buttonPressed)
        {
            m_buttonPressed = true;
            m_fadeOutTimer = m_fadeOutTime;
            Progression::ResetProgression();
            GameService::ChangeLevel(Level::LOBBY);
        }
    }
}

Oasis::IState * Death::Update()
{
    float delta = Oasis::WindowService::GetDeltaF() / 1000000.f;

    m_background.SetDimensions((float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight());
    Oasis::Renderer::DrawSprite(&m_background);

    if (m_fadeInTimer >= 0.f)
    {
        m_fadeInTimer -= delta;
        float alpha = 1.f - (m_fadeInTime - m_fadeInTimer) / m_fadeInTime;
        Oasis::Renderer::DrawQuad(0.f, 0.f, (float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight(), Oasis::Colours::BLACK, alpha);
    }
    if (m_fadeOutTimer > 0.f)
    {
        m_fadeOutTimer -= delta;
        float alpha = (m_fadeOutTime - m_fadeOutTimer) / m_fadeOutTime;
        Oasis::Renderer::DrawQuad(0.f, 0.f, (float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight(), Oasis::Colours::BLACK, alpha);
        if (m_fadeOutTimer <= 0.f)
        {
            return new Game();
        }
    }
    
    m_deathScreenTimer -= delta;

    return nullptr;
}
