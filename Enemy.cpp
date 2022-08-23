#include "Enemy.h";
/*
Add an spaun in points wehere "s" on the map/
there is 2 types of enemys: one who stays on ground and shooting, 
second - flying everywere and beat the Hero
*/
// Êîíñòðóêòîð
Enemy::Enemy( sf::Image& image, float X, float Y, int W, int H, sf::String Name, sf::String TileMapEnemy[HEIGHT_MAP]) :Entity(image, X, Y, W, H, Name)
{
	speed = 0.1;
	for (int i = 0; i < HEIGHT_MAP; i++)
	{
		TileMap[i] = TileMapEnemy[i];
	}
	isMove = false;
	status = "WTFBOT";
	state = fly;
	sf::Vector2f XY = SpaunTarget();
	x = XY.x;
	y = XY.y;
	if (name == "flybot")
	{
		sprite.setTextureRect(sf::IntRect(12, 5, 57, 68));//w = 45; h = 63
	}
	else if (name == "BOSSbot")
	{
		//w = 160; h = 60
		health = 500;
		BossPart.setTexture(texture);
		BossPart.setTextureRect(sf::IntRect(754, 216, 145, 128));
		sprite.setTextureRect(sf::IntRect(320, 0, 325, 292));
		
		BossPart.setScale(0.5, 0.5);
		sprite.setScale(0.5, 0.5);
		BossPart.setOrigin(64, 72);
		sprite.setOrigin(162, 146);
		BossPart.setPosition(XY.x + W / 2 - 5, XY.y + H / 2 - 10);
	}
	//sprite.setScale(1.2, 1.2);
	//sprite.setColor(sf::Color::Green);
	
	sprite.setPosition(XY.x + W / 2, XY.y + H / 2);
	
}

int Enemy::checkCollisionWithMap(float Dx, float Dy)
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
	dy = sin(rotation) * 0.08;

}
sf::Vector2f Enemy::SpaunTarget()
{
	int countOfSpauns = 0;
	float Sy = 100;
	float Sx = 100;
	for (int i = 0; i < HEIGHT_MAP; i++)//проходимся по элементам карты
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TileMap[i][j] == 's')//если элемент наш тайлик земли, то
			{
				countOfSpauns++;
			}
		}
	if (countOfSpauns != 0)
	{
		int** SpaunsCoordinate = new int* [countOfSpauns];
		for (int i = 0; i < countOfSpauns; ++i)
		{
			SpaunsCoordinate[i] = new int[2];
		}
		int randSpaun = rand() % countOfSpauns;
		for (int i = 0; i < HEIGHT_MAP; i++)//проходимся по элементам карты
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == 's')//если элемент наш тайлик земли, то
				{
					countOfSpauns--;
					SpaunsCoordinate[countOfSpauns][0] = i;
					SpaunsCoordinate[countOfSpauns][1] = j;
				}
			}
		Sy = SpaunsCoordinate[randSpaun][0] * 32;
		Sx = SpaunsCoordinate[randSpaun][1] * 32;
		
	}
	return sf::Vector2f(Sx, Sy);
}
void Enemy::draw(sf::RenderTarget& target)
{
	
	target.draw(sprite);
	if (name == "BOSSbot")
	{
		target.draw(BossPart);
	}
}
//Для определения действия врага
int Enemy::action(float time)
{
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
		x += dx * time * speed;
		checkCollisionWithMap(dx, 0);
		y += dy * time * speed;
		checkCollisionWithMap(0, dy);
		sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
		if (name == "BOSSbot")
		{
			BossPart.setPosition(x + w / 2 - 5, y + h / 2 - 10);
		}
		if (abs(XYAim.x - x) < w && abs(XYAim.y - y) < h)
		{
			isMove = false;
			dx = 0;
			dy = 0;
		}
			
	}
	//else if (name == "BOSSbot")
	//{
	//	//

	//}
	
	return 0;
}

int Enemy::animation()
{
	if (name == "flybot")
	{

		if (status == "anikilled")
		{
			if (moveTimer < 100)
			{
				sprite.setTextureRect(sf::IntRect(240, 0, 80, 80));
			}
			else if (moveTimer < 200)
			{
				sprite.setColor(sf::Color::Red);
			}
			else
			{
				status = "killed";
			}
		
		
		}
		else if (moveTimer < 200)
		{
			sprite.setTextureRect(sf::IntRect(12, 5, 57, 68));
		}
		else if (moveTimer < 400)
		{
			sprite.setTextureRect(sf::IntRect(172, 5, 56, 69));
		}
		else if (moveTimer < 600)
		{
			sprite.setTextureRect(sf::IntRect(92, 5, 58, 72));
		}
		else if (moveTimer < 800)
		{
			sprite.setTextureRect(sf::IntRect(172, 5, 56, 69));
		}
		else
		{
			moveTimer = 0;
		}
	}
	else if (name == "BOSSbot")
	{
		if (status == "anikilled")
		{
			
			if (moveTimer < 200)
			{
				sprite.setTextureRect(sf::IntRect(240, 0, 80, 80));
				sprite.setScale(4,4);
			}
			else if (moveTimer < 300)
			{
				sprite.setColor(sf::Color::Red);
				BossPart.setColor(sf::Color::Red);
			}
			else if (moveTimer < 400)
			{
				sprite.setColor(sf::Color::Yellow);
				BossPart.setColor(sf::Color::Black);
			}
			else if (moveTimer < 500)
			{
				status = "killed";
			}
		}
		else if (moveTimer < 2000)
		{
			sprite.rotate(0.5);
		}
		else if (moveTimer < 4000)
		{
			sprite.rotate(-0.5);
		}
		else
		{
			moveTimer = 0;
		}
		
	}
	return 0;
}


int Enemy::update(float time)
{
	//
	if (!life)
	{
		return 0;
	}
	if (speed < 1)
	{
		speed += 0.001;
	}
	moveTimer += time;
	animation();
	action(time);
	if (status == "killed")
	{
		life = false;
	}
	else if (health <= 0 && status != "anikilled") {
		status = "anikilled";
		state = stay;
		moveTimer = 0;
		if (name == "BOSSbot")
		{
			sprite.setOrigin(40, 50);
			sprite.setRotation(0);
		}
		//sprite.setColor(sf::Color::Red);
	}
	
	return 0;
}

int Enemy::SetAim(sf::Vector2f XY)
{
	
	if (!isMove) {
		if (name == "flybot")
		{
			XYAim = XY;
		}
		else if (name == "BOSSbot")
		{
			XYAim = SpaunTarget();
		}
	
	
	//isMove = true;
	}
	return 0;
}

void Enemy::struck(int damage)
{
	health -= damage;
	
}
