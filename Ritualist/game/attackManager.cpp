#include "attackManager.hpp"

AttackManager::AttackManager()
    : m_head(nullptr)
{
    
}

void AttackManager::Update(float delta)
{
    while(!Empty())
    {
        Attack * curr = dequeue();
        // Loop over all the attack functions
        for (auto& func : m_funcs)
        {
            func(curr->m_target, curr->m_damage, curr->m_x, curr->m_y, curr->m_w, curr->m_h);
        }
        delete curr;
    }
}

void AttackManager::AddAttack(Target target, float damage, float x, float y, float w, float h)
{
    // Add attacks to the head of the queue
    Attack * attack = new Attack();
    attack->m_target = target;
    attack->m_damage = damage;
    attack->m_x = x;
    attack->m_y = y;
    attack->m_w = w;
    attack->m_h = h;
    attack->m_next = m_head;
    m_head = attack;
}

void AttackManager::AddCallback(attack_func func)
{
    m_funcs.push_back(func);
}

Attack * AttackManager::dequeue()
{
    Attack * temp = m_head;
    m_head = m_head->m_next;
    temp->m_next = nullptr;
    return temp;
}