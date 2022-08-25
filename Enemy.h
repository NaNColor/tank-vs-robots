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

class Enemy :public Entity {
public:
	//передаем так же строку карты, чтоб все время не создавалась память при вызовах функций
	Enemy( sf::Image& image, float X, float Y, int W, int H, sf::String Name, sf::String TileMapEnemy[HEIGHT_MAP]);
	int update(float time);// Жизнь объекта, ф-я вызывается в основной программе
	int SetAim(sf::Vector2f);// Устанавливает положение движения
	sf::String GetStatus() { return status; };
	void struck(int damage);// Палундра!! На нас напали! - вычитаем HP
	sf::Vector2f GetXY() { return sf::Vector2f(x, y); }; // Возвращает позицию спрайта Enemy
	void SetStatus(sf::String St) { status = St; };
	void draw(sf::RenderTarget& target);// Так как нужно много за раз нарисовать, то вынесем в отдельный метод
	sf::FloatRect GetRect();// Переопределили ф-ю, чтобы подвинуть хитбокс
	sf::String GetName() { return name; };
	void recetBOSSdamagetimer() { BOSSdamagetimer = 0; };// для сброса таймера атаки босса
	float GetBOSSdamagetimer() { return BOSSdamagetimer; };// вернули таймер
private:
	// Будет вызываться внутри, так что инкапсулирую ф-ии
	float BOSSdamagetimer;
	sf::Sprite BossPart;//босс состоит из 2х частей: голова и пилы
	sf::Sprite healthSprite;//Спрайт для полоски HP
	int action(float);// действие оьъекта
	int animation();// no comments
	int checkCollisionWithMap(float, float);//ф ция проверки столкновений с картой (границами)
	sf::Vector2f XYAim;// координата спрайта игрока
	sf::String status;
	void SetDirection();// Вычисление направления по координатам взятых с SetAim
	sf::Vector2f SpaunTarget();// смотрим где же науши спауны на карте
	sf::String TileMap[HEIGHT_MAP];// карта
};