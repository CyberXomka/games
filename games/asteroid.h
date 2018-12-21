#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "Entity.h"
#include "Screen_size.h"

class asteroid : public Entity
{
public:
	asteroid();
	void update();
};