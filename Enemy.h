#pragma once
#include "Entity.h";
#include "Constants.h"
//Переменные от родителя :
//float dx, dy, x, y, speed, moveTimer;
//int w, h, health;
//bool life, isMove;
//sf::Texture texture;
//sf::Sprite sprite;
//sf::String name;
//*/


class Enemy :public Entity {
public:
	Enemy( sf::Image& image, float X, float Y, int W, int H, sf::String Name, sf::String TileMap[HEIGHT_MAP]);
	int update(float time, sf::String TileMap[HEIGHT_MAP]);// Жизнь объекта, ф-я вызывается в основной программе
	bool isAlive() { return life; }; // Возвращает позицию спрайта
	int SetAim(sf::Vector2f);//
	sf::String GetStatus() { return status; };
	void struck(int damage);
	sf::Vector2f GetXY() { return sf::Vector2f(x, y); }; // Возвращает позицию спрайта Enemy
	void SetStatus(sf::String St) { status = St; };
private:
	// Будет вызываться внутри, так что инкапсулирую ф-ии
	int action(float, sf::String TileMap[HEIGHT_MAP]);
	int animation();
	int checkCollisionWithMap(float, float, sf::String TileMap[HEIGHT_MAP]);//ф ция проверки столкновений с картой (границами)
	sf::Vector2f XYAim;// координата спрайта игрока
	sf::String status;
	void SetDirection();
};