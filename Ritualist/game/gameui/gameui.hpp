#pragma once
#include "oasis.h"

// Keeps track of input state for the game
class GameUI : public Oasis::GameStateLayer
{
public:
    GameUI();

    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual Oasis::IState * Update()  override;
private:
    Oasis::Sprite m_healthUISprite;
    Oasis::Sprite m_bossHealthUISprite;
};