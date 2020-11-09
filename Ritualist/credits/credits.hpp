#pragma once
#include "oasis.h"

constexpr float kCreditsSceneTimer = 3.f;

class Credits : public Oasis::IState
{
public:
    Credits();

    virtual void Init() override;
    virtual void Close() override;

    virtual void OnEvent(const Oasis::Event& event) override;
    virtual Oasis::IState * Update() override;
private:
    void DrawCredits(float delta);
private:
    Oasis::Sprite m_background;
    Oasis::Sprite m_text;

    // timers
    float m_creditsScreenTimer;

    // Text location
    float m_textLocation;
    const float m_textSpeed = 200.f;

    // transition timers
    bool m_buttonPressed;
    float m_fadeInTimer;
    const float m_fadeInTime = 5.f;
    float m_fadeOutTimer;
    const float m_fadeOutTime = 1.5f;
};