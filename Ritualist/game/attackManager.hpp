#pragma once
#include "oasis.h"

#include <vector>
#include <functional>

////////////////////////////////////////////////////////////
enum class Target 
{
    PLAYER = 0,
    ENEMY,
    COUNT
};

////////////////////////////////////////////////////////////
struct Attack
{
    Target m_target;
    float m_damage;
    // Info for our collision box
    float m_x, m_y;
    float m_w, m_h;

    // Linked list impl
    Attack * m_next;
};

////////////////////////////////////////////////////////////
// The gamelayer will own an instance of this
typedef std::function<void(Target, float, float, float, float, float)> attack_func;
class AttackManager
{
public:
    AttackManager();

    void Update(float delta);

    void AddAttack(Target target, float damage, float x, float y, float w, float h);
    void AddCallback(attack_func func);
private:
    inline bool Empty() const { return m_head == nullptr; }
    Attack * dequeue();
    Attack * m_head;

    std::vector<attack_func> m_funcs;
};