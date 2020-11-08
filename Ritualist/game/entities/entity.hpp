#pragma once

class Entity
{
public:
    Entity(float x, float y, float w, float h, float health, float capacity)
        : m_x(x)
        , m_y(y)
        , m_w(w)
        , m_h(h)
        , m_layer(0)
        , m_health(health)
        , m_healthCapacity(capacity)
    {}

    inline float GetX() const { return m_x; }
    inline float GetY() const { return m_y; }
    inline float GetWidth() const { return m_w; }
    inline float GetHeight() const { return m_h; }
    inline float GetHealth() const { return m_health; }
    inline float GetHealthCapacity() const { return m_healthCapacity; }

    inline int GetLayer() const { return m_layer; }
    inline void SetLayer(int layer) { m_layer = layer; }

    virtual void Update(float delta) = 0;
protected:
    float m_x, m_y;
    float m_w, m_h;

    int m_layer;

    float m_health;
    float m_healthCapacity;
};