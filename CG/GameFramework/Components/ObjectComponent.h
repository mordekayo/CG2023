#pragma once

class FGameObject;

class FObjectComponent
{

public:

    virtual void Init();
    virtual void Update();

    void SetOwner(FGameObject* NewOwner);

protected:

    FGameObject* Owner;
};
