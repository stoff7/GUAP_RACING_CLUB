#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Resources {
public:
    sf::Texture tcar, tRoad, tBackGround, tStartButton, tEndScreen, tCar1, tCar2, tCar3;
    std::vector<sf::Texture> obstacleTextures;

    Resources();
};
