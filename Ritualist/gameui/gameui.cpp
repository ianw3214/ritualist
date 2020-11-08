#include "gameui.hpp"

#include "game.hpp"
#include "entities/player.hpp"

void GameUI::Init() 
{

}

void GameUI::Close() 
{

}

bool GameUI::HandleEvent(const Oasis::Event& event) 
{
    return false;
}

void GameUI::Update()  
{
    {   // Render the player health bar
        Ref<Player> player = GameService::GetPlayer();
        float percent = player->GetHealth() / player->GetHealthCapacity();
        constexpr float x = 10.f;
        constexpr float y = 10.f;
        constexpr float w = 150.f;
        constexpr float h = 50.f;
        Oasis::Renderer::DrawQuad(x, y, w * percent, h, Oasis::Colours::RED);
    }

    {   // Render the boss health bar
        Ref<Entity> boss = GameService::GetBoss();
        float percent = boss->GetHealth() / boss->GetHealthCapacity();
        const float x = Oasis::WindowService::WindowWidth() / 2 - 350.f;
        const float y = Oasis::WindowService::WindowHeight() - 35.f;
        constexpr float w = 600.f;
        constexpr float h = 15.f;
        Oasis::Renderer::DrawQuad(x, y, w * percent, h, Oasis::Colours::RED);   
    }
}
