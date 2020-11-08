#pragma once
#include "oasis.h"

class Map
{
public:
    Map(const std::string& image_path, const std::string& collision_path);

    void RenderMap();
    bool CheckCollision(float x, float y);
private:
    Oasis::Sprite * m_map;
    // Collision map (use int because bool is wacky)
    int m_collisionMapWidth, m_collisionMapHeight;
    std::vector<std::vector<int>> m_collisionMaps;
};