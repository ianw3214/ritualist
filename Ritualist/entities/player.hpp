#pragma once
#include "oasis.h"

#include "entity.hpp"

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Player : public Entity
{
public:
    Player();

    virtual void Update(float delta) override;
private:
    Oasis::AnimatedSprite m_sprite;
    // Attack vfx animations
    Oasis::AnimatedSprite m_attackLeftAnim;
    Oasis::AnimatedSprite m_attackRightAnim;
    Oasis::AnimatedSprite m_attackDownAnim;
    Oasis::AnimatedSprite m_attackUpAnim;

    // Player 'constants'
    float m_speed;
    float m_attackCooldown;

    Direction m_direction;
    bool m_moving;

    bool m_attacking;
    float m_attackTimer;
    float m_invulnTimer;
    const float m_invulnTime = 0.7f;

private:
    void HandleAttack(float delta);
    void HandleMovement(float delta);

private:
    void TakeDamage(float damage);
};