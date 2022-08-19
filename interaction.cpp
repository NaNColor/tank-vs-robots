#include "interaction.h"
#include "map.h"
#include <iostream>
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
	Enemy *anotherEnemy = new Enemy(allImage, 200, 200, 45, 65, "flybot", TileMapMy);
    enemies.push_back(*anotherEnemy);//ukazatel chtob kartina bila
	
	//Bullet* anotherBullet = new Bullet(allImage, 100, 100, 16, 16, Hero->GetRotation(), "HeroBullet");
	//bullets.push_back(*anotherBullet);//ukazatel chtob kartina bila

	GameOver = false;
}

Engine::~Engine()
{
	delete Hero;
	enemies.clear();
	bullets.clear();
}

void Engine::start()
{
	sf::RenderWindow window(sf::VideoMode(1280, 800), "Game", sf::Style::Fullscreen);
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
				window.close();
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
		if (timerLVLup > 10000)
		{
			spaunlvl -= 500;
			if (spaunlvl < 2000)
			{
				spaunlvl = 2000;
			}
			timerLVLup = 0;
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
		
		if (gunTimer > 100 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			int damage = gunTimer / 10;
			if (damage > 100)
			{
				damage = 100;
			}
			if (damage > 20)
			{
				sf::Vector2f HeroXY = Hero->GetgunXY();
				Bullet* anotherBullet = new Bullet(allImage, HeroXY.x, HeroXY.y, 20, 20, Hero->GetRotation(), "HeroBullet", damage);
				bullets.push_back(*anotherBullet);//ukazatel chtob kartina bila
			}
			else
			{
				sf::Vector2f HeroXY = Hero->GetXY();
				Bullet* anotherBullet = new Bullet(allImage, HeroXY.x, HeroXY.y, 10, 10, Hero->GetRotation(), "HeroBullet", damage);
				bullets.push_back(*anotherBullet);//ukazatel chtob kartina bila
			}
			
			
			//std::cout << Hero->GetRotation() << std::endl;
			gunTimer = 0;
		}
		sf::Vector2f EnemyXY;
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
				iterEnemies->SetAim(Hero->GetXY());
				iterEnemies->update(time, TileMapMy);
				//EnemyXY = iterEnemies->GetXY();

				iterBullet = bullets.begin();
				while (iterBullet != bullets.end())
				{
					if (iterBullet->isAlive())
					{
						if (iterBullet->GetRect().intersects(iterEnemies->GetRect()))
						{
							iterEnemies->struck(iterBullet->GetDamage());
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
					iterEnemies->struck(100);
					
					if (iterEnemies->GetStatus() != "anikilled")
					{
						Hero->struck(20);
						Hero->Addscore(100);
					}
					
				}
				++iterEnemies;
				//if (!enemies.empty())
			}
			else 
			{
				iterEnemies = enemies.erase(iterEnemies); //стереть из списка
			}
			//if (!bullets.empty())
			//{
			//	iterBullet = bullets.begin();
			//	while (iterBullet != bullets.end())
			//	{
			//		if (iterBullet->isAlive())
			//		{
			//			iterBullet->update(time, TileMapMy);
			//			if (iterBullet->GetRect().intersects(iterEnemies->GetRect()))
			//			{
			//				iterEnemies->struck(iterBullet->GetDamage());
			//			}
			//			++iterBullet;
			//		}
			//		else
			//		{
			//			iterBullet = bullets.erase(iterBullet); //стереть из списка
			//		}
			//	}
			//}
			
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
    		window.draw(iterEnemies->sprite);
			++iterEnemies;
    	}
		iterBullet = bullets.begin();
		while (iterBullet != bullets.end()) {
			window.draw(iterBullet->sprite);
			++iterBullet;
		}
		//window.draw(MyEnemy->sprite);
		window.draw(GunDamage);
		window.draw(Health);
		window.display();
	}
}
