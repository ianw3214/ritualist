#include "map.hpp"

#include "game/camera/camera.hpp"

#include <stb/stb_image.h>

Map::Map(const std::string& image_path, const std::string& collision_path)
    : m_collisionMapWidth(0)
    , m_collisionMapHeight(0)
{
    m_map = new Oasis::Sprite(image_path);

    // Initialize the collision map
    unsigned char * localBuffer = nullptr;
    int width, height, bitsPerPixel;
    localBuffer = stbi_load(collision_path.c_str(), &width, &height, &bitsPerPixel, 1);
    for (unsigned int i = 0; i < (unsigned int) width; ++i)
    {
        m_collisionMaps.emplace_back();
        for (unsigned int j = 0; j < (unsigned int) height; ++j)
        {
            m_collisionMaps[i].emplace_back(localBuffer[j * width + i]);
        }
    }
    m_collisionMapWidth = width;
    m_collisionMapHeight = height;
    if (localBuffer)
    {
        stbi_image_free(localBuffer);
    }
}

void Map::RenderMap()
{
    m_map->SetPos(CameraService::RawToScreenX(0.f), CameraService::RawToScreenY(0.f));
    Oasis::Renderer::DrawSprite(m_map);
}

bool Map::CheckCollision(float x, float y)
{
    // Convert x and y to map grid coordinates
    if (x < 0.f || y < 0.f || x >= m_map->GetWidth() || y >= m_map->GetHeight())
    {
        return true;
    }
    int x_grid = (int)(x / static_cast<float>(m_map->GetWidth()) * m_collisionMapWidth);
    int y_grid = (int)(y / static_cast<float>(m_map->GetHeight()) * m_collisionMapHeight);
    // If the part of the bitmap is not white, we are colliding
    if (m_collisionMaps[x_grid][y_grid] != 255)
    {
        return true;
    }
    return false;
}