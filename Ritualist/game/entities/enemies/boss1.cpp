#include "boss1.hpp"

#include "game/camera/camera.hpp"
#include "game/game.hpp"
#include "game/entities/player.hpp"

#define PI 3.14156f
#include <random>

// This is super bad code but idc anymore cause life do be like that
std::default_random_engine generator;

Boss1::Boss1()  
    : Entity(1200.f, 1200.f, 200.f, 200.f, 1.f, 100.f)
    , m_sprite("res/enemies/boss1.png", 200, 200)
    , m_moving(false)
    , m_targetX(0.f)
    , m_targetY(0.f)
    , m_retargetTimer(0.f)
    , m_attackTimer(7.f)
    , m_ringTimer(0.f)
    , m_rings()
    , m_invulnTimer(0.f)
{
    m_sprite.SetDimensions(200.f, 200.f);
    m_sprite.AddAnimation("idle", 0, 0);
    m_sprite.AddAnimation("die", 4, 7);
    m_sprite.PlayAnimation("idle");

    GameService::AddAttackCallback([&](Target target, float dmg, float x, float y, float w, float h){
        if (target != Target::ENEMY)
        {
            return;
        }
        // Check for collision with player
        if (x + w > m_x && x < m_x + m_w && y + h > m_y && y < m_y + m_h)
        {
            // take the damage if we collide!
            TakeDamage(dmg);
        }
    });
}

void Boss1::Update(float delta)
{
    HandleAttack(delta);
    HandleMovement(delta);

    m_sprite.SetPos(CameraService::RawToScreenX(m_x), CameraService::RawToScreenY(m_y));

    // Update boss tint for when damaged
    float tint_alpha = std::max(m_invulnTimer / m_invulnTime, 0.f);
    tint_alpha *= tint_alpha;   // Square for a more dramatic curve
    m_sprite.SetTint(Oasis::Colours::RED, tint_alpha);
    m_invulnTimer -= delta;

    // Oasis::Renderer::DrawSprite(&m_sprite);
    Oasis::Renderer::DrawAnimatedSprite(&m_sprite);
    for (Ring& ring : m_rings)
    {
        
        Oasis::Renderer::DrawAnimatedSprite(ring.m_attack);
    }
}

void Boss1::HandleAttack(float delta)
{
    // Don't bother attacking if dead
    if (GetHealth() <= 0.0) return;

    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    if (m_attackTimer <= 0.f)
    {
        m_attackTimer = m_attackTime;
        m_ringTimer = m_ringTime;
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
            const float radius = distribution(generator) * 150.f + 200.f;
            const float angle = distribution(generator) * 2 * PI;
            const float centerX = m_x - std::cos(angle);
            const float centerY = m_y - std::sin(angle);
            const float direction = distribution(generator) >= 0.5f ? 1.f : -1.f;
            add_ring(radius, centerX, centerY, angle, direction);
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

            // Add an attack for each ring to actually do damage
            GameService::AddAttack(Target::PLAYER, m_ringDamage, ring.m_currentX, ring.m_currentY, 160.f, 80.f);
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

void Boss1::HandleMovement(float delta)
{
    // Dont' bother moving if dead
    if (GetHealth() <= 0.0) return;

    std::uniform_real_distribution<float> distribution(0.0, 1.0);

    // Check if we need to retarget
    if (m_retargetTimer <= 0.f)
    {
        m_retargetTimer = m_retargetTime;
        // Find a random spot in a circle around the player
        float key = distribution(generator);
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

void Boss1::TakeDamage(float damage)
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
        // Despawn the rings
        for (Ring& ring : m_rings)
        {
            delete ring.m_attack;
        }
        m_rings.clear();
    }
}