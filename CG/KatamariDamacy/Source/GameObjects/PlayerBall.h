#pragma once

#include "GameObjects/GameObject.h"

class PlayerBall : public FGameObject
{
public:
	PlayerBall();
	
	virtual void Construct() override;

	void SetScale(float NewScale);
	float GetScale() const;
	
private:

	float Scale;
};

