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

	////
	//Создаем объект героя
	Hero = new Player(allImage, 500, 500, 70, 80, "hero");
	////

	////
	//Создаем объект Enemy
	//Enemy *anotherEnemy = new Enemy(allImage, 200, 200, 45, 65, "flybot", TileMapMy);
    //enemies.push_back(*anotherEnemy);//ukazatel chtob kartina bila
	
	

	//Bullet* anotherBullet = new Bullet(allImage, 100, 100, 16, 16, Hero->GetRotation(), "HeroBullet");
	//bullets.push_back(*anotherBullet);//ukazatel chtob kartina bila

	GameOver = false;

	font.loadFromFile("images\\CyrilicOld.ttf");
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
}

int Engine::play(int number)
{
	sf::RenderWindow window(sf::VideoMode(1280, 800), "Game", sf::Style::Fullscreen);
	sf::Cursor cursor;
	if (cursor.loadFromSystem(sf::Cursor::Cross))
		window.setMouseCursor(cursor);
	if (number != 1 && !MainMenu(window))
	{
		return 0;
	}
	float timerspaun = 0;
	float gunTimer = 0;
	float spaunlvl = 5000;
	float timerLVLup = 0;
	sf::Image healthImg;
	healthImg.loadFromFile("images/Health.png");
	sf::Texture healthTexture;
	healthTexture.loadFromImage(healthImg);
	healthTexture.setRepeated(true);
	Health.setTexture(healthTexture);

	sf::Image gunDamageImg;
	gunDamageImg.loadFromFile("images/Bullet.png");
	sf::Texture gunDamageTexture;
	gunDamageTexture.loadFromImage(gunDamageImg);
	gunDamageTexture.setRepeated(true);
	GunDamage.setTexture(gunDamageTexture);

	Health.setTextureRect(sf::IntRect(0, 0, 32, 32));
	Health.setScale(0.5, 0.5);
	Health.setPosition(10,10);
	GunDamage.setTextureRect(sf::IntRect(0, 0, 70, 348));
	GunDamage.setScale(0.1, 0.1);
	GunDamage.setPosition(10, 30);
	while (window.isOpen() && !GameOver)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
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
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {//если нажата клавиша 
			//	GameOver = true;
			//}
		}
		time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры
		timerspaun += time;
		gunTimer += time;
		timerLVLup += time;
		if (timerLVLup > 30000)
		{
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
			Enemy* anotherEnemy = new Enemy(allImage, 200, 200, 45, 65, "flybot",TileMapMy);
			enemies.push_back(*anotherEnemy);//ukazatel chtob kartina bila
			timerspaun = 0;
		}

		Hero->update(time, TileMapMy, event);
		Health.setTextureRect(sf::IntRect(0, 0, 32*(Hero->Gethealth()/20), 32));
		if (gunTimer > 1000)
		{
			GunDamage.setTextureRect(sf::IntRect(0, 0, 70 * 10, 348));
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
		std::vector<Enemy>::iterator iterEnemies = enemies.begin();
		std::vector<Bullet>::iterator iterBullet = bullets.begin();
		while (iterBullet != bullets.end())
		{
			if (iterBullet->isAlive())
			{
				iterBullet->update(time, TileMapMy);
				++iterBullet;
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
				iterEnemies->SetAim(BufXYHero);
				iterEnemies->update(time);
				if (iterEnemies->GetName()=="BOSSbot" && iterEnemies->GetBOSSdamagetimer() > 2000)
				{
					sf::Vector2f BufXYEnemy = iterEnemies->GetXY();
					float rotation = rotation = atan2(BufXYHero.y - BufXYEnemy.y, BufXYHero.x - BufXYEnemy.x) * 180 / 3.14159265 + 90;
					Bullet* anotherBullet = new Bullet(allImage, BufXYEnemy.x, BufXYEnemy.y, 30, 30, rotation, "BossBullet", 25);
					bullets.push_back(*anotherBullet);
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
								Hero->struck(20);
								iterEnemies->recetBOSSdamagetimer();
							}
							
						}
					}
					else
					{
						iterEnemies->struck(100);
						if (iterEnemies->GetStatus() != "anikilled")
						{
							Hero->struck(20);
							
						}
					}
					
				}
				++iterEnemies;
				//if (!enemies.empty())
			}
			else 
			{
				if (iterEnemies->GetName() == "BOSSbot")
				{
				Hero->Addscore(500);
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
		//window.draw(robotsprite);
		//window.draw(Hero->sprite);
		Hero->draw(window);
		iterEnemies = enemies.begin();
    	while (iterEnemies != enemies.end()) {
			if (iterEnemies->GetName() != "BOSSbot")
			{
				iterEnemies->draw(window);
			}
			++iterEnemies;
    	}
		iterBullet = bullets.begin();
		while (iterBullet != bullets.end()) {
			// определяем прямоугольник размером 120x50
			window.draw(iterBullet->sprite);
			/*sf::RectangleShape rectangle(sf::Vector2f(iterBullet->GetRect().left, iterBullet->GetRect().top));
			rectangle.setSize(sf::Vector2f(iterBullet->GetRect().width, iterBullet->GetRect().height));
			rectangle.setFillColor(sf::Color(255,0,0,100));
			rectangle.setPosition(sf::Vector2f(iterBullet->GetRect().left, iterBullet->GetRect().top));
			window.draw(rectangle);*/
			
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
		//window.draw(MyEnemy->sprite);
		window.draw(GunDamage);
		window.draw(Health);
		text.setString("Score: " + std::to_string(Hero->Getscore()));
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
	//menuBackground.loadFromFile("images/jogaGame.png");
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
				menuRestart.setColor(sf::Color::Blue); menuNum = 1;
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
		//menuTexture3.loadFromFile("images/Restart.png");
		menuBackground.loadFromFile("images/jogaGame.png");
		sf::Sprite menuPlay(menuTexturePlay), menuQuit(menuTextureQuit), menuBg(menuBackground);
		bool isMenu = 1;
		int menuNum = 0;
		menuPlay.setPosition(100, 200);
		menuQuit.setPosition(100, 500);
		menuBg.setPosition(0, 0);
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

			target.draw(menuBg);
			target.draw(menuPlay);
			target.draw(menuQuit);


			target.display();
		}
		return 0;
	}
}
