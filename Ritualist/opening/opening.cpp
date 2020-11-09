#include "opening.hpp"

#include "game/game.hpp"
#include "game/progression.hpp"

Opening::Opening()
    : m_background("res/opening.png")
    , m_creditsScreenTimer(kCreditsSceneTimer)
    , m_buttonPressed(false)
{

}

void Opening::Init()
{
    m_fadeInTimer = m_fadeInTime;
    m_fadeOutTimer = 0.f;

    UIManager::HideWindow(kAltarUiName);
}

void Opening::Close()
{

}

void Opening::OnEvent(const Oasis::Event& event)
{
    if (event.GetType() == Oasis::EventType::KEY_PRESSED)
    {
        if (m_creditsScreenTimer < 0.f && !m_buttonPressed)
        {
            m_buttonPressed = true;
            m_fadeOutTimer = m_fadeOutTime;
        }
    }
}

Oasis::IState * Opening::Update()
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

    m_creditsScreenTimer -= delta;

    return nullptr;
}