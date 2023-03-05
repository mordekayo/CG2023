#include "SolarSystemGame.h"

#include <ranges>

#include "GameObjects/Planet.h"

SolarSystemGame* SolarSystemGame::Instance()
{
    if (!GameInstance)
    {
        GameInstance = new SolarSystemGame();
    }
    return static_cast<SolarSystemGame*>(GameInstance);
}

void SolarSystemGame::Construct()
{
    Planet* Sun = new Planet("Sun", 0.1f, 1, {0.0f, 1.0f, 0.0f});

    
    // Planet* Mercury = new Planet("Mercury", 5, 100);
    // Planets.insert( {"Mercury", Mercury} );
    //
    // Planet* Venus = new Planet("Venus", 10, 50);
    // Planets.insert( {"Venus", Venus} );
    //
    // Planet* Earth = new Planet("Earth", 20, 30);
    // Planets.insert( {"Earth", Earth} );
    //
    // Planet* Mars = new Planet("Mars", 30, 20);
    // Planets.insert( {"Mars", Mars} );
    //
    // Planet* Jupiter = new Planet("Jupiter", 50, 10);
    // Planets.insert( {"Jupiter", Jupiter} );
    //
    // Planet* Saturn = new Planet("Saturn", 60, 7);
    // Planets.insert( {"Saturn", Saturn} );
    //
    // Planet* Uranus = new Planet("Uranus", 40, 15);
    // Planets.insert( {"Uranus", Uranus} );
    //
    // Planet* Neptune = new Planet("Neptune", 30, 20);
    // Planets.insert( {"Neptune", Neptune} );
    //
    
    //
    // for(const auto Planet : Planets | std::views::values)
    // {
    //    AddGameObject(Planet); 
    // }

    AddGameObject(Sun);
    
    FGame::Construct();
}
