#include "Player.h"
#include "Constants.h"
#include <iostream>
// Конструктор
Player::Player(sf::Image &image, float X, float Y,int W,int H, sf::String Name):Entity(image,X,Y,W,H,Name)
{
    state = stay;
	frame.setTexture(texture);
	wheelL.setTexture(texture);
	wheelR.setTexture(texture);
	gun.setTexture(texture);

	wheelL.setScale(0.9, 1);
	wheelR.setScale(0.9, 1);
	frame.setScale(0.9, 0.9);
	gun.setScale(0.9, 0.9);

	wheelL.setTextureRect(sf::IntRect(1, 80, 19, 77));
	wheelL.setOrigin(w / 2 + 10, h / 2);

	wheelR.setTextureRect(sf::IntRect(20, 81, 19, 77));
	wheelR.setOrigin(- w / 2 +10, h / 2);

	frame.setTextureRect(sf::IntRect(84,80,70,80));
	frame.setOrigin(w / 2, h / 2);
	//texture.setSmooth(true);
	//sprite.setColor(sf::Color::Green);
	gun.setTextureRect(sf::IntRect(181, 81, 36, 78));
	gun.setOrigin(18, 58);

	frame.setPosition(x, y);
	wheelL.setPosition(x, y);
	wheelR.setPosition(x, y);
	gun.setPosition(x, y);
}

 //Для определения движения (пока скомунизжено)
int Player::control(sf::Event event)
{
	state = stay;
	if (sf::Keyboard::isKeyPressed) {//если нажата клавиша
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {//а именно левая
			state = left; speed = 0.1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			state = right; speed = 0.1;
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up))) {//если нажата клавиша вверх
			state = up; speed = 0.085;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			state = down; speed = 0.085;
		}
	}
	return 0;
}

int Player::checkCollisionWithMap(float Dx, float Dy, sf::String TileMap[HEIGHT_MAP])
{
	for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
		for (int j = x / 32; j < (x + w) / 32; j++)
		{
			if (TileMap[i][j] == '0' || TileMap[i][j] == 'b')//если элемент наш тайлик земли, то
			{
				if (Dy > 0) { y = i * 32 - h;  dy = 0; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
				if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//столкновение с верхними краями карты(может и не пригодиться)
				if (Dx > 0) { x = j * 32 - w; }//с правым краем карты
				if (Dx < 0) { x = j * 32 + 32; }// с левым краем карты
			}
		}
	return 0;
}
void Player::animation()
{
	//
	switch (state)
	{
	case Entity::left:
		frame.setRotation(-90);
		wheelL.setRotation(-90);
		wheelR.setRotation(-90);
		gun.setRotation(-90);
		break;
	case Entity::right:
		frame.setRotation(90);
		wheelL.setRotation(90);
		wheelR.setRotation(90);
		gun.setRotation(90);
		break;
	case Entity::up:
		frame.setRotation(0);
		wheelL.setRotation(0);
		wheelR.setRotation(0);
		gun.setRotation(0);
		break;
	case Entity::down:
		frame.setRotation(180);
		wheelL.setRotation(180);
		wheelR.setRotation(180);
		gun.setRotation(180);
		break;
	default:
		break;
	}
}
int Player::update(float time, sf::String TileMap[HEIGHT_MAP], sf::Event event)
{
	//
	if (!life)
	{
		return 0;
	}
	control(event);
	animation();
	switch (state)//тут делаются различные действия в зависимости от состояния
		{
		case right: dx = speed; dy = 0; break;//состояние идти вправо
		case left: dx = -speed; dy = 0; break;//состояние идти влево
		case up: dx = 0; dy = -speed; break;//будет состояние поднятия наверх (например по лестнице)
		case down: dx = 0; dy = speed; break;//будет состояние во время спуска персонажа (например по лестнице)
		case stay: dx = 0; dy = 0; break;//и здесь тоже		
		}
	x += dx*time; 
	checkCollisionWithMap(dx, 0, TileMap);//обрабатываем столкновение по Х
	y += dy*time; 
	checkCollisionWithMap(0, dy, TileMap);//обрабатываем столкновение по Y
	frame.setPosition(x+w/2, y+h/2); //задаем позицию спрайта в место его центра
	//frame.setPosition(x + w / 2, y + h / 2);
	wheelL.setPosition(x + w / 2, y + h / 2);
	wheelR.setPosition(x + w / 2, y + h / 2);
	gun.setPosition(x + w / 2, y + h / 2);
	if (health <= 0){life = false;}
	//if (!isMove){ speed = 0; }
	speed = 0;
	return 0;
}

void Player::struck(int damage)
{
	health -= damage;
	std::cout << "HIt";
	//sprite.setColor(sf::Color::Red);
}
void Player::draw(sf::RenderTarget& target)
{
	//
	target.draw(wheelL);
	target.draw(wheelR);
	target.draw(frame);
	target.draw(gun);
}