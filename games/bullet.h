#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "Entity.h"
#include "Screen_size.h"

class bullet : public Entity
{
public:
	const float DEGTORAD = 0.017453f;
	bullet();
	void update();
};