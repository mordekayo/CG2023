#pragma once

class GameObject
{
	public:

	GameObject() = default;
	
	virtual void Init();
	virtual void Update();
	virtual void Draw();
};

