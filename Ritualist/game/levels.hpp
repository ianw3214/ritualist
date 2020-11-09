#pragma once
#include "oasis.h"

enum class Level
{
    LOBBY = 0,
    BOSS1,
    FINALBOSS,
    COUNT
};

class GameLayer;
class LevelLoader
{
public:
    static void LoadLevel(Level level, Ref<GameLayer> layer);
private:
    static void LoadLevelLobby(Ref<GameLayer> layer);
    static void LoadLevelBoss1(Ref<GameLayer> layer);
    static void LoadLevelFinalBoss(Ref<GameLayer> layer);
};