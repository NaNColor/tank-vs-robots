#pragma once
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
class Engine {
    public:
        Engine();
        ~Engine();
        void start();

    private:
        Player* Hero;
        std::vector<Bullet> bullets;
        std::vector<Enemy> enemies;// вектор врагов
        sf::Clock clock;
        float time;
        sf::Font font;
        sf::Text text;
        sf::Image map_image;//объект изображения для карты
        sf::Image allImage;//Все изображения, которые исполузуются поверх карты
        sf::Sprite s_map;//спрайт для карты
        sf::Texture map;
        sf::Sprite Health;
        sf::Sprite GunDamage;
        bool GameOver;//тригер для окончания игры
        int MainMenu(sf::RenderWindow& target);
        
};
