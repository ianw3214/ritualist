#include "finalboss.hpp"

#include "game/camera/camera.hpp"
#include "game/game.hpp"
#include "game/progression.hpp"
#include "game/entities/player.hpp"

#define PI 3.14156f
#include <random>

// This is super bad code but idc anymore cause life do be like that
static std::random_device rd2;
static std::default_random_engine generator2(rd2());

FinalBoss::FinalBoss()
    : Entity(1200.f, 1200.f, 400.f, 400.f, 50.f, 50.f)
    , m_sprite("res/enemies/final_boss.png", 400, 400)
    , m_attack2Sprite("res/enemies/final_boss_attack2.png", 1200, 1200)
    , m_moving(false)
    , m_targetX(0.f)
    , m_targetY(0.f)
    , m_retargetTimer(0.f)
    , m_attackTimer(5.f)
    , m_ringTimer(0.f)
    , m_charging(false)
    , m_attack(0)
    , m_rings()
    , m_invulnTimer(0.f)
{
    m_sprite.SetDimensions(400.f, 400.f);
    m_sprite.AddAnimation("idle", 0, 1);
    m_sprite.AddAnimation("charge", 2, 3);
    m_sprite.AddAnimation("attack", 4, 5);
    m_sprite.AddAnimation("charge2", 6, 7);
    m_sprite.AddAnimation("die", 8, 9);
    m_sprite.SetFPS(8);
    m_sprite.PlayAnimation("idle");

    m_attack2Sprite.SetDimensions(1200.f, 1200.f);
    m_attack2Sprite.AddAnimation("default", 0, 1);
    m_attack2Sprite.PlayAnimation("default");

    GameService::AddAttackCallback([&](Target target, float dmg, float x, float y, float w, float h){
        if (target != Target::ENEMY)
        {
            return;
        }
        float tx = m_x - m_h / 2.f;
        // Check for collision with player
        if (x + w > tx && x < tx + m_w && y + h > m_y && y < m_y + m_h)
        {
            // take the damage if we collide!
            TakeDamage(dmg);
        }
    });
}

void FinalBoss::Update(float delta)
{
    HandleAttack(delta);
    HandleMovement(delta);

    m_sprite.SetPos(CameraService::RawToScreenX(m_x - m_h / 2.f), CameraService::RawToScreenY(m_y));
    m_attack2Sprite.SetPos(CameraService::RawToScreenX(m_x - m_h / 2.f - 400.f), CameraService::RawToScreenY(m_y - 400.f));

    // Update boss tint for when damaged
    float tint_alpha = std::max(m_invulnTimer / m_invulnTime, 0.f);
    tint_alpha *= tint_alpha;   // Square for a more dramatic curve
    m_sprite.SetTint(Oasis::Colours::RED, tint_alpha);
    m_attack2Sprite.SetTint(Oasis::Colours::RED, tint_alpha);
    m_invulnTimer -= delta;

    // If we are on attack 2 and not charging - play that sprite instead
    if (m_attack == 1 && !m_charging)
    {
        Oasis::Renderer::DrawAnimatedSprite(&m_attack2Sprite);
        // HACK: Add risidual damage here
        GameService::AddAttack(Target::PLAYER, 2.f, m_x - m_w * 1.5f, m_y, 1200.f, 400.f);
        GameService::AddAttack(Target::PLAYER, 2.f, m_x - m_w * 0.5f, m_y - 400.f, 400.f, 1200.f);
    }
    else
    {
        // Oasis::Renderer::DrawSprite(&m_sprite);
        Oasis::Renderer::DrawAnimatedSprite(&m_sprite);
    }
    for (Ring& ring : m_rings)
    {
        
        Oasis::Renderer::DrawAnimatedSprite(ring.m_attack);
    }
}

void FinalBoss::HandleAttack(float delta)
{
    // Don't bother attacking if dead
    if (GetHealth() <= 0.0) return;

    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    if (m_attackTimer <= 0.f)
    {
        if (m_charging)
        {
            // Ring attack
            if (m_attack == 0)
            {
                m_charging = false;
                m_attackTimer = m_attackTime;
                m_ringTimer = m_ringTime;
                // Artificial time to retarget after an attack
                m_retargetTimer = 1.5f;
                auto add_ring = [&](float radius, float centerX, float centerY, float angle, float direction){
                    Ring ring{m_x, m_y, radius, centerX, centerY, angle, direction};
                    ring.m_attack = new Oasis::AnimatedSprite("res/vfx/boss1_attack.png", 160, 80);
                    ring.m_attack->SetDimensions(160.f, 80.f);
                    ring.m_attack->AddAnimation("default", 0, 2);
                    ring.m_attack->PlayAnimation("default");
                    m_rings.emplace_back(ring);
                };
                for (unsigned int i = 0; i < m_ringsPerAttack; ++i)
                {
                    // arbitrary numbers
                    const float radius = distribution(generator2) * 350.f + 250.f;
                    const float angle = distribution(generator2) * 2 * PI;
                    const float centerX = m_x - std::cos(angle);
                    const float centerY = m_y - std::sin(angle);
                    const float direction = distribution(generator2) >= 0.5f ? 1.f : -1.f;
                    add_ring(radius, centerX, centerY, angle, direction);
                }   
                m_sprite.PlayAnimation("attack");
            }
            // Stinger attack
            else
            {
                // Artificial time to retarget after an attack
                m_retargetTimer = 1.0f;
                m_charging = false;
                m_attackTimer = m_attack2Time;
                // cross pattern around the boss
                // BIG initial damage
                GameService::AddAttack(Target::PLAYER, 5.f, m_x - m_w * 1.5f, m_y, 1200.f, 400.f);
                GameService::AddAttack(Target::PLAYER, 5.f, m_x - m_w * 0.5f, m_y - 400.f, 400.f, 1200.f);
            }
        }
        else
        {
            // Decide which attack to use
            std::uniform_real_distribution<float> distribution(0.0, 1.0);
            const int key = int(distribution(generator2) + 0.5);    // Round down to either 0 or 1
            // Ring attack
            if (key == 0)
            {
                m_charging = true;
                m_attackTimer = m_attackChargeTime;
                m_sprite.PlayAnimation("charge");
                m_attack = 0;
            }
            // Stinger attack
            else
            {
                m_charging = true;
                m_attackTimer = m_attack2ChargeTime;
                m_sprite.PlayAnimation("charge2");
                m_attack = 1;
            }
        }
        
        return;
    }
    m_attackTimer -= delta;
    m_ringTimer -= delta;
    // Update the rings
    if (m_ringTimer >= 0.f)
    {
        const float time_elapsed = m_ringTime - m_ringTimer;
        for (Ring& ring : m_rings)
        {
            const float curr_angle = ring.m_startAngle + time_elapsed * m_ringAngleSpeed * ring.m_direction;
            const float xOffset = std::cos(curr_angle) * ring.m_radius;
            const float yOffset = std::sin(curr_angle) * ring.m_radius;
            ring.m_currentX = ring.m_centerX + xOffset;
            ring.m_currentY = ring.m_centerY + yOffset;
            ring.m_attack->SetPos(CameraService::RawToScreenX(ring.m_currentX), CameraService::RawToScreenY(ring.m_currentY));

            // Add an attack for each ring to actually do damage (be a little more lenient)
            GameService::AddAttack(Target::PLAYER, m_ringDamage, ring.m_currentX + 10.f, ring.m_currentY + 10.f, 160.f - 20.f, 80.f - 20.f);
        }
    }
    else
    {
        for (Ring& ring : m_rings)
        {
            delete ring.m_attack;
        }
        m_rings.clear();
    }
}

void FinalBoss::HandleMovement(float delta)
{
    // Dont' bother moving if dead
    if (GetHealth() <= 0.0) return;

    std::uniform_real_distribution<float> distribution(0.0, 1.0);

    // Check if we need to retarget
    if (m_retargetTimer <= 0.f && !m_charging)
    {
        // Super hack to force boss away from attack 2 sprite
        m_attack = 0;
        m_sprite.PlayAnimation("idle");
        m_retargetTimer = m_retargetTime;
        // Find a random spot in a circle around the player
        const float key = distribution(generator2);
        const float x_offset = static_cast<float>(std::cos(2 * PI * key));
        const float y_offset = static_cast<float>(std::sin(2 * PI * key));

        Ref<Player> player = GameService::GetPlayer();
        m_targetX = player->GetX() + x_offset * m_retargetRadius;
        m_targetY = player->GetY() + y_offset * m_retargetRadius;
        return;
    }
    m_retargetTimer -= delta;

    // Move towards the target
    if (m_x < m_targetX)
    {
        m_x += m_speed * delta;
        if (m_x > m_targetX) m_x = m_targetX;
    }
    if (m_x > m_targetX)
    {
        m_x -= m_speed * delta;
        if (m_x < m_targetX) m_x = m_targetX;
    }
    if (m_y < m_targetY)
    {
        m_y += m_speed * delta;
        if (m_y > m_targetY) m_y = m_targetY;
    }
    if (m_y > m_targetY)
    {
        m_y -= m_speed * delta;
        if (m_y < m_targetY) m_y = m_targetY;
    }
}

void FinalBoss::TakeDamage(float damage)
{
    // Don't want to die twice :o
    if (GetHealth() <= 0.0f) return;
    // Don't take damage if invulnerable!
    if (m_invulnTimer > 0.f) return;
    m_invulnTimer = m_invulnTime;
    m_health -= damage;
    if (m_health <= 0.f)
    {
        m_sprite.SetFPS(10);
        m_sprite.PlayAnimation("die", 1, true);
        // TODO: Death animation
        // m_sprite.PlayAnimation("die", 1, true);
        // Despawn the rings
        for (Ring& ring : m_rings)
        {
            delete ring.m_attack;
        }
        m_rings.clear();
        // Game is won!
        Progression::SetGameWon();
        CameraService::ScreenShake(4.4f, 10.f);
    }
    else
    {
        // Screenshake when taking damage
        CameraService::ScreenShake(0.15f, 4.f);   
    }
}