#ifndef __Player_h__
#define __Player_h__

//#include "View.h"
#include "Entity.h"
#include "Constants.h"
/*
Класс игрок. Главный герой!!!
Может возвращать свои координаты.
Должен сталкиваться с препятсвиями и врашами.

Переменные от родителя:
float dx, dy, x, y, speed, moveTimer;
int w, h, health;
bool life, isMove;
sf::Texture texture;
sf::Sprite sprite;
sf::String name;
*/
//sf::View* view = new sf::View;
//void setPlayerCoordinateForView(float x, float y) {
//	view->setCenter(x + 100, y);
//}

class Player :protected Entity {
public:
	Player(sf::Image& image, float X, float Y, int W, int H, sf::String Name);
	int update(float time, sf::String TileMap[HEIGHT_MAP], sf::Event event);// Жизнь объекта, ф-я вызывается в основной программе
	sf::Vector2f GetXY() { return sf::Vector2f(x, y); }; // Возвращает позицию спрайта героя
	void struck(int damage);
	bool isAlive() { return life; };
	void draw(sf::RenderTarget& target);
private:
	// Будет вызываться внутри, так что инкапсулирую ф-ии
	int control(sf::Event event);
	int checkCollisionWithMap(float, float, sf::String TileMap[HEIGHT_MAP]);//ф ция проверки столкновений с картой
	void animation();
	sf::Sprite wheelR;
	sf::Sprite wheelL;
	sf::Sprite gun;
	sf::Sprite frame;
};

#endif