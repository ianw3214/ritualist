#pragma once
#include "oasis.h"
#include "entity.hpp"

class Altar : public Entity 
{
public:
    Altar();

    virtual void Update(float delta) override;
private:
    Oasis::AnimatedSprite m_sprite;

    float m_activateTimer;
    const float m_activateTime = 2.f;
    bool m_activated;
    const float m_activateRadius = 240.f;
};