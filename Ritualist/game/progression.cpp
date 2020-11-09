#include "progression.hpp"

constexpr float defaultPlayerHealth = 20.f;

std::unordered_map<Level, bool> Progression::s_finishedLevels;
float Progression::s_playerHealth = 0.f;
bool Progression::s_gameWon = false;
std::chrono::time_point<std::chrono::system_clock> Progression::s_startTime;
float Progression::s_totalTime = 0.f;

void Progression::ResetProgression()
{
    for (int i = 0; i < static_cast<int>(Level::COUNT); ++i)
    {
        s_finishedLevels[static_cast<Level>(i)] = false;
    }
    // s_finishedLevels[Level::BOSS1] = true;
    s_playerHealth = defaultPlayerHealth;
    s_gameWon = false;
    s_startTime = std::chrono::system_clock::now();
}

bool Progression::CheckLevelFinished(Level level)
{
    return s_finishedLevels[level];
}

float Progression::GetPlayerHealth()
{
    return s_playerHealth;
}

float Progression::GetPlayerHealthCapacity()
{
    return defaultPlayerHealth;
}

void Progression::SetLevelFinished(Level level)
{
    s_finishedLevels[level] = true;
}

void Progression::SetPlayerHealth(float health)
{
    s_playerHealth = health;
}

bool Progression::GameWon()
{
    return s_gameWon;
}

void Progression::SetGameWon()
{
    s_gameWon = true;
    s_totalTime = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - s_startTime).count());
}

float Progression::GetTotalTime()
{
    return s_totalTime;
}