#pragma once
#include "oasis.h"

#include <vector>

#include "../entity.hpp"

class Boss1 : public Entity
{
public:
    Boss1();

    inline float GetX() const { return m_x; }
    inline float GetY() const { return m_y; }
 
    virtual void Update(float delta) override;
private:
    Oasis::AnimatedSprite m_sprite;

    const float m_speed = 400.f;

    bool m_moving;
    float m_targetX;
    float m_targetY;
    float m_retargetTimer;
    const float m_retargetRadius = 450.f;
    const float m_retargetTime = 1.5f;

    struct Ring
    {
        float m_currentX;
        float m_currentY;

        float m_radius;
        float m_centerX;
        float m_centerY;
        float m_startAngle;
        float m_direction;  // either 1.0 or -1.0 for clockwise/counterclockwise
        // Put animated sprite in ring so each ring has distinct animation
        Oasis::AnimatedSprite * m_attack;
    };
    float m_attackTimer;
    float m_ringTimer;
    const float m_attackTime = 10.f;
    const float m_ringTime = 5.f;
    const float m_ringAngleSpeed = 3.0f;
    const unsigned int m_ringsPerAttack = 4;
    const float m_ringDamage = 2.f;
    std::vector<Ring> m_rings;

    float m_invulnTimer;
    const float m_invulnTime = 0.7f;

private:
    void HandleAttack(float delta);
    void HandleMovement(float delta);
private:
    void TakeDamage(float damage);
};