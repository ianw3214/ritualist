#include "oasis.h"

class GameLayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;
};

class Game : public Oasis::GameState
{
public:
    virtual void CreateLayers() override
    {
        // Layers are added bottom -> top
        AddLayer(new GameLayer());
    }
};