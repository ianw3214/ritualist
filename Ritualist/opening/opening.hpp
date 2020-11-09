#pragma once
#include "oasis.h"

constexpr float kCreditsSceneTimer = 1.f;

class Opening : public Oasis::IState
{
public:
    Opening();

    virtual void Init() override;
    virtual void Close() override;

    virtual void OnEvent(const Oasis::Event& event) override;
    virtual Oasis::IState * Update() override;
private:
    Oasis::Sprite m_background;

    // timers
    float m_creditsScreenTimer;

    // transition timers
    bool m_buttonPressed;
    float m_fadeInTimer;
    const float m_fadeInTime = 4.f;
    float m_fadeOutTimer;
    const float m_fadeOutTime = 1.5f;
};