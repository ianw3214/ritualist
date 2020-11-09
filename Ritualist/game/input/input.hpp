#pragma once
#include "oasis.h"

class InputService
{
public:
    static bool MoveLeft();
    static bool MoveRight();
    static bool MoveDown();
    static bool MoveUp();

    static bool Attack();
    static bool Teleport();
private:
    friend class Input;
    static Ref<Input> s_input;
};

// Keeps track of input state for the game
class Input : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual Oasis::IState * Update()  override;

    // Input state
    bool m_left;
    bool m_right;
    bool m_down;
    bool m_up;

    bool m_attack;
    bool m_teleport;
};