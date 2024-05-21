#pragma once
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "Obstacle.h"
#include <vector>

class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update(float time);
    void generateObstacle();

    sf::RenderWindow window;
    sf::Clock clock;
    sf::Texture tRoad, tBackGround, tStartButton, tPit, tCar;
    sf::Sprite road, startButton, backGround;
    Car car;
    std::vector<Obstacle> obstacles;
    float acceleration;
    bool onMenu;
    bool inGame;
    bool onEndScreen;
    float obstacleSpawnTimer;
};
