#pragma once
#include <SFML/Graphics.hpp>

class Car {
public:
    Car(sf::Texture& t);
    void update(float time, float acceleration);
    sf::Sprite sprite;
    float dx;
private:
    sf::FloatRect rect;
    bool onRoad;
    bool hasCollision;
    float currentFrame;
    float centerStayTime; // Таймер нахождения в центре
    bool inCenter; // Флаг, указывающий, что машина в центре
};
