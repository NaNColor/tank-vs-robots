#ifndef __Entity__
#define __Entity__

#include <SFML/Graphics.hpp>
/*
Основной класс. Наследуется к остальным объектам.
Поля:
- Текстура, Спрайт
- Строка отличия для схожих объектов, пусть то вид героя, врага или пули
- Булевые переменные живой - нет, двигается - нет
- целые числа жизни и две переменные под размер спрайта 
- дробные числа под скорость/перемещение + таймер
*без файла cpp*
*/
class Entity {
public:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::FloatRect GetRect() {return sf::FloatRect(x,y,w,h);};
	Entity( sf::Image &image, float X, float Y,int W,int H, sf::String Name){
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2); // В центр спрайта ставим координаты
	}
	bool isAlive() { return life; }; // Возвращает позицию спрайта
protected:
	float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
	int w, h, health;
	bool life, isMove;
	sf::String name;//различим врагов по имени
	enum { left, right, up, down, stay, fly } state;//добавляем тип перечисления - состояние объекта
};

#endif