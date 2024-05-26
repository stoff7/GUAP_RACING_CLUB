#pragma once
#include <SFML/Graphics.hpp>

class Car {
public:
    Car(sf::Texture& t);
    void update(float time);
    sf::Sprite sprite;
    float dx;
private:
    sf::FloatRect rect;
    bool onRoad;
    bool hasCollision;
    float currentFrame;
};