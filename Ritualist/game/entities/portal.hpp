#pragma once
#include "oasis.h"

#include "entity.hpp"

class Portal : public Entity
{
public:
    Portal(float x = 0.f, float y = 0.f);

    virtual void Update(float delta) override;
private:
    Oasis::AnimatedSprite m_sprite;

    const float m_activateRadius = 90.f;
};