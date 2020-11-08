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

    bool m_activated;
};