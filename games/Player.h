#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "Entity.h"
#include "Screen_size.h"


class player : public Entity
{
public:
	const float DEGTORAD = 0.017453f;
	bool thrust;
	player();
	void update();
};




/*class player : public Entity
{
public:
	const float DEGTORAD = 0.017453f;
	bool thrust;

	player()
	{
		name = "player";
	}

	void update()
	{
		if (thrust)
		{
			dx += cos(angle*DEGTORAD)*float(0.2);
			dy += sin(angle*DEGTORAD)*float(0.2);
		}
		else
		{
			dx *= 0.99;
			dy *= 0.99;
		}

		int maxSpeed = 15;
		float speed = sqrt(dx*dx + dy * dy);
		if (speed > maxSpeed)
		{
			dx *= maxSpeed / speed;
			dy *= maxSpeed / speed;
		}

		x += dx;
		y += dy;

		if (x > W) x = 0; if (x < 0) x = W;
		if (y > H) y = 0; if (y < 0) y = H;
	}

};*/