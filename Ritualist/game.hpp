#include "oasis.h"

#include "attackManager.hpp"

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

    static void AddAttackCallback(attack_func f);
    static void AddAttack(Target target, float damage, float x, float y, float w, float h);
};

////////////////////////////////////////////////////////////////
class GameLayer : public Oasis::GameStateLayer
{
private:
    friend class GameService;
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;
private:
    std::vector<Entity *> m_entities;
    Ref<Player> m_player;
    Ref<Entity> m_boss;
    

    AttackManager m_attackManager;

    Oasis::Sprite * m_map;
};

#include "camera/camera.hpp"
#include "input/input.hpp"
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