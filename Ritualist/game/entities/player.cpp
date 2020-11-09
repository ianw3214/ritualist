#include "player.hpp"

#include "game/camera/camera.hpp"
#include "game/input/input.hpp"

#include "game/game.hpp"
#include "game/progression.hpp"

Player::Player(float x, float y)
    : Entity(x, y, 80.f, 80.f, 20.f, 20.f)
    , m_sprite("res/player.png", 80, 80)
    , m_attackLeftAnim("res/vfx/player_attack_left.png", 160, 80)
    , m_attackRightAnim("res/vfx/player_attack_right.png", 160, 80)
    , m_attackDownAnim("res/vfx/player_attack_down.png", 80, 160)
    , m_attackUpAnim("res/vfx/player_attack_up.png", 80, 160)
    , m_speed(300.f)
    , m_attackCooldown(0.22f)
    , m_direction(Direction::RIGHT)
    , m_moving(false)
    , m_forceResetAnim(false)
    , m_attacking(false)
    , m_attackTimer(0.f)
    , m_teleporting(false)
    , m_teleportTimer(0.f)
    , m_teleportCooldown(0.f)
    , m_teleportInvulnTimer(0.f)
    , m_invulnTimer(0.f)
    , m_dead(false)
{
    m_sprite.SetDimensions(80.f, 80.f);
    m_sprite.AddAnimation("idle_left", 0, 3);
    m_sprite.AddAnimation("idle_right", 6, 9);
    m_sprite.AddAnimation("idle_down", 12, 15);
    m_sprite.AddAnimation("idle_up", 18, 21);
    m_sprite.AddAnimation("run_left", 24, 29);
    m_sprite.AddAnimation("run_right", 30, 35);
    m_sprite.AddAnimation("run_down", 36, 41);
    m_sprite.AddAnimation("run_up", 42, 47);
    m_sprite.AddAnimation("attack_left", 48, 52);
    m_sprite.AddAnimation("attack_right", 54, 58);
    m_sprite.AddAnimation("attack_down", 60, 64);
    m_sprite.AddAnimation("attack_up", 66, 70);
    m_sprite.AddAnimation("death_transition", 72, 74);
    m_sprite.AddAnimation("dead", 78, 78);
    m_sprite.AddAnimation("teleport", 84, 87);
    m_sprite.PlayAnimation("idle_right");
    m_sprite.SetFPS(12);

    m_attackLeftAnim.SetDimensions(160.f, 80.f);
    m_attackLeftAnim.AddAnimation("attack_left", 0, 4);
    m_attackLeftAnim.PlayAnimation("");
    m_attackLeftAnim.SetFPS(20);
    m_attackRightAnim.SetDimensions(160.f, 80.f);
    m_attackRightAnim.AddAnimation("attack_right", 0, 4);
    m_attackRightAnim.PlayAnimation("");
    m_attackRightAnim.SetFPS(20);
    m_attackDownAnim.SetDimensions(80.f, 160.f);
    m_attackDownAnim.AddAnimation("attack_down", 0, 4);
    m_attackDownAnim.PlayAnimation("");
    m_attackDownAnim.SetFPS(20);
    m_attackUpAnim.SetDimensions(80.f, 160.f);
    m_attackUpAnim.AddAnimation("attack_up", 0, 4);
    m_attackUpAnim.PlayAnimation("");
    m_attackUpAnim.SetFPS(20);


    // Let the camera class store a reference to the player (there should only be one)
    GameService::SetPlayer(this);

    GameService::AddAttackCallback([&](Target target, float dmg, float x, float y, float w, float h){
        if (target != Target::PLAYER)
        {
            return;
        }
        // Check for collision with player
        // HARD CODE THE OFFSETS
        float tx = m_x - m_w / 2.f + 20.f;
        if (x + w > tx && x < tx + 40.f && y + h > m_y && y < m_y + m_h)
        {
            // take the damage if we collide!
            TakeDamage(dmg);
        }
    });

    // Set player stats from progression system
    m_health = Progression::GetPlayerHealth();
    m_healthCapacity = Progression::GetPlayerHealthCapacity();
    // Set speed from progression system
    if (!Progression::CheckLevelFinished(Level::BOSS1))
    {
        m_speed *= 1.2f;
    }
}

void Player::Update(float delta)
{
    HandleTeleport(delta);
    HandleAttack(delta);
    HandleMovement(delta);

    m_invulnTimer -= delta;

    m_sprite.SetPos(CameraService::RawToScreenX(m_x - m_w / 2.f), CameraService::RawToScreenY(m_y));

    // Update player tint for when damaged
    float tint_alpha = std::max(m_invulnTimer / m_invulnTime, 0.f);
    // tint_alpha *= tint_alpha;   // Square for a more dramatic curve
    m_sprite.SetTint(Oasis::Colours::RED, tint_alpha);
    m_invulnTimer -= delta;

    if (m_attacking)
    {
        m_attackUpAnim.SetPos(CameraService::RawToScreenX(m_x - m_h / 2.f), CameraService::RawToScreenY(m_y));
        m_attackDownAnim.SetPos(CameraService::RawToScreenX(m_x - m_h / 2.f), CameraService::RawToScreenY(m_y - 80.f));
        m_attackLeftAnim.SetPos(CameraService::RawToScreenX(m_x - 80.f - m_h / 2.f), CameraService::RawToScreenY(m_y));
        m_attackRightAnim.SetPos(CameraService::RawToScreenX(m_x - m_h / 2.f), CameraService::RawToScreenY(m_y));
    }
    
    // Oasis::Renderer::DrawSprite(&m_sprite);
    Oasis::Renderer::DrawAnimatedSprite(&m_attackUpAnim);
    Oasis::Renderer::DrawAnimatedSprite(&m_sprite);
    Oasis::Renderer::DrawAnimatedSprite(&m_attackLeftAnim);
    Oasis::Renderer::DrawAnimatedSprite(&m_attackRightAnim);
    Oasis::Renderer::DrawAnimatedSprite(&m_attackDownAnim);
}

void Player::HandleAttack(float delta)
{
    // If we're not in combat, don't attack
    Ref<Entity> boss = GameService::GetBoss();
    if (!boss || boss->GetHealth() <= 0.f)
    {
        m_attacking = false;
        m_attackTimer = 0.f;
        m_sprite.SetFPS(12);
        return;
    }
    // If we're dead, also don't attack
    if (m_dead || m_teleporting)
    {
        return;
    }
    // just update the timer if we are already attacking
    if (m_attacking)
    {
        m_attackTimer -= delta;
        if (m_attackTimer <= 0.f)
        {
            m_sprite.SetFPS(12);
            m_attacking = false;
        }
    }
    else
    {
        if (InputService::Attack())
        {
            m_sprite.SetFPS(24);
            m_attacking = true;
            m_attackTimer = m_attackCooldown;
            if (m_direction == Direction::UP) 
            {
                m_sprite.PlayAnimation("attack_up", 1);
                m_sprite.QueueAnimation("idle_up");
                m_attackUpAnim.SetPos(CameraService::RawToScreenX(m_x - m_w / 2.f), CameraService::RawToScreenY(m_y));
                m_attackUpAnim.PlayAnimation("attack_up", 1, true);
                GameService::AddAttack(Target::ENEMY, 1.f, m_x - m_w / 2.f, m_y, 80.f, 160.f);
            }
            if (m_direction == Direction::DOWN) 
            {
                m_sprite.PlayAnimation("attack_down", 1);
                m_sprite.QueueAnimation("idle_down");
                m_attackDownAnim.SetPos(CameraService::RawToScreenX(m_x - m_w / 2.f), CameraService::RawToScreenY(m_y - 80.f));
                m_attackDownAnim.PlayAnimation("attack_down", 1, true);
                GameService::AddAttack(Target::ENEMY, 1.f, m_x - m_w / 2.f, m_y - 80.f, 80.f, 160.f);
            }
            if (m_direction == Direction::LEFT) 
            {
                m_sprite.PlayAnimation("attack_left", 1);
                m_sprite.QueueAnimation("idle_left");
                m_attackLeftAnim.SetPos(CameraService::RawToScreenX(m_x - 80.f - m_w / 2.f), CameraService::RawToScreenY(m_y));
                m_attackLeftAnim.PlayAnimation("attack_left", 1, true);
                GameService::AddAttack(Target::ENEMY, 1.f, m_x - 80.f - m_w / 2.f, m_y, 160.f, 80.f);
            }
            if (m_direction == Direction::RIGHT) 
            {
                m_sprite.PlayAnimation("attack_right", 1);
                m_sprite.QueueAnimation("idle_right");
                m_attackRightAnim.SetPos(CameraService::RawToScreenX(m_x - m_w / 2.f), CameraService::RawToScreenY(m_y));
                m_attackRightAnim.PlayAnimation("attack_right", 1, true);
                GameService::AddAttack(Target::ENEMY, 1.f, m_x - m_w / 2.f, m_y, 160.f, 80.f);
            }
        }
    }
}

void Player::HandleTeleport(float delta)
{
    // Can't move if attacking OR if dead
    if (m_attacking || m_dead)
    {
        m_moving = false;
        return;
    }
    // Can't use teleport if haven't unlocked
    if (!Progression::CheckLevelFinished(Level::BOSS1))
    {
        return;
    }
    if (m_teleporting)
    {
        m_teleportTimer -= delta;
        if (m_teleportTimer <= 0.f)
        {
            m_teleporting = false;
            m_forceResetAnim = true;
            // Reset the player position based on last facing direction
            if (m_direction == Direction::UP)
            {
                float yOffset = 0.f;
                while(GameService::CheckMapConstriants(m_x, m_y + yOffset + 1.f) && yOffset <= m_speed * m_teleportTime * m_teleportSpeedMod)
                {
                    yOffset += 1.f;
                }
                m_y += yOffset;
                m_direction = Direction::UP;
                m_moving = true;
            }
            if (m_direction == Direction::DOWN)
            {
                float yOffset = 0.f;
                while(GameService::CheckMapConstriants(m_x, m_y - yOffset - 1.f) && yOffset <= m_speed * m_teleportTime * m_teleportSpeedMod)
                {
                    yOffset += 1.f;
                }
                m_y -= yOffset;
                m_direction = Direction::DOWN;
                m_moving = true;
            }
            if (m_direction == Direction::LEFT)
            {
                float xOffset = 0.f;
                while(GameService::CheckMapConstriants(m_x - xOffset - 1.f, m_y) && xOffset <= m_speed * m_teleportTime * m_teleportSpeedMod)
                {
                    xOffset += 1.f;
                }
                m_x -= xOffset;
                m_direction = Direction::LEFT;
                m_moving = true;
            }
            if (m_direction == Direction::RIGHT)
            {
                float xOffset = 0.f;
                while(GameService::CheckMapConstriants(m_x + xOffset + 1.f, m_y) && xOffset <= m_speed * m_teleportTime * m_teleportSpeedMod)
                {
                    xOffset += 1.f;
                }
                m_x += xOffset;
                m_direction = Direction::RIGHT;
                m_moving = true;
            }
        }
    }
    else
    {
        if (InputService::Teleport())
        {
            if (m_teleportCooldown <= 0.f)
            {
                m_teleportCooldown = m_teleportCooldownTime;
                m_teleportInvulnTimer = m_teleportInvulnTime;
                m_teleporting = true;
                m_teleportTimer = m_teleportTime;
                m_sprite.PlayAnimation("teleport", 1, true);
            }
        }
    }
    m_teleportCooldown -= delta;
    m_teleportInvulnTimer -= delta;
}

void Player::HandleMovement(float delta)
{
    // Can't move if attacking OR if dead
    if (m_attacking || m_teleporting || m_dead)
    {
        m_moving = false;
        return;
    }

    bool was_moving = m_moving;
    Direction last_dir = m_direction;

    m_moving = false;
    if (InputService::MoveUp())
    {
        // m_y += m_speed * delta;
        float yOffset = 0.f;
        while(GameService::CheckMapConstriants(m_x, m_y + yOffset + 1.f) && yOffset <= m_speed * delta)
        {
            yOffset += 1.f;
        }
        m_y += yOffset;
        m_direction = Direction::UP;
        m_moving = true;
    }
    if (InputService::MoveDown())
    {
        // m_y -= m_speed * delta;
        float yOffset = 0.f;
        while(GameService::CheckMapConstriants(m_x, m_y - yOffset - 1.f) && yOffset <= m_speed * delta)
        {
            yOffset += 1.f;
        }
        m_y -= yOffset;
        m_direction = Direction::DOWN;
        m_moving = true;
    }
    if (InputService::MoveLeft())
    {
        float xOffset = 0.f;
        while(GameService::CheckMapConstriants(m_x - xOffset - 1.f, m_y) && xOffset <= m_speed * delta)
        {
            xOffset += 1.f;
        }
        m_x -= xOffset;
        // m_x -= m_speed * delta;
        m_direction = Direction::LEFT;
        m_moving = true;
    }
    if (InputService::MoveRight())
    {
        float xOffset = 0.f;
        while(GameService::CheckMapConstriants(m_x + xOffset + 1.f, m_y) && xOffset <= m_speed * delta)
        {
            xOffset += 1.f;
        }
        m_x += xOffset;
        // m_x += m_speed * delta;
        m_direction = Direction::RIGHT;
        m_moving = true;
    }
    if (m_moving != was_moving || m_direction != last_dir || m_forceResetAnim)
    {
        if (m_moving)
        {
            if (m_direction == Direction::UP) m_sprite.PlayAnimation("run_up");
            if (m_direction == Direction::DOWN) m_sprite.PlayAnimation("run_down");
            if (m_direction == Direction::LEFT) m_sprite.PlayAnimation("run_left");
            if (m_direction == Direction::RIGHT) m_sprite.PlayAnimation("run_right");
        }
        else
        {
            if (m_direction == Direction::UP) m_sprite.PlayAnimation("idle_up");
            if (m_direction == Direction::DOWN) m_sprite.PlayAnimation("idle_down");
            if (m_direction == Direction::LEFT) m_sprite.PlayAnimation("idle_left");
            if (m_direction == Direction::RIGHT) m_sprite.PlayAnimation("idle_right");
        }
        m_forceResetAnim = false;
    }
}

void Player::TakeDamage(float damage)
{
    // If we're already dead, just die
    if (m_dead)
    {
        return;
    }
    // Don't take damage if invulnerable!
    if (m_invulnTimer > 0.f)
    {
        return;
    }
    // Don't take damage if teleporting!
    if (m_teleportCooldown > 0.f)
    {
        return;
    }
    m_invulnTimer = m_invulnTime;
    m_health -= damage;
    if (m_health <= 0.f)
    {
        m_health = 0.f;
        m_dead = true;
        // Queue death animations
        m_sprite.SetFPS(3);
        m_sprite.PlayAnimation("death_transition");
        m_sprite.QueueAnimation("dead");
        CameraService::ScreenShake(1.5f, 12.f);
    }
    else
    {
        // Screenshake when taking damage
        CameraService::ScreenShake(0.3f, 10.f);
    }
}