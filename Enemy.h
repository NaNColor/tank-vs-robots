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
	Enemy( sf::Image& image, float X, float Y, int W, int H, sf::String Name, sf::String TileMapEnemy[HEIGHT_MAP]);
	int update(float time);// Жизнь объекта, ф-я вызывается в основной программе
	int SetAim(sf::Vector2f);//
	sf::String GetStatus() { return status; };
	void struck(int damage);
	sf::Vector2f GetXY() { return sf::Vector2f(x, y); }; // Возвращает позицию спрайта Enemy
	void SetStatus(sf::String St) { status = St; };
	void draw(sf::RenderTarget& target);
private:
	// Будет вызываться внутри, так что инкапсулирую ф-ии
	sf::Sprite BossPart;
	int action(float);
	int animation();
	int checkCollisionWithMap(float, float);//ф ция проверки столкновений с картой (границами)
	sf::Vector2f XYAim;// координата спрайта игрока
	sf::String status;
	void SetDirection();
	sf::Vector2f SpaunTarget();
	sf::String TileMap[HEIGHT_MAP];
};