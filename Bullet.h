#pragma once
#include "Entity.h";
#include "Constants.h"
class Bullet : public Entity
{
public:
	Bullet(sf::Image& image, float X, float Y, int W, int H, float rotation,  sf::String Name, int);
	//~Bullet();
	void update(float time, sf::String TileMap[HEIGHT_MAP]);
	int GetDamage();
private:
	int damage;
	float TTL;
	void animation();
	void checkCollisionWithMap(float Dx, float Dy, sf::String TileMap[HEIGHT_MAP]);
};
