#include <iostream>
#include <vector>
#include "interaction.h"

/* 
Здесь будет вход в программу.
необходимо:
-Окно
-таймер
-сделать карту
-прописать модельки (отдельный файл для картинок)
-не забывать про деструкторы

используем для вектора тип sf::Vector2f
*/




int main()
{
	Engine* level = new Engine();
	//sf::Event event;
	level->start();
	delete level;
	return 0;
}