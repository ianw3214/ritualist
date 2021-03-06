#pragma once
#include "oasis.h"

class Player;

////////////////////////////////////////////////////////////////////////////////////////
class Camera : public Oasis::GameStateLayer
{
    friend class CameraService;
public:
    Camera();

    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual Oasis::IState * Update()  override;
private:
    // Center position of the camera
    float m_x;
    float m_y;
    float m_realX;
    float m_realY;
    // How zoomed in/out the user is with the camera
    float m_scale;

    float m_currScreenShakeTime;
    float m_screenShakeTime;
    float m_screenShakeMagnitude;

    bool m_initialized;
};

////////////////////////////////////////////////////////////////////////////////////////
class CameraService
{
    friend class Camera;
    static Ref<Camera> s_camera;
public:
    static float ScreenToRawX(int x);
    static float ScreenToRawY(int y);
    static float RawToScreenX(float x, float parallax = 1.f);
    static float RawToScreenY(float y, float parallax = 1.f);

    static void ScreenShake(float duration, float magnitude);

    inline static float GetX() { return s_camera->m_x; }
    inline static float GetY() { return s_camera->m_y; }
    inline static float GetScale() { return s_camera->m_scale; }
};