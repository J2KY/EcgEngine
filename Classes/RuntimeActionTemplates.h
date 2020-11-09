#pragma once
#include "GameObject.h"

class RuntimeActionTemplates
{
public:
	static void MoveX(GameObject* target, float delta);
	static void FlipSpriteXTrue(GameObject* target);
	static void FlipSpriteXFalse(GameObject* target);
	static void FlipSpriteX(GameObject* target);
};

