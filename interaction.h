#pragma once
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
class Engine {
    public:
        Engine();
        ~Engine();
        int play(int number);

    private:
        Player* Hero;
        std::vector<Bullet> bullets;// вектор пуль
        std::vector<Enemy> enemies;// вектор врагов
        sf::Clock clock;// аппаратный таймер
        float time;// основное время, отвечает за скорость игры
        sf::Font font;// шрифт для текста
        sf::Text text;// текст
        sf::Image map_image;//объект изображения для карты
        sf::Image allImage;//Все изображения, которые исполузуются поверх карты
        sf::Sprite s_map;//спрайт для карты
        sf::Texture map;
        sf::Sprite Health;//для здоровья
        sf::Sprite GunDamage;//для уровня заряда пушки
        bool GameOver;//тригер для окончания игры
        int MainMenu(sf::RenderWindow& target);//меню при старте
        int RestartMenu(sf::RenderWindow& target);//меню внутри игры
};
