#include "credits.hpp"

#include "game/game.hpp"
#include "game/progression.hpp"

Credits::Credits()
    : m_background("res/ui_assets/credits.png")
    , m_text("res/ui_assets/credits_text.png")
    , m_creditsScreenTimer(kCreditsSceneTimer)
    , m_buttonPressed(false)
{
    m_textLocation = Oasis::WindowService::WindowWidth() / 2.f - m_text.GetWidth() / 2.f;
}

void Credits::Init()
{
    m_fadeInTimer = m_fadeInTime;
    m_fadeOutTimer = 0.f;
}

void Credits::Close()
{

}

void Credits::OnEvent(const Oasis::Event& event)
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

Oasis::IState * Credits::Update()
{
    float delta = Oasis::WindowService::GetDeltaF() / 1000000.f;

    m_background.SetDimensions((float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight());
    Oasis::Renderer::DrawSprite(&m_background);

    DrawCredits(delta);

    // Draw the final time as well
    const float time = Progression::GetTotalTime() / 1000000.f;
    std::string time_str("TIME: ");
    time_str += std::to_string(time);
    time_str += "s";
    Oasis::TextRenderer::DrawString("large", time_str, 60.f, 560.f, Oasis::Colour{.1f, .1f, .1f});

    if (m_fadeInTimer >= 0.f)
    {
        m_fadeInTimer -= delta;
        float alpha = 1.f - (m_fadeInTime - m_fadeInTimer) / m_fadeInTime;
        Oasis::Renderer::DrawQuad(0.f, 0.f, (float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight(), Oasis::Colours::WHITE, alpha);
    }
    if (m_fadeOutTimer > 0.f)
    {
        m_fadeOutTimer -= delta;
        float alpha = (m_fadeOutTime - m_fadeOutTimer) / m_fadeOutTime;
        Oasis::Renderer::DrawQuad(0.f, 0.f, (float)Oasis::WindowService::WindowWidth(), (float)Oasis::WindowService::WindowHeight(), Oasis::Colours::WHITE, alpha);
        if (m_fadeOutTimer <= 0.f)
        {
            exit(0);
        }
    }

    m_creditsScreenTimer -= delta;

    return nullptr;
}

void Credits::DrawCredits(float delta)
{
    const float x = m_textLocation;
    const float y = Oasis::WindowService::WindowHeight() / 2.f - m_text.GetHeight() / 2.f;
    m_text.SetPos(x, y);
    Oasis::Renderer::DrawSprite(&m_text);
    // Render again at the right if the text should wrap
    if (x < 0.f)
    {
        m_text.SetPos(x + Oasis::WindowService::WindowWidth(), y);
        Oasis::Renderer::DrawSprite(&m_text);
    }
    m_textLocation -= delta * m_textSpeed;
    // See if we need to reset text location
    if (m_textLocation < -m_text.GetWidth())
    {
        m_textLocation = Oasis::WindowService::WindowWidth() - m_text.GetWidth();
    }
}