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
    Player(float x = 0.f, float y = 0.f);

    inline bool IsDead() const { return m_dead; }
    inline bool IsTelporting() const { return m_teleporting; }

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
    bool m_forceResetAnim;

    bool m_attacking;
    float m_attackTimer;
    bool m_teleporting;
    float m_teleportTimer;
    const float m_teleportTime = 0.25f;
    const float m_teleportSpeedMod = 2.f;
    float m_teleportCooldown;
    float m_teleportInvulnTimer;
    const float m_teleportCooldownTime = 0.8f;
    const float m_teleportInvulnTime = 0.5;
    float m_invulnTimer;
    const float m_invulnTime = 1.1f;
    bool m_dead;

private:
    void HandleAttack(float delta);
    void HandleTeleport(float delta);
    void HandleMovement(float delta);

private:
    void TakeDamage(float damage);
};