#include "input.hpp"

Ref<Input> InputService::s_input = nullptr;

bool InputService::MoveLeft()
{
    return s_input->m_left;
}

bool InputService::MoveRight()
{
    return s_input->m_right;
}

bool InputService::MoveDown()
{
    return s_input->m_down;
}

bool InputService::MoveUp()
{
    return s_input->m_up;
}

bool InputService::Attack()
{
    return s_input->m_attack;
}

void Input::Init()
{
    InputService::s_input = this;
}

void Input::Close()
{

}

bool Input::HandleEvent(const Oasis::Event& event)
{
    if (event.GetType() == Oasis::EventType::KEY_PRESSED)
    {
        auto keyEvent = dynamic_cast<const Oasis::KeyPressedEvent&>(event);
        auto key = keyEvent.GetKey();
        if (key == SDL_SCANCODE_LEFT || key == SDL_SCANCODE_A)
        {
            m_left = true;
        }
        if (key == SDL_SCANCODE_RIGHT || key == SDL_SCANCODE_D)
        {
            m_right = true;
        }
        if (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_W)
        {
            m_up = true;
        }
        if (key == SDL_SCANCODE_DOWN || key == SDL_SCANCODE_S)
        {
            m_down = true;
        }
        if (key == SDL_SCANCODE_Z || key == SDL_SCANCODE_COMMA)
        {
            m_attack = true;
        }
    }
    if (event.GetType() == Oasis::EventType::KEY_RELEASED)
    {
        auto keyEvent = dynamic_cast<const Oasis::KeyReleasedEvent&>(event);
        auto key = keyEvent.GetKey();
        if (key == SDL_SCANCODE_LEFT || key == SDL_SCANCODE_A)
        {
            m_left = false;
        }
        if (key == SDL_SCANCODE_RIGHT || key == SDL_SCANCODE_D)
        {
            m_right = false;
        }
        if (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_W)
        {
            m_up = false;
        }
        if (key == SDL_SCANCODE_DOWN || key == SDL_SCANCODE_S)
        {
            m_down = false;
        }
        if (key == SDL_SCANCODE_Z || key == SDL_SCANCODE_COMMA)
        {
            m_attack = false;
        }
    }
    return false;
}

Oasis::IState * Input::Update()
{
    return nullptr;
}