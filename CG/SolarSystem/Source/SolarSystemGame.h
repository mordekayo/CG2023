#pragma once
#include <map>

#include "Game.h"

class Planet;

class SolarSystemGame : public FGame
{
public:
    static SolarSystemGame* Instance();

protected:

    void Construct() override;

    void Update(float DeltaTime) override;

private:
    Planet* Sun;
    Planet* Earth;
    Planet* Moon;
    Planet* Venus;
    Planet* Venus2;
    
    std::map<std::string, Planet*> Planets;
};
