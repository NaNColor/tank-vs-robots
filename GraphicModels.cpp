/*#include "GraphicModels.h"


Здесь будет конструктор, открывающий картинку.
Должны быть поля на каждую картинку.
Необходимо для чектого и простого изображения.
Поля для:
-врагов
-героя и его оружия
-пол
-стены
-декорации
-эффектов
каждый элемент размером 80х80. Возможны исключения.


GraphicModels::GraphicModels()
{
	maintexture.loadFromFile("robots.png");
	enemysprite1.setTexture(maintexture);
	enemysprite1.setTextureRect(sf::IntRect(0, 0, 80, 80));
	enemysprite2.setTexture(maintexture);
	enemysprite2.setTextureRect(sf::IntRect(80, 80, 80, 80));

	//an creator
}
GraphicModels::~GraphicModels()
{
	//an destructor
}
sf::Sprite GraphicModels::GetEnemySprite(int number)
{
	if (number == 1)
	{
		return enemysprite1;
	}
	else 
	{
		return enemysprite2;
	}
}
void GraphicModels::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    
}*/