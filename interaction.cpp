#include "interaction.h"
#include "map.h"
Engine::Engine()
{
	
	map_image.loadFromFile("images/map.png");//загружаем файл для карты
	//текстура карты
	
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	s_map.setTexture(map);//заливаем текстуру спрайтом
	allImage.loadFromFile("images/robots.png");
	//загрузили изображения
	clock.restart(); //перезагружает время
	//Создаем объект героя
	Hero = new Player(allImage, 500, 500, 70, 80, "hero");

	GameOver = false;

	font.loadFromFile("images\\CyrilicOld.ttf");//шрифт загрузили
	text.setFont(font);
	text.setCharacterSize(24);
	text.setStyle(sf::Text::Bold);
	text.setPosition(1000, 5);
}

Engine::~Engine()
{
	delete Hero;
	enemies.clear();
	bullets.clear();
	//очищаем память, за собой нужно убирать
}

int Engine::play(int number)
{

	sf::RenderWindow window(sf::VideoMode(1280,800), "Game", sf::Style::Fullscreen);
	//1280*800
	//окно сформировали, сделали на полный экран
	sf::Cursor cursor;
	if (cursor.loadFromSystem(sf::Cursor::Cross))
		window.setMouseCursor(cursor);
	//устанавливаем курсор-крестик (типа прицел)
	if (number != 1 && !MainMenu(window))
	{
		return 0;
	}
	float timerspaun = 0;//таймер для появления врагов
	float gunTimer = 0;//для контроля выстрелов гг
	float spaunlvl = 5000;//время нужное спауна
	float timerLVLup = 0;//повышаем уровень, со временем
	sf::Image healthImg;
	healthImg.loadFromFile("images/Health.png");
	sf::Texture healthTexture;
	healthTexture.loadFromImage(healthImg);
	healthTexture.setRepeated(true);//Чтоб не рисовать - повторяем один и тот же texture в спрайте
	Health.setTexture(healthTexture);

	sf::Image gunDamageImg;
	gunDamageImg.loadFromFile("images/Bullet.png");
	sf::Texture gunDamageTexture;
	gunDamageTexture.loadFromImage(gunDamageImg);
	gunDamageTexture.setRepeated(true);
	GunDamage.setTexture(gunDamageTexture);
	//Поставили картинку здоровья гг и запал пушки
	Health.setTextureRect(sf::IntRect(0, 0, 32, 32));
	Health.setScale(0.5, 0.5);
	Health.setPosition(10,10);
	GunDamage.setTextureRect(sf::IntRect(0, 0, 70, 348));
	GunDamage.setScale(0.1, 0.1);
	GunDamage.setPosition(10, 30);
	while (window.isOpen() && !GameOver)
	{//Пока окно открыто и игра не закончена
		sf::Event event;//если нажата клавиша
		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{//нажали esc = открыли меню, игра на паузе
				switch (RestartMenu(window))
				{
				case 0:
					window.close();
					return 0;
				case 1:
					window.close();
					return 1;
				default:
					break;
				}
			}
		}
		time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры
		timerspaun += time;
		gunTimer += time;
		timerLVLup += time;
		if (timerLVLup > 30000)
		{
			//30 сек и повысили сложность + появился босс
			Enemy* anotherEnemy = new Enemy(allImage, 200, 200, 120, 90, "BOSSbot", TileMapMy);
			enemies.push_back(*anotherEnemy);//ukazatel chtob kartina bila
			spaunlvl -= 500;
			if (spaunlvl < 2000)
			{
				spaunlvl = 2000;
			}
			timerLVLup = 0;
			timerspaun = 0;
		}
 		//
		if (timerspaun > spaunlvl && enemies.size()<10)
		{
			//пусть будет не больше 10 врагов, ок? А то они летают там везде, сложно бегать от них
			Enemy* anotherEnemy = new Enemy(allImage, 200, 200, 45, 65, "flybot",TileMapMy);
			enemies.push_back(*anotherEnemy);//ukazatel chtob kartina bila
			timerspaun = 0;
		}

		Hero->update(time, TileMapMy, event);//Герой сделал свой ход
		Health.setTextureRect(sf::IntRect(0, 0, 32*(Hero->Gethealth()/20), 32));
		if (gunTimer > 1000)
		{
			GunDamage.setTextureRect(sf::IntRect(0, 0, 70 * 10, 348));
			//максимальный урон = большая полоска
		}
		else
		{
			GunDamage.setTextureRect(sf::IntRect(0, 0, 70 * (gunTimer / 100), 348));
		}
		
		if (gunTimer > 150 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			int damage = gunTimer / 10;
			if (damage > 100)
			{
				damage = 100;
			}
			if (damage > 20)
			{
				sf::Vector2f HeroXY = Hero->GetgunXY();
				Bullet* anotherBullet = new Bullet(allImage, HeroXY.x, HeroXY.y, 25, 25, Hero->GetRotation(), "HeroBullet", damage);
				bullets.push_back(*anotherBullet);//ukazatel chtob kartina bila
			}
			else
			{
				sf::Vector2f HeroXY = Hero->GetXY();
				Bullet* anotherBullet = new Bullet(allImage, HeroXY.x, HeroXY.y, 10, 10, Hero->GetRotation(), "HeroBullet", damage);
				bullets.push_back(*anotherBullet);//ukazatel chtob kartina bila
			}
			gunTimer = 0;
		}
		std::vector<Enemy>::iterator iterEnemies = enemies.begin();//итераторы для пуль и врагов в начало + создаем их
		std::vector<Bullet>::iterator iterBullet = bullets.begin();
		while (iterBullet != bullets.end())
		{
			if (iterBullet->isAlive())
			{
				iterBullet->update(time, TileMapMy);
				++iterBullet;//проходим по каждому объекту в векторе
			}
			else
			{
				
				iterBullet = bullets.erase(iterBullet); //стереть из списка
			}
		}
    	while (iterEnemies != enemies.end()) 
		{
			if (iterEnemies->isAlive())
			{
				sf::Vector2f BufXYHero = Hero->GetXY();
				iterEnemies->SetAim(BufXYHero);//цель - герой 
				iterEnemies->update(time);//враг сходил
				if (iterEnemies->GetName()=="BOSSbot" && iterEnemies->GetBOSSdamagetimer() > 2000)
				{
					//босс стреляет раз в 2 секунды
					sf::Vector2f BufXYEnemy = iterEnemies->GetXY();//берем координаты
					float rotation = rotation = atan2(BufXYHero.y - BufXYEnemy.y, BufXYHero.x - BufXYEnemy.x) * 180 / 3.14159265 + 90;
					//вычисляем угол поворота
					Bullet* anotherBullet = new Bullet(allImage, BufXYEnemy.x, BufXYEnemy.y, 30, 30, rotation, "BossBullet", 25);
					bullets.push_back(*anotherBullet);//создаем и добавляем в вектор пулю
					iterEnemies->recetBOSSdamagetimer();
				}

				iterBullet = bullets.begin();
				while (iterBullet != bullets.end())
				{
					if (iterBullet->isAlive())
					{
						if (iterBullet->GetName() != "BossBullet" && iterBullet->GetRect().intersects(iterEnemies->GetRect()))
						{
							iterEnemies->struck(iterBullet->GetDamage());
						}
						if (iterBullet->GetName() != "HeroBullet" && iterBullet->GetRect().intersects(Hero->GetRect()))
						{
							Hero->struck(iterBullet->GetDamage());
						}
						++iterBullet;
						//Находим пересечение хитбоксов пули и врагов с ГГ, далее уничтожается пуля, а отсальные получают урон
					}
					else
					{
						
						iterBullet = bullets.erase(iterBullet); //стереть из списка
					}
				}

				if(Hero->GetRect().intersects(iterEnemies->GetRect()))
				{
					if (iterEnemies->GetName() == "BOSSbot")
					{
						if (iterEnemies->GetStatus() != "anikilled")
						{
							if (iterEnemies->GetBOSSdamagetimer() > 500)
							{
								//При пересечении хитбоксов ГГ и босса, босс бъет раз в 500 мкс
								Hero->struck(20);
								iterEnemies->recetBOSSdamagetimer();
							}
							
						}
					}
					else
					{
						iterEnemies->struck(100);
						//Другие же получают урон, несовместимый с жизнью
						if (iterEnemies->GetStatus() != "anikilled")
						{
							Hero->struck(20);	
						}
					}
					
				}
				++iterEnemies;
			}
			else 
			{
				if (iterEnemies->GetName() == "BOSSbot")
				{
				Hero->Addscore(500);//Враг убит - получили очки
				}
				else if (iterEnemies->GetName() == "flybot")
				{
					Hero->Addscore(100);
				}
				iterEnemies = enemies.erase(iterEnemies); //стереть из списка
			}
			
		}
		GameOver = !Hero->isAlive();
		//drawing ->
		window.clear();
		/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TileMapMy[i][j] == ' ')  s_map.setTextureRect(sf::IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
			else if (TileMapMy[i][j] == '0')  s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));//если встретили символ 0, то рисуем 2й квадратик
			else if (TileMapMy[i][j] == 'b') s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));//если встретили символ b, то рисуем 3й квадратик
			else if (TileMapMy[i][j] == 'p') s_map.setTextureRect(sf::IntRect(96, 0, 32, 32));//если встретили символ p, то рисуем 4й квадратик
			else if (TileMapMy[i][j] == 'h') s_map.setTextureRect(sf::IntRect(128, 0, 32, 32));//если встретили символ h, то рисуем 5й квадратик
			else s_map.setTextureRect(sf::IntRect(160, 0, 32, 32));
			s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. 
			//то есть задает каждому из них позицию.
			window.draw(s_map);//рисуем квадратики на экран
		}
		Hero->draw(window);//рисуется герой-танк
		iterEnemies = enemies.begin();
    	while (iterEnemies != enemies.end()) {
			if (iterEnemies->GetName() != "BOSSbot")
			{
				iterEnemies->draw(window);
				//не босс рисуется
				//босс выше всех летает
			}
			++iterEnemies;
    	}
		iterBullet = bullets.begin();
		while (iterBullet != bullets.end()) {
			window.draw(iterBullet->sprite);
			++iterBullet;
		}
		iterEnemies = enemies.begin();
		while (iterEnemies != enemies.end()) {
			if (iterEnemies->GetName() == "BOSSbot")
			{
				iterEnemies->draw(window);
			}
			++iterEnemies;
		}
		window.draw(GunDamage);
		window.draw(Health);
		text.setString("Score: " + std::to_string(Hero->Getscore()));//преобразовали цифру в текст и показали
		window.draw(text);
		window.display();
	}
	if (RestartMenu(window))
	{
		window.close();
		return 1;
	}
	
	window.close();
	return 0;
}

int Engine::RestartMenu(sf::RenderWindow& target)
{
	sf::Texture menuTexturePlay, menuTextureQuit, menuTextureRestart;
	menuTexturePlay.loadFromFile("images/Play.png");
	menuTextureQuit.loadFromFile("images/Quit.png");
	menuTextureRestart.loadFromFile("images/Restart.png");
	sf::Sprite menuPlay(menuTexturePlay), menuQuit(menuTextureQuit), menuRestart(menuTextureRestart);
	bool isMenu = 1;
	int menuNum = 0;
	if (!GameOver)
	{
		menuRestart.setPosition(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16 - 120);
		menuPlay.setPosition(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16);
	}
	else
	{
		menuPlay.setPosition(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16 - 120);
		menuRestart.setPosition(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16);
	}
	menuQuit.setPosition(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16 + 120);
	//Расставили кнопки
	//////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{
		menuRestart.setColor(sf::Color::White);
		menuPlay.setColor(sf::Color::White);
		menuQuit.setColor(sf::Color::White);
		menuNum = 0;
		sf::RectangleShape rectangle(sf::Vector2f(20, 20));
		rectangle.setSize(sf::Vector2f(WIDTH_MAP*32-40, HEIGHT_MAP*32-40));
		if (!GameOver)
		{
			rectangle.setFillColor(sf::Color(255, 228, 200, 1));
			rectangle.setPosition(sf::Vector2f(20, 20));
			if (sf::IntRect(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16 - 120, 310, 110).contains(sf::Mouse::getPosition()))
			{
				menuRestart.setColor(sf::Color::Blue); menuNum = 1;// 1 = рестарт
			}
			if (sf::IntRect(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16, 310, 110).contains(sf::Mouse::getPosition()))
			{
				menuPlay.setColor(sf::Color::Blue); menuNum = 2;
			}
			if (sf::IntRect(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16 + 120, 310, 110).contains(sf::Mouse::getPosition()))
			{
				menuQuit.setColor(sf::Color::Blue); menuNum = 0;
			}
		}
		else
		{
			//если игра окончена, то саму игру надо закрасить
			target.clear(sf::Color(255, 228, 200));
			text.setCharacterSize(64);
			text.setPosition(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16 - 120);
			if (sf::IntRect(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16, 310, 110).contains(sf::Mouse::getPosition()))
			{
				menuRestart.setColor(sf::Color::Blue); menuNum = 1;
			}
			if (sf::IntRect(WIDTH_MAP * 16 - 155, HEIGHT_MAP * 16 + 120, 310, 110).contains(sf::Mouse::getPosition()))
			{
				menuQuit.setColor(sf::Color::Blue); menuNum = 0;
			}
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			
			return menuNum;

		}
		if (!GameOver)
		{
			target.draw(rectangle);
			target.draw(menuPlay);
		}
		target.draw(menuRestart);
		target.draw(menuQuit);
		text.setString("Score: " + std::to_string(Hero->Getscore()));
		target.draw(text);

		target.display();
	}
	return 0;
}

int Engine::MainMenu(sf::RenderWindow& target)
{
	{
		sf::Texture menuTexturePlay, menuTextureQuit, menuBackground;
		menuTexturePlay.loadFromFile("images/Play.png");
		menuTextureQuit.loadFromFile("images/Quit.png");
		menuBackground.loadFromFile("images/jogaGame.png");
		sf::Sprite menuPlay(menuTexturePlay), menuQuit(menuTextureQuit), menuBg(menuBackground);
		//menuBackground.setSmooth(true);
		bool isMenu = 1;
		int menuNum = 0;
		menuPlay.setPosition(100, 200);
		menuQuit.setPosition(100, 500);
		menuBg.setPosition(0, 0);//красивый background 1280х800px
		//////////////////////////////МЕНЮ///////////////////
		while (isMenu)
		{
			menuPlay.setColor(sf::Color::White);
			menuQuit.setColor(sf::Color::White);
			menuNum = 0;
			target.clear();

			if (sf::IntRect(100, 200, 310, 110).contains(sf::Mouse::getPosition())) { menuPlay.setColor(sf::Color::Blue); menuNum = 1; }
			if (sf::IntRect(100, 500, 310, 110).contains(sf::Mouse::getPosition())) { menuQuit.setColor(sf::Color::Blue); menuNum = 2; }

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (menuNum == 1)
				{
					isMenu = false;//если нажали первую кнопку, то выходим из меню 
					return 1;
				}
				if (menuNum == 2)
				{
					isMenu = false;
					return 0;
				}

			}
			//target = window
			target.draw(menuBg);
			target.draw(menuPlay);
			target.draw(menuQuit);
			target.display();
		}
		return 0;
	}
}
