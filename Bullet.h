#pragma once
#include "Entity.h";
#include "Constants.h"
class Bullet : public Entity
{
public:
	Bullet(sf::Image& image, float X, float Y, int W, int H, float rotation,  sf::String Name, int);// 1
	void update(float time, sf::String TileMap[HEIGHT_MAP]); //1
	int GetDamage();//2
	sf::String GetName() { return name; };//3
	sf::FloatRect GetRect();//1
private:
	int damage;//2
	float rotationPer;// Просто поворот пули   //1
	float TTL;//3
	void animation();//3
	void checkCollisionWithMap(float Dx, float Dy, sf::String TileMap[HEIGHT_MAP]);//2
};
