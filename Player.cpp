#include "Player.h"
#include "Constants.h"
//#include <iostream>
// Конструктор
Player::Player(sf::Image &image, float X, float Y,int W,int H, sf::String Name):Entity(image,X,Y,W,H,Name)
{
	score = 0;
	struckTimer = 0;
    state = up;
	frame.setTexture(texture);//10-13 установили текстуры для спрайтов
	wheelL.setTexture(texture);// герой состоит из 4х спрайтов
	wheelR.setTexture(texture);// рама, правое/левое колеса, пушка
	gun.setTexture(texture);

	wheelL.setScale(0.9, 1);//подгоняем размеры картинок (далее "спрайтов")
	wheelR.setScale(0.9, 1);// чтобы отображался герой как единое целое
	frame.setScale(0.9, 0.9);
	gun.setScale(0.9, 0.9);

	wheelL.setTextureRect(sf::IntRect(1, 80, 19, 77)); // вырезали из текстуры то что надо
	wheelL.setOrigin(w / 2 + 10, h / 2);//оцентравали левое колесо

	wheelR.setTextureRect(sf::IntRect(20, 81, 19, 77));
	wheelR.setOrigin(- w / 2 +10, h / 2);//оцентравали дургое колесо

	frame.setTextureRect(sf::IntRect(84,80,70,80));
	frame.setOrigin(w / 2, h / 2);//оцентравали раму
	
	gun.setTextureRect(sf::IntRect(181, 81, 36, 78));
	gun.setOrigin(18, 58);//оцентравали 
	//чтобы вращение происходило относительно центра рамы
	frame.setPosition(x, y);//Установили начальную позицию на экране
	wheelL.setPosition(x, y);//для всех спрайтов
	wheelR.setPosition(x, y);//у них после оцентровки единая точка 
	gun.setPosition(x, y);//для последующего перемещения
	gunrotation = 0;//инициализировали поворот пушки
}

 //Для определения движения
int Player::control(sf::Event event)
{
	state = stay;//положение игрока - "стоит"
	sf::Vector2i DXY = sf::Mouse::getPosition();// взяли позицию курсора
	gunrotation = atan2(-y + DXY.y - 40, -x + DXY.x - 35) * 180 / 3.14159265 + 90;
	//задали угол поворота пушки отн-но положения курсора и сдвинули точку отчета на середину рамы
	if (sf::Keyboard::isKeyPressed) {//если нажата клавиша
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {//а именно левая
			state = left; speed = 0.1;//двигаемся влево
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			state = right; speed = 0.1;//двигаемся вправо
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W))) {//если нажата клавиша вверх
			state = up; speed = 0.085;//двигаемся вверх (скорость меньше тк поле по y меньше)
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			state = down; speed = 0.085;//двигаемся вниз
		}
	}
	
	return 0;
}
//проверяем на столконвение с препятсвиями и хилимся если стоим на хилпоинте
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
			if (TileMap[i][j] == 'h' && state == stay && moveTimer > 1000)
			{
				//хилимся если стоим на месте более 1000 милисек
				health += 10;
				//каждые 500 милисек хилимся на +10
				moveTimer = 500;
				if (health > 500)
				{
					health = 500;//установили максимум здоровья
				}
			}
		}
	
	return 0;
}
void Player::animation()
{
	if (state != stay)
	{	//анимация движения колес раз в 0.1 сек меняем картинку
		if (moveTimer < 100)
		{
			wheelL.setTextureRect(sf::IntRect(1, 80, 19, 77));
			wheelR.setTextureRect(sf::IntRect(20, 81, 19, 77));
		}
		else if (moveTimer < 200)
		{
			wheelL.setTextureRect(sf::IntRect(40, 82, 19, 77));
			wheelR.setTextureRect(sf::IntRect(59, 82, 19, 77));
		}
		else
		{
			moveTimer = 0;
		}
		//Колеса двигаются)
	}
	gun.setRotation(gunrotation);
	//повернули спрайт пушку=и
	switch (state)
	{// разворачиваем кузов танка в зависимости от направления
	case Entity::left:
		frame.setRotation(-90);
		wheelL.setRotation(-90);
		wheelR.setRotation(-90);
		break;
	case Entity::right:
		frame.setRotation(90);
		wheelL.setRotation(90);
		wheelR.setRotation(90);
		break;
	case Entity::up:
		frame.setRotation(0);
		wheelL.setRotation(0);
		wheelR.setRotation(0);
		break;
	case Entity::down:
		frame.setRotation(180);
		wheelL.setRotation(180);
		wheelR.setRotation(180);
		break;
	default:
		break;
	}
	//далее анимация получения урона по таймеру
	if (struckTimer > 0)
	{
		gun.setColor(sf::Color::Red);
		frame.setColor(sf::Color::Red);
		wheelL.setColor(sf::Color::Red);
		wheelR.setColor(sf::Color::Red);
	}
	else
	{
		gun.setColor(sf::Color::White);
		frame.setColor(sf::Color::White);
		wheelL.setColor(sf::Color::White);
		wheelR.setColor(sf::Color::White);
	}//Красим при дамаге
	return;
}
int Player::update(float time, sf::String TileMap[HEIGHT_MAP], sf::Event event)
{
	if (!life)
	{
		return 0;//вернули 0 когда жизнь иссякла
	}
	if (struckTimer > 0)
	{
		struckTimer -= time;//таймер получения урона должен быть <= 0
	}
	moveTimer += time;// таймер движения
	control(event);//вызвали управление героем
	animation();// анимацию
	switch (state)//тут делаются различные действия в зависимости от состояния
		{// устанавливаем смещение и скорость
		case right: dx = speed; dy = 0; break;//состояние идти вправо
		case left: dx = -speed; dy = 0; break;//состояние идти влево
		case up: dx = 0; dy = -speed; break;// состояние поднятия наверх 
		case down: dx = 0; dy = speed; break;// состояние во время спуска  
		case stay: dx = 0; dy = 0; break;//стоим		
		}
	x += dx*time; //двигаем координату на смещение
	checkCollisionWithMap(dx, 0, TileMap);//обрабатываем столкновение по Х
	y += dy*time; //двигаем координату на смещение
	checkCollisionWithMap(0, dy, TileMap);//обрабатываем столкновение по Y
	frame.setPosition(x+w/2, y+h/2); //задаем позицию спрайта 
	wheelL.setPosition(x + w / 2, y + h / 2);
	wheelR.setPosition(x + w / 2, y + h / 2);
	gun.setPosition(x + w / 2, y + h / 2);
	if (health <= 0){life = false;}//жизнь покидает нас, когда здоровье полностью ушло
	speed = 0;// занулили скорость для контроля остановки ГГ если не нажата кнопка
	return 0;
}

sf::Vector2f Player::GetgunXY()
{
	sf::Vector2f buf = gun.getPosition();
	// возвращаем конец пушки, для генерации пули
	// можно было бы сразу пулю создавать, но.....
	//далее подстраиваем положение конца пушки, относительно угла поворота
	if (gunrotation < 45 && gunrotation > -45)
	{
		//up angle
		buf.x += 80 * cos((gunrotation - 90) / 180 * 3.14159265) + 20;
		buf.y += 80 * sin((gunrotation - 90) / 180 * 3.14159265);
	}
	else if (gunrotation < 135 && gunrotation > 45 || gunrotation < -225 && gunrotation > -270)
	{
		//right angle
		buf.x += 80 * cos((gunrotation - 90) / 180 * 3.14159265);
		buf.y += 80 * sin((gunrotation - 90) / 180 * 3.14159265) + 20;
	}
	else if (gunrotation < -45 && gunrotation > -135 || gunrotation < 270 && gunrotation > 225)
	{
		//left angle
		buf.x += 80 * cos((gunrotation - 90) / 180 * 3.14159265) ;
		buf.y += 80 * sin((gunrotation - 90) / 180 * 3.14159265) - 40;
	}
	else if(gunrotation < 225 && gunrotation > 135)
	{
		//down angle
		buf.x += 80 * cos((gunrotation - 90) / 180 * 3.14159265) - 40;
		buf.y += 80 * sin((gunrotation - 90) / 180 * 3.14159265);
	}
	return buf;
}

void Player::struck(int damage)
{
	health -= damage;//получили урон
	struckTimer = 200;//Задали таймер анимации получения урона
}
void Player::draw(sf::RenderTarget& target)
{
	//тк ГГ состоит из 4х частей, обычным способам его не отрисовать
	//поэтому сделан отдельный метод для отрисовки всех частей ГГ на экране
	target.draw(wheelL);
	target.draw(wheelR);
	target.draw(frame);
	target.draw(gun);
}