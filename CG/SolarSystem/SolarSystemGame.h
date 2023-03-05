#pragma once
#include <map>

#include "../GameFramework/Game.h"

class Planet;

class SolarSystemGame : public FGame
{
public:
    static SolarSystemGame* Instance();

protected:

    void Construct() override;

    void Update(float DeltaTime) override;

private:

    bool WasProjectionKeyDown = false;
    std::map<std::string, Planet*> Planets;
};
