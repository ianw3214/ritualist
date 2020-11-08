#include "oasis.h"

#include <vector>

#include "attackManager.hpp"
#include "map.hpp"
#include "levels.hpp"

const std::string kAltarUiName = "ALTAR BACKGROUND";
const std::string kLobbyButton = "SACRIFICE";

class Entity;
class Player;
class Game;

////////////////////////////////////////////////////////////////
class GameService
{
    friend class GameLayer;
    static Ref<GameLayer> s_game;

    friend class Player;
    static void SetPlayer(Ref<Player> player);
public:
    static Ref<Player> GetPlayer();
    static Ref<Entity> GetBoss();
    static bool ActivateAltar();
    static bool CheckMapConstriants(float x, float y);

    static void AddAttackCallback(attack_func f);
    static void AddAttack(Target target, float damage, float x, float y, float w, float h);

    static void ChangeLevel(Level level);
    static Level GetLevel();

    // Buttons
    static void ResetButtons();
    static bool SacrificeButton();
private:
    static Level s_level;

    // This global init will only be called once
    static bool s_gameInit;
    static void GlobalInit();

    // Buttons
    static bool s_sacrificeButton;
};

////////////////////////////////////////////////////////////////
class GameLayer : public Oasis::GameStateLayer
{
private:
    friend class GameService;
    friend class LevelLoader;
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual Oasis::IState * Update()  override;

    bool CheckMapConstraint(float x, float y);
private:
    std::vector<Entity *> m_entities;
    Ref<Player> m_player;
    Ref<Entity> m_boss;
    
    AttackManager m_attackManager;

    // Just render things in gamelayer
    Oasis::Sprite * m_playerShadow;

    Map * m_map;

    bool m_changeLevel;
};

#include "game/camera/camera.hpp"
#include "game/input/input.hpp"
#include "gameui/gameui.hpp"

////////////////////////////////////////////////////////////////
class Game : public Oasis::GameState
{
public:
    virtual void CreateLayers() override
    {
        // Layers are added bottom -> top
        AddLayer(new Camera());
        AddLayer(new GameLayer());
        AddLayer(new Input());
        AddLayer(new GameUI());
    }
};