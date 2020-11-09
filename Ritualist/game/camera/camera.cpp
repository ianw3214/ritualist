#include "camera.hpp"

#include "game/game.hpp"
#include "game/entities/player.hpp"

#include <random>

constexpr float kCameraSpeed = 500.f;

static std::random_device rd;
static std::default_random_engine generator(rd());


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

void CameraService::ScreenShake(float duration, float magnitude)
{
    s_camera->m_currScreenShakeTime = duration;
    s_camera->m_screenShakeTime = duration;
    s_camera->m_screenShakeMagnitude = magnitude;
}

Camera::Camera()
    : m_x(0.f)
    , m_y(0.f)
    , m_realX(0.f)
    , m_realY(0.f)
    , m_scale(1.f)
    , m_screenShakeTime(0.f)
    , m_screenShakeMagnitude(0.f)
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
    // Microseconds to seconds
    float delta = Oasis::WindowService::GetDeltaF() / 1000000.f;

    Ref<Player> player = GameService::GetPlayer();
    if (!m_initialized)
    {
        m_realX = player->GetX();
        m_realY = player->GetY();
        m_x = player->GetX();
        m_y = player->GetY();
        m_initialized = true;
        return nullptr;
    }
    else
    {
        if (m_realX < player->GetX())
        {
            m_realX += kCameraSpeed * delta;
            if (m_realX > player->GetX()) m_realX = player->GetX();
        }
        if (m_realX > player->GetX())
        {
            m_realX -= kCameraSpeed * delta;
            if (m_realX < player->GetX()) m_realX = player->GetX();
        }
        if (m_realY < player->GetY())
        {
            m_realY += kCameraSpeed * delta;
            if (m_realY > player->GetY()) m_realY = player->GetY();
        }
        if (m_realY > player->GetY())
        {
            m_realY -= kCameraSpeed * delta;
            if (m_realY < player->GetY()) m_realY = player->GetY();
        }
    }

    // Update the x/y based on screenshake
    float xOffset = 0.f;
    float yOffset = 0.f;
    if (m_currScreenShakeTime > 0.f)
    {
        std::uniform_real_distribution<float> distribution(-1.0, 1.0);
        float alpha = 1.f - (m_screenShakeTime - m_currScreenShakeTime) / m_screenShakeTime;
        xOffset = distribution(generator) * m_screenShakeMagnitude * alpha;
        yOffset = distribution(generator) * m_screenShakeMagnitude * alpha;
        m_currScreenShakeTime -= delta;
    }
    m_x = m_realX + xOffset;
    m_y = m_realY + yOffset;

    return nullptr;
}