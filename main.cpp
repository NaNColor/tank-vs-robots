#include <ctime>
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
	srand(unsigned(time(0)));
	int way=2;
	while ( way !=0)
	{
		Engine* level = new Engine();
		way = level->play(way);
		delete level;
	}
	
	return 0;
}