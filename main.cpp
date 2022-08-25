#include <ctime>
#include "interaction.h"

/* 
Здесь вход в программу.
используем для вектора тип sf::Vector2f
*/

int main()
{
	srand(unsigned(time(0)));
	int way=2;//для определения действия при завершении игры
	while ( way !=0)
	{
		Engine* level = new Engine();//Создали объект = запусьтли игру
		way = level->play(way);
		delete level;// Удалили объект (игру)
	}
	
	return 0;
}