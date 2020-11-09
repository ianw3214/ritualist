#include "camera.hpp"

#include "game/game.hpp"
#include "game/entities/player.hpp"

constexpr float kCameraSpeed = 500.f;

Ref<Camera> CameraService::s_camera;

float CameraService::ScreenToRawX(int x)
{
    const float scale = s_camera->m_scale;
    return static_cast<float>(x - Oasis::WindowService::WindowWidth() / 2) / scale + s_camera->m_x;

}

float CameraService::ScreenToRawY(int y)
{
    const float scale = s_camera->m_scale;
    return static_cast<float>(Oasis::WindowService::WindowHeight() - y - Oasis::WindowService::WindowHeight() / 2) / scale + s_camera->m_y;
}

float CameraService::RawToScreenX(float x, float parallax)
{
    const float scale = s_camera->m_scale;
    const float window_width = static_cast<float>(Oasis::WindowService::WindowWidth());
    return (x - s_camera->m_x) / parallax * scale + window_width / 2;
}

float CameraService::RawToScreenY(float y, float parallax)
{
    const float scale = s_camera->m_scale;
    float window_height = static_cast<float>(Oasis::WindowService::WindowHeight());
    return (y - s_camera->m_y)  / parallax * scale + window_height / 2;
}

Camera::Camera()
    : m_x(0.f)
    , m_y(0.f)
    , m_scale(1.f)
    , m_initialized(false)
{
    
}

void Camera::Init()
{
	CameraService::s_camera = this;
}

void Camera::Close()
{
    
}

bool Camera::HandleEvent(const Oasis::Event& event)
{
    return false;
}

Oasis::IState * Camera::Update() 
{
    Ref<Player> player = GameService::GetPlayer();
    if (!m_initialized)
    {
        m_x = player->GetX();
        m_y = player->GetY();
        m_initialized = true;
    }
    else
    {
        // Microseconds to seconds
        float delta = Oasis::WindowService::GetDeltaF() / 1000000.f;
        if (m_x < player->GetX())
        {
            m_x += kCameraSpeed * delta;
            if (m_x > player->GetX()) m_x = player->GetX();
        }
        if (m_x > player->GetX())
        {
            m_x -= kCameraSpeed * delta;
            if (m_x < player->GetX()) m_x = player->GetX();
        }
        if (m_y < player->GetY())
        {
            m_y += kCameraSpeed * delta;
            if (m_y > player->GetY()) m_y = player->GetY();
        }
        if (m_y > player->GetY())
        {
            m_y -= kCameraSpeed * delta;
            if (m_y < player->GetY()) m_y = player->GetY();
        }
    }
    return nullptr;
}