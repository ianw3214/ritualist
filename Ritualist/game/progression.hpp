#pragma once
#include "oasis.h"

#include <unordered_map>
#include <chrono>

#include "levels.hpp"

class Progression
{
public:
    static void ResetProgression();
    static bool CheckLevelFinished(Level level);
    static float GetPlayerHealth();
    static float GetPlayerHealthCapacity();

    static void SetLevelFinished(Level level);
    static void SetPlayerHealth(float health);

    static bool GameWon();
    static void SetGameWon();
    static float GetTotalTime();
private:
    static std::unordered_map<Level, bool> s_finishedLevels;  
    static float s_playerHealth;
    static bool s_gameWon;

    static std::chrono::time_point<std::chrono::system_clock> s_startTime;
    static float s_totalTime;
};