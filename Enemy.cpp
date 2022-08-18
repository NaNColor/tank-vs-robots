#include "Enemy.h";
/*
Add an spaun in points wehere "s" on the map/
there is 2 types of enemys: one who stays on ground and shooting, 
second - flying everywere and beat the Hero
*/
// Êîíñòðóêòîð
Enemy::Enemy( sf::Image& image, float X, float Y, int W, int H, sf::String Name, sf::String TileMap[HEIGHT_MAP]) :Entity(image, X, Y, W, H, Name)
{

	isMove = false;
	status = "WTFBOT";
	state = fly;
	sprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
	//sprite.setScale(1.2, 1.2);
	//sprite.setColor(sf::Color::Green);
	
	int countOfSpauns=0;
	for (int i = 0; i < HEIGHT_MAP; i++)//проходимся по элементам карты
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TileMap[i][j] == 's' )//если элемент наш тайлик земли, то
			{
				countOfSpauns++;
			}
		}
	if (countOfSpauns != 0)
	{
		int **SpaunsCoordinate = new int* [countOfSpauns]; 
		for (int i = 0; i < countOfSpauns; ++i)
		{
			SpaunsCoordinate[i] = new int [2];
		}
		int randSpaun = rand() % countOfSpauns;
		for (int i = 0; i < HEIGHT_MAP; i++)//проходимся по элементам карты
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == 's' )//если элемент наш тайлик земли, то
				{
					countOfSpauns--;
					SpaunsCoordinate[countOfSpauns][0] = i;
					SpaunsCoordinate[countOfSpauns][1] = j;
				}
			}
		y = SpaunsCoordinate[randSpaun][0] * 32;
		x = SpaunsCoordinate[randSpaun][1] * 32;
	}
	XYAim = sf::Vector2f(x, y);
	sprite.setPosition(x + W / 2, y + H / 2);
}

int Enemy::checkCollisionWithMap(float Dx, float Dy, sf::String TileMap[HEIGHT_MAP])
{
	for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
		for (int j = x / 32; j < (x + w) / 32; j++)
		{
			if (TileMap[i][j] == '0' )//если элемент наш тайлик земли, то
			{
			
				//float rotation = -(atan2(y - XYAim.y, x - XYAim.x)) * 180 / 3.14159265; // rad -> grad
				
				isMove = false;
				SetDirection();
				//std::cout << dx << " : " << dy << " rot: " << rotation << std::endl;
			}
		}
	return 0;
}
void Enemy::SetDirection()
{
	float rotation = (atan2(XYAim.y - y, XYAim.x - x)); // rad -> grad?
	dx = cos(rotation) * 0.1;// тут возможна ошибка направления движения, пока как фича
	dy = sin(rotation) * 0.085;

}
//Для определения действия врага
int Enemy::action(float time, sf::String TileMap[HEIGHT_MAP])
{
	if (name == "flybot")
	{
		//
		if (!isMove)
		{
			isMove = true;
			SetDirection();
			
			//std::cout << dx << " : " << dy  << " rot: " << rotation << std::endl;

		}
		else
		{
			if (state == stay)
			{
				return 0;
			}
			x += dx * time;
			checkCollisionWithMap(dx, 0, TileMap);
			y += dy * time;
			checkCollisionWithMap(0, dy, TileMap);
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			
			if (abs(XYAim.x - x) < 20 && abs(XYAim.y - y) < 20)
			{
				isMove = false;
				dx = 0;
				dy = 0;
			}
			
		}
	}
	
	return 0;
}

int Enemy::animation()
{
	if (status == "AlahuuuAkbar" || status == "killed")
	{
		sprite.setTextureRect(sf::IntRect(240, 0, 80, 80));
		if (status == "killed")
		{
			sprite.setColor(sf::Color::Red);
		}
	}
	else if (moveTimer < 200)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
	}
	else if (moveTimer < 400)
	{
		sprite.setTextureRect(sf::IntRect(160, 0, 80, 80));
	}
	else if (moveTimer < 600)
	{
		sprite.setTextureRect(sf::IntRect(80, 0, 80, 80));
	}
	else if (moveTimer < 800)
	{
		sprite.setTextureRect(sf::IntRect(160, 0, 80, 80));
	}
	else
	{
		moveTimer = 0;
	}

	return 0;
}


int Enemy::update(float time, sf::String TileMap[HEIGHT_MAP])
{
	//
	if (!life)
	{
		return 0;
	}
	moveTimer += time;
	animation();
	action(time, TileMap);
	
	if (status == "killed")
	{
		life = false;
	}
	else if (health <= 0) { 
		status = "killed";
		state = stay;
		//sprite.setColor(sf::Color::Red);
	}
	
	return 0;
}

int Enemy::SetAim(sf::Vector2f XY)
{
	
	if (!isMove) {
	XYAim = XY;
	
	//isMove = true;
	}
	return 0;
}

void Enemy::struck(int damage)
{
	health -= damage;
	
}
