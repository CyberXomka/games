#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "Animation.h"

class Entity // объект
{
public:
	float x, y, dx, dy, R, angle;
	bool life;
	std::string name;
	Animation anim;

	Entity()
	{
		life = 1;
	}

	void settings(Animation &a, float X, float Y, float Angle = 0, float radius = 1)
	{
		x = X;
		y = Y;
		anim = a;
		angle = Angle;
		R = radius;
	}

	virtual void update()
	{}

	void draw(sf::RenderWindow &app)
	{
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);
		sf::CircleShape circle(R);
		circle.setFillColor(sf::Color(255, 0, 0, 170));
		circle.setPosition(x, y);
		circle.setOrigin(R, R);
		//app.draw(circle);
	}

	virtual ~Entity()
	{
	}
}; 
