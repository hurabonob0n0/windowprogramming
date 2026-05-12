#pragma once
#include <atlimage.h>
#include "Component.h"

class Sprite : public Component { // Initialize method - 
public:
	Sprite(GameObject* Owner) : Component(Owner) {}
	virtual~Sprite() {};

private:
	CImage img;
};