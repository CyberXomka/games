#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <iostream>
#include <sstream>

#include "menu.h"//меню
#include "Animation.h"//Анимация
#include "Entity.h"//Объект
#include "asteroid.h"//Астероид
#include "bullet.h"//Пуля
#include "Player.h"//Герой
#include "Screen_size.h"//Размер экрана

using namespace sf;//включаем пространство имен sf, чтобы постоянно не писать sf::

bool isCollide(Entity *a, Entity *b) // столкновение 
{
	return (b->x - a->x)*(b->x - a->x) +
		   (b->y - a->y)*(b->y - a->y) <
		   (a->R + b->R)*(a->R + b->R);
}

bool StartGame()
{
	RenderWindow app(VideoMode(W, H), "world of meteorite");//размер и название экрана
	menu(app);
	app.setFramerateLimit(60);
	///
	Clock clock;
	Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры 
	int gameTime = 0;//объявили игровое время, инициализировали.
	///
	Font font;//шрифт 
	font.loadFromFile("images/cyrillicold.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 30);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setStyle(sf::Text::Bold);//жирный 

	Texture t1, t2, t3, t4, t5, t6, t7, t8;//создаем объект Texture
	t1.loadFromFile("images/mod.png");// игрок
	t2.loadFromFile("images/space.jpg");// фон
	t3.loadFromFile("images/type_C.png");// взрыв
	t4.loadFromFile("images/meteor.png");// метеорит
	t5.loadFromFile("images/fire_red.png");// пули
	t6.loadFromFile("images/meteor_min.png");// метеорил мини
	t7.loadFromFile("images/type_B.png");// взрыв
	t8.loadFromFile("images/GO.png");

	t1.setSmooth(true);
	t2.setSmooth(true);
	//t8.setSmooth(true);
	

	Sprite GameOver(t8);
	Sprite backgroud(t2);
	Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
	Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
	Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
	Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
	Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
	Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
	Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
	GameOver.setPosition(600, 400);
	std::list<Entity*> entities;//object

	for (int i = 0; i < 10; i++)
	{
		asteroid *a = new asteroid();
		a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
		entities.push_back(a);
	}

	player *p = new player();
	p->settings(sPlayer, 1000, 500, 0, 20);
	entities.push_back(p);

	/////основной цикл/////
	while (app.isOpen())
	{
		app.draw(backgroud);
		Event event;

		auto time = clock.getElapsedTime().asMicroseconds();
		if (p->life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в секундах идёт вперед, пока жив игрок
		clock.restart();
		time = time / 800;

		if (!p->life)
		{
			app.draw(GameOver);
		}

		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)
				app.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Space)
				{
					bullet *b = new bullet();
					b->settings(sBullet, p->x, p->y, p->angle, 10);
					entities.push_back(b);
				}
		}
		if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//если таб, то перезагружаем игру
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//если эскейп, то выходим из игры
		if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 4;// поворот на право
		if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle -= 4;// поворот на лево
		if (Keyboard::isKeyPressed(Keyboard::Up)) p->thrust = true;// ускорение
		else p->thrust = false;

		for (auto a : entities)
			for (auto b : entities)
			{
				if (a->name == "asteroid" && b->name == "bullet")
					if (isCollide(a, b))
					{
						a->life = false;
						b->life = false;

						Entity *e = new Entity();
						e->settings(sExplosion, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);

						for (int i = 0; i < rand() % 4 + 0; i++)
						{
							if (a->R == 15) continue;
							Entity *e = new asteroid();
							e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
							entities.push_back(e);
						}
					}
				if (a->name == "player" && b->name == "asteroid")
					if (isCollide(a, b))
					{
						b->life = false;
						a->life = false;
						Entity *e = new Entity();
						e->settings(sExplosion_ship, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);
					}
			}

		if (p->thrust)  p->anim = sPlayer_go; //анимация при движение 
		else   p->anim = sPlayer;

		for (auto e : entities)
			if (e->name == "explosion")
				if (e->anim.isEnd()) e->life = 0;

		if (rand() % 150 == 0)// появления новых метеоритов
		{
			asteroid *a = new asteroid();
			a->settings(sRock, 0, rand() % H, rand() % 360, 25);
			entities.push_back(a);
		}

		for (auto i = entities.begin(); i != entities.end();)
		{
			Entity *e = *i;
			e->update();
			e->anim.update();

			if (e->life == false)
			{
				i = entities.erase(i);
				delete e;
			}
			else i++;
		}

		std::ostringstream gameTimeString;    // объявили переменную времени
		gameTimeString << gameTime;		//формируем строку
		for (auto i : entities)
			i->draw(app);
		text.setString("Время:" + gameTimeString.str());//задает строку тексту
		text.setPosition(50, 30);//задаем позицию текста, центр камеры
		app.draw(text);//рисую этот текст
		app.display();// вывод на экран
	}
}

void gameRunning() {//ф-ция перезагружает игру , если это необходимо
	if (StartGame()) { gameRunning(); }////если startGame() == true, то вызываем занова ф-цию GameRunning, которая в свою очередь опять вызывает startGame() 
}

int main()
{
	gameRunning();//запускаем процесс игры
	return 0;
	
}