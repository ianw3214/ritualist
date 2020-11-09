#include "levels.hpp"

#include "game.hpp"

#include "entities/altar.hpp"
#include "entities/player.hpp"
#include "entities/altar.hpp"
#include "entities/portal.hpp"
#include "entities/bossportal.hpp"
#include "entities/enemies/boss1.hpp"
#include "entities/enemies/finalboss.hpp"

void LevelLoader::LoadLevel(Level level, Ref<GameLayer> layer)
{
    if (level == Level::LOBBY)
    {
        LoadLevelLobby(layer);
    }

    if (level == Level::BOSS1)
    {
        LoadLevelBoss1(layer);
    }

    if (level == Level::FINALBOSS)
    {
        LoadLevelFinalBoss(layer);
    }
}


void LevelLoader::LoadLevelLobby(Ref<GameLayer> layer)
{
    layer->m_map = new Map("res/maps/lobby.png", "res/lobby.bmp");

    Player * player = new Player(600.f, 1200.f - 470.f);
    layer->m_entities.push_back(player);

    Portal * portal = new Portal(200.f, 750.f);
    portal->SetLayer(-1);
    layer->m_entities.push_back(portal);

    BigPortal * bigportal = new BigPortal(500.f, 820.f);
    bigportal->SetLayer(-1);
    layer->m_entities.push_back(bigportal);
}

void LevelLoader::LoadLevelBoss1(Ref<GameLayer> layer)
{
    layer->m_map = new Map("res/maps/background1.png", "res/test.bmp");

    Player * player = new Player(400.f, 400.f);
    layer->m_entities.push_back(player);
    Boss1 * boss = new Boss1();
    layer->m_entities.push_back(boss);
    Altar * altar = new Altar();
    layer->m_entities.push_back(altar);
    layer->m_boss = boss;
}

void LevelLoader::LoadLevelFinalBoss(Ref<GameLayer> layer)
{
    layer->m_map = new Map("res/maps/finalboss_background.png", "res/finalboss.bmp");
    Player * player = new Player(1000.f, 300.f);
    layer->m_entities.push_back(player);
    FinalBoss * boss = new FinalBoss();
    layer->m_entities.push_back(boss);
    layer->m_boss = boss;
}
