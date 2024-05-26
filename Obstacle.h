#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    Obstacle(sf::Texture& t);
    void update(float time);
    sf::Sprite sprite;
private:
    sf::FloatRect rect;
    float dx;
    float dy;
    bool onRoad;
    float currentFrame;
};