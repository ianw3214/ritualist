#pragma once
#include "oasis.h"

#include <vector>

#include "../entity.hpp"

class FinalBoss : public Entity
{
public:
    FinalBoss();

    inline float GetX() const { return m_x; }
    inline float GetY() const { return m_y; }
 
    virtual void Update(float delta) override;
private:
    Oasis::AnimatedSprite m_sprite;
    Oasis::AnimatedSprite m_attack2Sprite;

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
    bool m_charging;
    int m_attack;
    const float m_attackTime = 7.5f;
    const float m_attack2Time = 2.5f;
    const float m_attackChargeTime = 1.5f;
    const float m_attack2ChargeTime = 1.f;
    const float m_ringTime = 5.5f;
    const float m_ringAngleSpeed = 2.5f;
    const unsigned int m_ringsPerAttack = 6;
    const float m_ringDamage = 2.5f;
    std::vector<Ring> m_rings;

    float m_invulnTimer;
    const float m_invulnTime = 0.7f;

private:
    void HandleAttack(float delta);
    void HandleMovement(float delta);
private:
    void TakeDamage(float damage);
};